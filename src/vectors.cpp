#include <SFML/Graphics.hpp>
#include <math.h>
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

/**
*@brief Returns the magnitude of specified 2D Vector
*/
float Vec2DMagnitude (Vec2D V)
{
  return std::sqrt(std::pow(V.x,2) + std::pow(V.y ,2));
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
  V->x *= 1/M;
  V->y *= 1/M;
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

