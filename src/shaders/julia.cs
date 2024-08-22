#define BOUNDING_RADIUS_2	3.0
#define ESCAPE_THRESHOLD   	10
#define DEL                	1e-4

typedef float4 quat_t;

RWTexture2D<float4> output_tex : register(u0);

cbuffer params : register(b0)
{
	float4		mu;
	int			width,
				height;
	bool		self_shadow;
	float		epsilon;
	float		zoom;
	int			iterations;
	row_major float4x4	rotation;
};

quat_t		quat_mul(quat_t q1, quat_t q2);
quat_t		quat_sq(quat_t q);
float3		normal_estimate(float3 p, quat_t c, uint max_iters);
float		intersect_julia(inout float3 ro, inout float3 rd, quat_t c, uint max_iters, float eps);
float3		phong(float3 light_pos, float3 eye, float3 p, float3 n);
float3		intersect_sphere(float3 ro, float3 rd);
float4		qjulia(float3 ro, float3 rd, quat_t mu, float eps, float3 eye, float3 light_pos, bool render_shadows, uint max_iters);

[numthreads(16, 16, 1)]
void
main(uint3 thread_id : SV_DispatchThreadID)
{
	float4		coord;
	float2		size,
				half,
				position;
	float		scale;
	float4		light,
				eye,
				ray;
	float3		ro,
				rd;


	coord = float4((float)thread_id.x, (float)thread_id.y, 0, 1);
	size = float2(width, height);
	scale = min(size.x, size.y);
	half = float2(0.5f, 0.5f);
	position = (coord.xy - half * size) / scale * BOUNDING_RADIUS_2 * (1 / zoom);

  	light = float4(1.5f, 0.5f, 4.0f, 1.0f);
  	eye   = float4(0.0f, 0.0f, 4.0f, 1.0f);
  	ray   = float4(position.x, position.y, 0.0f, 1.0f);

	light = mul(rotation, light);
	eye = mul(rotation, eye);
	ray = mul(rotation, ray);

	ro = eye.xyz;
	rd = ray.xyz - ro;

	float4 color = qjulia(ro, rd, mu, epsilon, eye.xyz, light.xyz, self_shadow, iterations);

	output_tex[thread_id.xy] = color;
}

quat_t
quat_mul(quat_t q1,
		 quat_t q2)
{
	quat_t		r;


	r.x = q1.x*q2.x - dot(q1.yzw, q2.yzw);
	r.yzw = q1.x*q2.yzw + q2.x*q1.yzw + cross(q1.yzw, q2.yzw);

	return (r);
}

quat_t
quat_sq(quat_t q)
{
	quat_t		r;


	r.x = q.x*q.x - dot( q.yzw, q.yzw );
	r.yzw = 2*q.x*q.yzw;

	return (r);
}

float3		
normal_estimate(float3 p, 
				quat_t c, 
				uint max_iters)
{
	float3		n;
	quat_t		qp = quat_t(p, 0);
	float		grad_x,
				grad_y,
				grad_z;


	quat_t gx1 = qp - quat_t(DEL, 0, 0, 0);
	quat_t gx2 = qp + quat_t(DEL, 0, 0, 0);
	quat_t gy1 = qp - quat_t(0, DEL, 0, 0);
	quat_t gy2 = qp + quat_t(0, DEL, 0, 0);
	quat_t gz1 = qp - quat_t(0, 0, DEL, 0);
	quat_t gz2 = qp + quat_t(0, 0, DEL, 0);

	for (uint i = 0; i < max_iters; i++)
	{
		gx1 = quat_sq(gx1) + c;
		gx2 = quat_sq(gx2) + c;
		gy1 = quat_sq(gy1) + c;
		gy2 = quat_sq(gy2) + c;
		gz1 = quat_sq(gz1) + c;
		gz2 = quat_sq(gz2) + c;
	}

	grad_x = length(gx2) - length(gx1);
	grad_y = length(gy2) - length(gy1);
	grad_z = length(gz2) - length(gz1);

	n = normalize(float3(grad_x, grad_y, grad_z));

	return (n);
}

float		
intersect_julia(inout float3 ro, 
				inout float3 rd, 
				quat_t c, 
				uint max_iters, 
				float eps)
{
	float		dist = eps;
	float		dd = 0;

	while (dist >= eps && dd < BOUNDING_RADIUS_2)
	{
		quat_t z = quat_t(ro, 0);
		quat_t zp = quat_t(1, 0, 0, 0);
		float norm_z;
		float zd = 0;
		uint count = 0;

		// NOTE(matthew): we inline this because otherwise it would hang.
		while (zd < ESCAPE_THRESHOLD && count < max_iters)
		{
			zp = 2.0f * quat_mul(z, zp);
			z = quat_sq(z) + c;
			zd = dot(z,z);
			count++;
		}

		norm_z = length(z);
		dist = 0.5f * norm_z * log(norm_z) / length(zp);

		ro += rd * dist;

		dd = dot(ro, ro);
	}

	return (dist);
}

float3		
phong(float3 light_pos, 
	  float3 eye, 
	  float3 p, 
	  float3 n)
{
	float3		diffuse = float3(169.0f / 255.0f, 140.0f / 255.0f, 112.0f / 255.0f);
	uint		spec_exp = 10;
	float		specularity = 0.45f;

	float3 l = normalize(light_pos - p);
	float3 e = normalize(eye - p);
	float ndotl = dot(n,l);
	float3 r = l - 2 * ndotl * n;

	/* diffuse += abs(n) * 0.3f; */

	return (diffuse * max(ndotl, 0) + specularity * pow(max(dot(e,r),0), spec_exp));
}

float3		
intersect_sphere(float3 ro,
				 float3 rd)
{
	float		b, c, d, t0, t1, t;


	b = 2 * dot(ro, rd);
	c = dot(ro, ro) - BOUNDING_RADIUS_2;
	d = sqrt(b * b - 4 * c);
	t0 = (-b + d) * 0.5f;
	t1 = (-b - d) * 0.5f;
	t = min(t0, t1);

	ro += t * rd;

	return (ro);
}

float4		
qjulia(float3 ro, 
	   float3 rd, 
	   quat_t mu, 
	   float eps, 
	   float3 eye, 
	   float3 light_pos, 
	   bool render_shadows,
	   uint max_iters)
{
	float4		bg_color = float4(0.1f, 0.1f, 0.1f, 1.0f);
	float4		color;
	float		dist;


	color = bg_color;

	rd = normalize(rd);
	ro = intersect_sphere(ro, rd);
	
	dist = intersect_julia(ro, rd, mu, max_iters, eps);

	if (dist < eps)
	{
		float3 n = normal_estimate(ro, mu, max_iters);

		color.rgb = phong(light_pos, rd, ro, n);

		if (render_shadows)
		{
			float3 l = normalize(light_pos - ro);
			ro += n * eps * 2.0f;
			dist = intersect_julia(ro, l, mu, max_iters, eps);

			if (dist < eps)
			{
				color.rgb *= 0.4f;
			}
		}
	}

	return (color);
}


