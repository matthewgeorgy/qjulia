#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__

///////////////////
// Matrix3x3
///////////////////

struct mat3_t
{
	// NOTE(matthew): this will actually behave as a 3x3 matrix, but it needs
	// to be 4x4 because HLSL.
	f32		elements[4][4];
};

mat3_t mat3_identity(void);
mat3_t mat3_mul(mat3_t &m1, mat3_t &m2);
mat3_t mat3_rotate(f32 xa, f32 ya, f32 za, f32 angle);



///////////////////
// Trackball
///////////////////

struct trackball_t
{
    f32			x0, y0, z0; 	// start point on ball
    f32			x1, y1, z1;		// end point on ball for rotation
    mat3_t		rotate;     	// delta rotation while dragging
    mat3_t 		orientation;	// orientation of trackball


  trackball_t();

  void			point_on_trackball(f32 &x, f32 &y, f32 &z, s32 mouse_x, s32 mouse_y, s32 mouse_width, s32 mouse_height);
  void 			drag_start(s32 mouse_x, s32 mouse_y, s32 mouse_width, s32 mouse_height);
  void  		drag_move(s32 mouse_x, s32 mouse_y, s32 mouse_width, s32 mouse_height);
  void 			drag_end();
  mat3_t		get_rotation_matrix();
};


mat3_t
mat3_identity(void)
{
	mat3_t		m;


	for (u32 j = 0; j < 3; j++)
	{
    	for (u32 i = 0; i < 3; i++)
		{
      		m.elements[j][i] = 0;
		}
	}

    m.elements[0][0] = 1;
    m.elements[1][1] = 1;
    m.elements[2][2] = 1;

	return (m);
}

mat3_t
mat3_mul(mat3_t &m1,
		 mat3_t &m2)
{
	mat3_t		res;


    for (u32 j = 0; j < 3; j++)
	{
    	for (u32 i = 0; i < 3; i++)
    	{
      		res.elements[j][i] = 0;
      		for (u32 k = 0; k < 3; k++)
			{
        		res.elements[j][i] += m1.elements[j][k] * m2.elements[k][i];
			}
    	}
	}

    return res;
}

mat3_t
mat3_rotate(f32 xa,
			f32 ya,
			f32 za,
			f32 angle)
{
	mat3_t		m;
	f32			t,
				ca,
				sa;


	t = sqrtf(xa * xa + ya * ya + za * za);

	if (fabs(t) > 1e-6) // check that t is non-zero
	{
		xa /= t;
		ya /= t;
		za /= t;
	}

	ca = cosf(angle);
	sa = sinf(angle);

	m.elements[0][0] = 1 + (1 - ca) * (xa * xa - 1);
	m.elements[1][1] = 1 + (1 - ca) * (ya * ya - 1);
	m.elements[2][2] = 1 + (1 - ca) * (za * za - 1);

	m.elements[0][1] =  za * sa + (1 - ca) * xa * ya;
	m.elements[1][0] = -za * sa + (1 - ca) * xa * ya;

	m.elements[0][2] = -ya * sa + (1 - ca) * xa * za;
	m.elements[2][0] =  ya * sa + (1 - ca) * xa * za;

	m.elements[1][2] =  xa * sa + (1 - ca) * ya * za;
	m.elements[2][1] = -xa * sa + (1 - ca) * ya * za;

	return (m);
}

trackball_t::trackball_t()
{
    orientation = mat3_identity();
    rotate = mat3_identity();
}


void
trackball_t::point_on_trackball(f32 &x,
							    f32 &y,
								f32 &z,
								s32 mouse_x,
								s32 mouse_y,
								s32 mouse_width,
								s32 mouse_height)
{
	f32		r2;


    x =  (f32)(mouse_x - mouse_width / 2) / (f32)min(mouse_width, mouse_height) * 2;
    y =  (f32)(mouse_y - mouse_height / 2) / (f32)min(mouse_width, mouse_height) * 2;

    r2 = x * x + y * y;

    if (r2 < 1)
	{
		z = sqrtf(1- r2);
	}
    else
    {
		x = x / sqrtf(r2);
		y = y / sqrtf(r2);
		z = 0;
    }
}

void
trackball_t::drag_start(s32 mouse_x,
						s32 mouse_y,
						s32 mouse_width,
						s32 mouse_height)
{
	point_on_trackball(x0, y0, z0, mouse_x, mouse_y, mouse_width, mouse_height);
}

void
trackball_t::drag_move(s32 mouse_x,
					   s32 mouse_y,
					   s32 mouse_width,
					   s32 mouse_height)
{
	f32		xa,
			ya,
			za,
			angle;


	point_on_trackball(x1, y1, z1, mouse_x, mouse_y, mouse_width, mouse_height);

	// rotation axis  = cross(p0, p1)
	xa = y0 * z1 - y1 * z0;
	ya = z0 * x1 - z1 * x0;
	za = x0 * y1 - x1 * y0;

	// rotation angle = acos(dot(p0,p1))
 	// multiply angle by 2 for 360 degree range
	angle = 2 * acosf(min(max(x0 * x1 + y0 * y1 + z0 * z1, -1), 1));

	// rotate matrix around axis
	/* rotate.rotateAroundAxis(xa, ya, za, angle); */
	rotate = mat3_rotate(xa, ya, za, angle);
}

void trackball_t::drag_end()
{
	orientation = mat3_mul(orientation, rotate);
	rotate = mat3_identity();
}

mat3_t trackball_t::get_rotation_matrix()
{
	return mat3_mul(orientation, rotate);
}

#endif // __TRACKBALL_H__

