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

mat3_t mat3_identity();
mat3_t mat3_mul(mat3_t &m1, mat3_t &m2);
mat3_t mat3_rotate(f32 xa, f32 ya, f32 za, f32 angle);



///////////////////
// Trackball
///////////////////

struct TrackBall
{
    float x0, y0, z0;  // point on trackball when mouse down at start rotation mouse drag
    float x1, y1, z1;  // point on trackball at mouse drag positioin

    mat3_t rotate;       // delta rotation while dragging
    mat3_t orientation;  // orientation of trackball


  TrackBall();

  void PointOnTrackBall(float &x, float &y, float &z, int mouseX, int mouseY, int mouseWidth, int mouseHeight);

  void DragStart(int mouseX, int mouseY, int mouseWidth, int mouseHeight);
  void  DragMove(int mouseX, int mouseY, int mouseWidth, int mouseHeight);
  void DragEnd();

  mat3_t GetRotationMatrix();
};


mat3_t mat3_identity()
{
	mat3_t m;

	for (int j=0; j<3; j++)
    	for (int i=0; i<3; i++)
      		m.elements[j][i] = 0;

    m.elements[0][0] = 1;
    m.elements[1][1] = 1;
    m.elements[2][2] = 1;

	return (m);
}

mat3_t mat3_mul(mat3_t &m1, mat3_t &m2)
{
	mat3_t res;

    for (int j=0; j<3; j++)
	{
    	for (int i=0; i<3; i++)
    	{
      		res.elements[j][i] = 0;
      		for (int k=0; k<3; k++)
			{
        		res.elements[j][i] += m1.elements[j][k] * m2.elements[k][i];      
			}
    	}
	}

    return res;
}

mat3_t mat3_rotate(f32 xa, f32 ya, f32 za, f32 angle)
{
	mat3_t m;

	float t = sqrtf(xa*xa+ya*ya+za*za);
      if (t!=0)
      {
        xa/=t;
        ya/=t;
        za/=t;
      }

	  float ca = cosf(angle);
	  float sa = sinf(angle);

      m.elements[0][0] = 1+(1-ca)*(xa*xa-1);
      m.elements[1][1] = 1+(1-ca)*(ya*ya-1);
      m.elements[2][2] = 1+(1-ca)*(za*za-1);

      m.elements[0][1] = +za*sa+(1-ca)*xa*ya;
      m.elements[1][0] = -za*sa+(1-ca)*xa*ya;

      m.elements[0][2] = -ya*sa+(1-ca)*xa*za;
      m.elements[2][0] = +ya*sa+(1-ca)*xa*za;

      m.elements[1][2] = +xa*sa+(1-ca)*ya*za;
      m.elements[2][1] = -xa*sa+(1-ca)*ya*za;

	  return (m);
}

TrackBall::TrackBall()
{
    orientation = mat3_identity();
    rotate = mat3_identity();
}


//***********************************************************************************
void TrackBall::PointOnTrackBall(float &x, float &y, float &z, int mouseX, int mouseY, int mouseWidth, int mouseHeight)
//***********************************************************************************
{
    x =  (float)(mouseX - mouseWidth/2)  / (float)min(mouseWidth, mouseHeight)*2;
    y =  (float)(mouseY - mouseHeight/2) / (float)min(mouseWidth, mouseHeight)*2;

    float rr = x*x+y*y;

    if ( rr < 1)
      z = sqrtf(1- rr);
    else
    {
      x = x / sqrtf(rr);
      y = y / sqrtf(rr);
      z = 0;
    }
}


void TrackBall::DragStart(int mouseX, int mouseY, int mouseWidth, int mouseHeight)
{
	PointOnTrackBall(x0, y0, z0, mouseX, mouseY, mouseWidth, mouseHeight);
}

void TrackBall::DragMove(int mouseX, int mouseY, int mouseWidth, int mouseHeight)
{
	PointOnTrackBall(x1, y1, z1, mouseX, mouseY, mouseWidth, mouseHeight);
	
	// rotation axis  = cross(p0, p1)
	f32 xa = y0*z1 - y1*z0;
	f32 ya = z0*x1 - z1*x0;
	f32 za = x0*y1 - x1*y0;
	
	// rotation angle = acos(dot(p0,p1))
	float angle = 2*acosf(min(max(x0*x1+y0*y1+z0*z1,-1),1)); // multiply angle by 2 for 360 degree range
	
	// rotate matrix around axis
	/* rotate.rotateAroundAxis(xa, ya, za, angle); */ 
	rotate = mat3_rotate(xa, ya, za, angle);
}

void TrackBall::DragEnd()
{  
	orientation = mat3_mul(orientation, rotate);
	rotate = mat3_identity();
}

mat3_t TrackBall::GetRotationMatrix()
{
	return mat3_mul(orientation, rotate);
}

#endif // __TRACKBALL_H__

