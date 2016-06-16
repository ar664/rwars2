#ifndef __VECTORS_H_
#define __VECTORS_H_

typedef struct
{
	union {float x;	float w;};
	union {float y;	float h;};
}Vec2D;


typedef struct
{
	float x; 
	float y;
	float z;
}Vec3D;


typedef struct
{

	float x; 
	float y;
	float z;
	float w;
}Vec4D;
/*
typedef struct
{


}circle;

typedef struct
{


}triangle;
typedef struct
{


)rect;
enum Shapes
{
	Shape_Rect,
	Shape_Circle,
	Shape_Triangle
};

typedef struct
{
	int selection;
	union {
		circle c;
		rect r;
		triangle t;
}shape;
*/

/**
*@brief Methods to create Vecs
*/

Vec2D CreateVec2D(float x , float y);
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)


/**
 * @brief Macro to get the dot product from a vector.
 *
 * @param a Vect3D component of the dot product
 * @param b Vect3D component of the dot product
 *
 * @return the calculated dot product
 */
#define Vec2DDotProduct(a,b)      (a.x*b.x+a.y*b.y)

/**
 * @brief checks if vectors are exactly matches of each other
 *
 * @param a one vector to check
 * @param b other vector to check
 */
#define Vec2DEqual(a,b) ((a.x == b.x)&&(a.y == b.y))

/**
 * @brief Macro to subtract two vectors
 * varient ending in p takes a pointer to Vect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result Vect3D output
 * @param a Vect3D input
 * @param b Vect3D input
 */
#define Vec2DSub(dst,a,b)     (dst.x=a.x-b.x,dst.y=a.y-b.y)

/**
 * @brief Macro to add two vectors
 * varient ending in p takes a pointer to Vect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result Vect3D output
 * @param a Vect3D input
 * @param b Vect3D input
 */

#define Vec2DAdd(dst,a,b)   (dst.x = a.x+b.x,dst.y = a.y+b.y)

/**
 * @brief Macro to scale a vector by a scalar value
 * varient ending in p takes a pointer to Vect3D instead.
 * Varients ending with 2D only operate on the x an y components of vectors
 *
 * @param dst result Vect3D output
 * @param src Vect3D input
 * @Param factpr the scalar value to scale the vector by.
 */
#define Vec2DScale(dst,src,factor) (dst.x = src.x *factor,\
                                         dst.y = src.y *factor)
/**
 * @brief Macro that sets vector to zero.
 * @param a Vect[2D|3D|4D] input
 */

#define Vec2DClear(a)       (a.x=a.y=0)

/**
 * @brief Macro to set the components of the vector
 *
 * @param v Vect3D output
 * @param a Float x component
 * @param b Float y component
 * @param c Float z component (only in 3D & 4D version)
 * @param d Float w component (only in 4D version)
 */
#define Vec2DSet(v, a, b)  (v.x=(a), v.y=(b))

/**
 * @brief Macro to get the negative of a vector
 *
 * @param src Vect[2D|3D|4D] input
 * @param dst Vect[2D|3D|4D] negated output
 */
#define Vec2DNegate(dst,src)      (dst.x = -src.x,dst.y = -src.y)


/**
 * @brief reflect a vector about a normal.  Simulates "Bounce"
 * @param out the resultant vector
 * @param normal the normal vector for the surface that is providing the bounce
 * @param in the input vector that will bounce off of the normal
 */
void Vec2DReflect(Vec2D *out, Vec2D normal,Vec2D in);

/**
 * @brief normalizes the vector passed.  does nothing for a zero length vector.
 *
 * @param v pointer to the vector to be normalized.
 */
float Vec2DMagnitude(Vec2D V);

void Vec2DNormalize (Vec2D *V);
/**
 * @brief returns the magnitude squared, which is faster than getting the magnitude
 * which would involve taking the square root of a floating point number.
 * @param V the vector to get the magnitude for
 * @return the square of the magnitude of V
 */
float Vec2DMagnitudeSquared(Vec2D V);

/**
 * @brief scales the vector to the specified length without changing direction
 * No op is magnitude is 0 or V is NULL
 * @param V a pointer to the vector to scale
 * @param magnitude the new length for the vector
 */
void Vec2DSetMagnitude(Vec2D * V,float magnitude);
/**
 * @brief checks if the distance between the two points provided is less than size.
 * @param p1 one point for the distance check
 * @param p2 another point for the distance check
 * @param size the value to check against
 * @return True if the distance between P1 and P2 is less than size, false otherwise
 */
int DistanceBetweenLessThan2D(Vec2D p1,Vec2D p2,float size);


int DistanceBetweenGreaterThan2D(Vec2D p1,Vec2D p2,float size);

/**
 * @brief random macro taken from Id Software's Quake 2 Source.
 * This macro exposes the more random bits from the rand() function
 * @return a random float between 0 and 1.0
 */
#define random()  ((rand ()%1000)/(float)1000.0)

/**
 * @brief random macro taken from Id Software's Quake 2 Source.
 * This macro exposes the more random bits from the rand() function
 * @return a random float between -1.0 and 1.0
 */
#define crandom() (((float)((rand()%1000)/(float)1000.0) * 2.0) - 1.0)


//GetLength
//GetLength
//GetAngle
//AngleToVector
//Rotate

#endif
