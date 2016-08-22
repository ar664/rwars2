#include <SFML/Graphics.hpp>
#include "vectors.h"


/**
*@brief Creates a 2D Vector
*/
Vec2D CreateVec2D(float x, float y)
{
  Vec2D vec;
  Vec2DSet(vec,x, y);
  return vec;
}
/**
*@brief Creates a 3D Vector
*/
Vec3D CreateVec3D(float x, float y,float z)
{
  Vec3D vec;
  Vec3DSet(vec,x, y,z);
  return vec;
}
/**
*@brief Creates a 4D Vector
*/
Vec4D CreateVec4D(float x, float y,float z,float r)
{
  Vec4D vec;
  Vec4DSet(vec,x, y,z,r);
  return vec;
}
/**
*@brief Returns the magnitude of specified 2D Vector
*/
float Vec2DMagnitude (Vec2D V)
{
  return std::sqrt(V.x*V.x + V.y *V.y);
}
/**
*@brief Returns the magnitude of specified 4D Vector
*/
float Vec4DMagnitude (Vec4D V)
{
  return std::sqrt(std::pow(V.x,2) + std::pow(V.y ,2)
	  + std::pow(V.z ,2)+ std::pow(V.r ,2));
}

/**
*@brief Returns the magnitude squared of specified 2D Vector
*/
float Vec2DMagnitude_squared(Vec2D V)
{
  return (V.x * V.x + V.y * V.y);
}


/**
*@brief Sets the magnitude squared of specified 2D Vector
*/
void Vec2DSetMagnitude(Vec2D * V,float magnitude)
{
  if (!V)return;
  Vec2DNormalize(V);
  V->x *= magnitude;
  V->y *= magnitude;
}

/**
*@brief Normalizes the specified 2D Vector
*/
void Vec2DNormalize (Vec2D *V)
{

  float M;
  if (!V)return;
  M = Vec2DMagnitude (*V);
  if (M == 0.0f)
  {
    return;
  }
  float invM = 1.0f/M;
  V->x *= invM;
  V->y *= invM;
  
}

float Vec2DLengthSquared(Vec2D vec)
{
	return vec.x*vec.x+vec.y*vec.y;
}
/**
*@brief Allocates memory for a new 2D Vector and returns it;
*/
Vec2D *Vec2D_New()
{
  Vec2D *vec = NULL;
  vec = (Vec2D *)malloc(sizeof(Vec2D));
  if (vec == NULL)
  {
    return NULL;
  }
  Vec2DClear((*vec));
  return vec;
}

int DistanceBetweenLessThan2D(Vec2D p1,Vec2D p2,float size)
{
	float i; 
	i = sqrt(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2));
	if(sqrt(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2)) < size)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
int DistanceBetweenGreaterThan2D(Vec2D p1,Vec2D p2,float size)
{
		float i; 
	i = sqrt(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2));
	if(sqrt(pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2)) > size)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
void Vec2DReflect(Vec2D *out, Vec2D normal,Vec2D in)
{
  float f;
  f = Vec2DDotProduct(in,normal);
  out->x = in.x - (2 * normal.x * f);
  out->y = in.y - (2 * normal.y * f);
}

void Vec2D::AddScaledVector(Vec2D &vec,float t)
{
	x+= vec.x * t;
	y+= vec.y * t;
}

float Vec2D::CrossProduct(Vec2D &b)
{
	return x*b.y - y * b.x;
}

Vec2D RotatePoint(Vec2D vec, float angle)
{
	Vec2D newV;
	newV= CreateVec2D( vec.x * cos(angle) - vec.y * sin(angle),
		vec.x * sin(angle) + vec.y * cos(angle));
	return newV;
}