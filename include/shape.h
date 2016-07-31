#ifndef SHAPE_H
#define SHAPE_H
#define MaxPolyVertexCount 64
#include <assert.h>
#include <utility>
#include "matrix.h"
#include "globals.h"

struct RigidBody;

struct rShape
{
	enum Type
	{
	Circle,
	Poly,
	Count
	};
	rShape(){};
  virtual rShape *Clone( void ) const = 0;
  virtual void Initialize( void ) = 0;
  virtual void ComputeMass( float density ) = 0;
  virtual void SetOrientation( float radians ) = 0;
  virtual void Draw( sf::RenderTarget &gRenderWindow ) const = 0;
  virtual Type GetType( void ) const = 0;
  
  RigidBody *rbody;
  
  float radius;

  Matrix22 mat;	//Orientation Matrix from model to world

};

struct Polygon : public rShape
{
	int mVertexCount;
	Vec2D mVertices[MaxPolyVertexCount];
	Vec2D mNormals[MaxPolyVertexCount];


	void Initialize( void )
	{
		ComputeMass( 1.0f );
	}


	void ComputeMass( float density )
	{
	  // Calculate centroid and moment of interia
	  Vec2D c = CreateVec2D( 0.0f, 0.0f ); // centroid
	  float area = 0.0f;
	  float I = 0.0f;
	  const float k_inv3 = 1.0f / 3.0f;

	  for(int i1 = 0; i1 <mVertexCount; ++i1)
	  {
	    // Triangle vertices, third vertex implied as (0, 0)
	    Vec2D p1 = CreateVec2D( mVertices[i1].x,mVertices[i1].y );
	    int i2 = i1 + 1 < mVertexCount ? i1 + 1 : 0;
	    Vec2D p2 = CreateVec2D( mVertices[i2].x,mVertices[i2].y );

		float D = Cross(p1,p2);
	    float triangleArea = 0.5f * D;

	    area += triangleArea;

	    // Use area to weight the centroid average, not just vertex position
	    c += (p1 + p2) * triangleArea * k_inv3;

	    float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
	    float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
	    I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	  }

	  c *= 1.0f / area;

	  // Translate vertices to centroid (make the centroid (0, 0)
	  // for the polygon in model space)
	  // Not really necessary, but I like doing this anyway
	  for(int i = 0; i < mVertexCount; ++i)
	    mVertices[i] -= c;

	  rbody->mass = density * area;
	  rbody->mass /= 1000;
	  rbody->MomentOfInertia = I * density;
	  rbody->invMomentOfIntertia= rbody->MomentOfInertia ? 1.0f / 
		  rbody->MomentOfInertia : 0.0f;
	  rbody->invMomentOfIntertia=rbody->invMomentOfIntertia*40;
	}
	rShape *Clone( void ) const
	{
	 Polygon *poly = new Polygon( );
	 poly->mat = mat;
	 for(int i = 0; i < mVertexCount; ++i)
	    {
	   poly->mVertices[i] = mVertices[i];
	      poly->mNormals[i] = mNormals[i];
	    }
	    poly->mVertexCount = mVertexCount;
	    return poly;
  }


	void SetOrientation( float radians )
	{
		if(rbody != nullptr)
		{
		rbody->orientation = radians;
		}
		else
		{
			exit(0);
		}
		mat.Set(radians);
	}

	void Draw(sf::RenderTarget &gRenderWindow) const
	{
		sf::Color c(rbody->r,rbody->g,rbody->b,100);
		sf::ConvexShape convex;
		convex.setPointCount(mVertexCount);
		for(int i =0; i < mVertexCount;++i)
		{
			Vec2D v = rbody->position + mat * mVertices[i];
			convex.setPoint(i,sf::Vector2f(v.x,v.y));
		}
		convex.setFillColor(c);
		gRenderWindow.draw(convex);
	}
	Type GetType(void ) const
	{
		return Poly;
	}
	  // Half width and half height

   void Set( Vec2D *vertices, int count )
  {
    // No hulls with less than 3 vertices (ensure actual polygon)
    assert( count > 2 && count <= MaxPolyVertexCount );
	count = std::min( (int)count, MaxPolyVertexCount );

    // Find the right most point on the hull
    int rightMost = 0;
    float highestXCoord = vertices[0].x;
    for(int i = 1; i < count; ++i)
    {
      float x = vertices[i].x;
      if(x > highestXCoord)
      {
        highestXCoord = x;
        rightMost = i;
      }

      // If matching x then take farthest negative y
      else if(x == highestXCoord)
        if(vertices[i].y < vertices[rightMost].y)
          rightMost = i;
    }

    int hull[MaxPolyVertexCount];
    int outCount = 0;
    int indexHull = rightMost;

    for (;;)
    {
      hull[outCount] = indexHull;

      // Search for next index that wraps around the hull
      // by computing cross products to find the most counter-clockwise
      // vertex in the set, given the previos hull index
      int nextHullIndex = 0;
      for(int i = 1; i < (int)count; ++i)
      {
        // Skip if same coordinate as we need three unique
        // points in the set to perform a cross product
        if(nextHullIndex == indexHull)
        {
          nextHullIndex = i;
          continue;
        }

        // Cross every set of three unique vertices
        // Record each counter clockwise third vertex and add
        // to the output hull
        // See : http://www.oocities.org/pcgpe/math2d.html
        Vec2D e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
        Vec2D e2 = vertices[i] - vertices[hull[outCount]];
		float c = e1.CrossProduct(e2);
        if(c < 0.0f)
          nextHullIndex = i;

        // Cross product is zero then e vectors are on same line
        // therefor want to record vertex farthest along that line
        if(c == 0.0f && Vec2DLengthSquared(e2) > Vec2DLengthSquared(e1))
          nextHullIndex = i;
      }
      
      ++outCount;
      indexHull = nextHullIndex;

      // Conclude algorithm upon wrap-around
      if(nextHullIndex == rightMost)
      {
        mVertexCount = outCount;
        break;
      }
    }

    // Copy vertices into shape's vertices
    for(int i = 0; i < mVertexCount; ++i)
      mVertices[i] = vertices[hull[i]];

    // Compute face normals
    for(int i1 = 0; i1 < mVertexCount; ++i1)
    {
      int i2 = i1 + 1 < mVertexCount ? i1 + 1 : 0;
      Vec2D face = mVertices[i2] - mVertices[i1];
	  float v = Vec2DLengthSquared(face);
      // Ensure no zero-length edges, because that's bad
      assert( v > 0.0001f * 0.0001f );

      // Calculate normal with 2D cross product between vector and scalar
      mNormals[i1] = CreateVec2D( face.y, -face.x );
      Vec2DNormalize(&mNormals[i1]);
    }
  }

	void SetBox( float hw,float hh )
  {
    mVertexCount = 4;
    mVertices[0] = CreateVec2D( -hw, -hh );
    mVertices[1]= CreateVec2D(  hw, -hh );
    mVertices[2]= CreateVec2D(  hw,  hh );
    mVertices[3]= CreateVec2D( -hw,  hh );
    mNormals[0]= CreateVec2D(  0.0f,  -1.0f );
    mNormals[1]= CreateVec2D(  1.0f,   0.0f );
    mNormals[2]= CreateVec2D(  0.0f,   1.0f );
    mNormals[3]= CreateVec2D( -1.0f,   0.0f );
  }
	Vec2D GetSupport( const Vec2D& dir )
  {
    float bestProjection = -FLT_MAX;
    Vec2D bestVertex;

    for(int i = 0; i < mVertexCount; ++i)
    {
      Vec2D v = mVertices[i];
      float projection = v * dir ;

      if(projection > bestProjection)
      {
        bestVertex = v;
        bestProjection = projection;
      }
    }

    return bestVertex;
  }
};


#endif
