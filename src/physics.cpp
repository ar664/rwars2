#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "globals.h"
#include "vectors.h"
#include "physics.h"

ForceRegistry forceRegistry;
Grid* gGrid;
/**
* 
*
*/
float CalculateSeperatingVelocity(Manifold *m)
{
	Vec2D rVelocity = m->A->GetVelocity();
	if(m->B->mass != 0)
	{
		rVelocity -= m->B->GetVelocity();
	}
	return rVelocity * CreateVec2D(-m->normal.x,-m->normal.y);
}
/**
* 
*
*/
void Entity::ResolveContact(Manifold *m)
{
	ResolveVelocity(m);
	//ResolveFriction(m);
	ResolveInterpenetration(m);
}
/**
* 
*
*/
void Entity::ResolveVelocity(Manifold *m)
{
	/*
	float inverseMass = 1/mBody->mass;
	float sVelocity = CalculateSeperatingVelocity(m);
	if(sVelocity > 0)
	{
		return;
	}
	float newSVelocity = -sVelocity* mBody->restitution;

	//To keep objects in resting contact, this block corrects the increase of velocity that would arise
	//from them settling into one another over the course of one frame
	Vec2D accCausedVelocity = m->A->GetAcceleration();
	if(m->B->mass != 0)
		accCausedVelocity += m->B->GetAcceleration();
	float accCausedSepVelocity = accCausedVelocity * CreateVec2D(-m->normal.x,-m->normal.y) * deltaTime;
	if(accCausedSepVelocity < 0)
		newSVelocity += mBody->restitution * accCausedSepVelocity;
	if(newSVelocity <0)
		newSVelocity = 0;


	float deltaVelocity = newSVelocity -sVelocity;
	float totalInverseMass = inverseMass;
	if(m->B->mass != 0)
		totalInverseMass += 1/m->B->mass;
	if(totalInverseMass <=0)
		return;
	float impulse = deltaVelocity / totalInverseMass;

	Vec2D impulseVec = CreateVec2D(-m->normal.x,-m->normal.y) * impulse;
	mBody->SetVelocity(mBody->GetVelocity() + impulseVec*(inverseMass));
	if(m->B->mass != 0)
		m->B->SetVelocity(m->B->GetVelocity() + impulseVec * -(1/m->B->mass));
	*/

	float inverseMass = 1/mBody->mass;
	for(int i = 0; i < m->contact_count; ++i)
	 {
	   // Calculate radii from COM to contact
	   Vec2D ra = m->contacts[i] - m->A->position;
	   Vec2D rb = m->contacts[i] - m->B->position;

	   // Relative velocity
		 Vec2D rVelocity = m->B->velocity + Cross( m->B->angularVelocity, rb ) -
              m->A->velocity - Cross( m->A->angularVelocity, ra );

		float sVelocity = rVelocity * m->normal;		
	    
		
		if(sVelocity > 0)
	     return;
		float newSVelocity = -sVelocity* mBody->restitution;

	   float raCrossN = Cross( ra, m->normal);
	   float rbCrossN = Cross( rb, m->normal);
		
		float deltaVelocity = newSVelocity -sVelocity;
		float totalInverseMass = (raCrossN*raCrossN) * m->A->invMomentOfIntertia;
		if(m->B->mass != 0)
		{
			totalInverseMass += (rbCrossN * rbCrossN) * m->B->invMomentOfIntertia;
			totalInverseMass += 1/m->B->mass;

		}
		totalInverseMass += inverseMass;

		float impulse = deltaVelocity / totalInverseMass;
		//impulse /= (float)m->contact_count;
		
		Vec2D impulseVec = m->normal * impulse;
		
		if(impulseVec.y < 0)
		{
			std::cout << "AHH" << std::endl;
		}

		mBody->SetVelocity(mBody->GetVelocity() + impulseVec*(-inverseMass));
		mBody->SetAngVelocity(mBody->GetAngVelocity() + mBody->invMomentOfIntertia 
			* Cross(ra,impulseVec*-1));
		if(m->B->mass != 0)
		{
			m->B->SetVelocity(m->B->GetVelocity() + impulseVec * (1/m->B->mass));
			m->B->SetAngVelocity(m->B->GetAngVelocity() + m->B->invMomentOfIntertia 
			*Cross(rb,impulseVec));
		}
	}
}

/**
* 
*
*/
void ResolveFriction(Manifold* m)
{
	Vec2D rv,tangent,frictionImpulse;
	float jt,mu,dynamicFriction,invMass1,invMass2,velAlongNormal;

	if(m->A->mass == 0)					//Check for infinite Mass
		invMass1 = 0;
	else
		invMass1 = (1/m->A->mass);
	if(m->B->mass == 0)
		invMass2 = 0;
	else
		invMass2 = (1/m->B->mass);

	for(int i = 0; i < m->contact_count; ++i)
	 {
	   // Calculate radii from COM to contact
	   Vec2D ra = m->contacts[i] - m->A->position;
	   Vec2D rb = m->contacts[i] - m->B->position;
		rv = m->B->velocity + Cross( m->B->angularVelocity, rb ) -
              m->A->velocity - Cross( m->A->angularVelocity, ra );
		Vec2D t = rv - (m->normal *(rv*m->normal));
		Vec2DNormalize(&t);

		//Solve for tangent Vector
		velAlongNormal = Vec2DDotProduct(rv,m->normal);
		tangent = CreateVec2D(rv.x - velAlongNormal * m->normal.x,rv.y - velAlongNormal * m->normal.y);

		
		
		jt = -Vec2DDotProduct( rv, tangent);
		jt = jt / (invMass1 + invMass2);
		//jt /= (float)m->contact_count;
		//Use pythag to solve for mu (Doing Coulumbs law)
		mu = sqrt(pow(m->A->staticFriction,(float)2) + pow(m->B->staticFriction,(float)2));

		float e = std::min(m->A->restitution,m->B->restitution);			//Restitution
		float j = -(1+e) * jt;
		j = (j/(invMass1+invMass2));

		if(jt == 0.0f)
			return;
		if(abs(jt) < j * mu)
			//Assuming the object is at rest, this code will execute
		{
			frictionImpulse = CreateVec2D(tangent.x * jt,tangent.y * jt);
		}
		else
		{
			dynamicFriction =sqrt(pow(m->A->dynamicFriction,(float)2) + pow(m->B->dynamicFriction,(float)2));
			frictionImpulse = CreateVec2D(-j*tangent.x*dynamicFriction,-j*tangent.y*dynamicFriction);
		}
		m->A->SetVelocity(m->A->GetVelocity() + frictionImpulse*(-1/m->A->mass));
		m->A->SetAngVelocity(m->A->GetAngVelocity() + m->A->invMomentOfIntertia 
			* Cross(ra,frictionImpulse*-1));
		if(m->B->mass != 0)
		{
			m->B->SetVelocity(m->B->GetVelocity() + frictionImpulse* (1/m->B->mass));
			m->B->SetAngVelocity(m->B->GetAngVelocity() + m->B->invMomentOfIntertia 
			*Cross(rb,frictionImpulse));
		}

	}
	/*
	//Find resolution Vector
	Vec2DSub(rv,m->B->GetVelocity(),m->A->GetVelocity());
	//Solve for tangent Vector
	velAlongNormal = Vec2DDotProduct(rv,m->normal);
	tangent = CreateVec2D(rv.x - velAlongNormal * m->normal.x,rv.y - velAlongNormal * m->normal.y);

	Vec2DNormalize(&tangent);

	// Solve for magnitude to apply along the friction vector
	jt = -Vec2DDotProduct( rv, tangent);
	jt = jt / (invMass1 + invMass2);
	//Use pythag to solve for mu (Doing Coulumbs law)
	mu = sqrt(pow(m->A->staticFriction,(float)2) + pow(m->B->staticFriction,(float)2));

	float e = std::min(m->A->restitution,m->B->restitution);			//Restitution
	float j = -(1+e) * jt;
	j = (j/(invMass1+invMass2));

	if(abs(jt) < j * mu)
		//Assuming the object is at rest, this code will execute
	{
		frictionImpulse = CreateVec2D(tangent.x * jt,tangent.y * jt);
	}
	else
	{
		dynamicFriction =sqrt(pow(m->A->dynamicFriction,(float)2) + pow(m->B->dynamicFriction,(float)2));
		frictionImpulse = CreateVec2D(-j*tangent.x*dynamicFriction,-j*tangent.y*dynamicFriction);
	}
	frictionImpulse.x = frictionImpulse.x;
	frictionImpulse.y = frictionImpulse.y;

	m->A->SetVelocity(
		CreateVec2D(m->A->GetVelocity().x - invMass1 *frictionImpulse.x,m->A->GetVelocity().y - invMass1 *frictionImpulse.y));
	m->B->SetVelocity(
		CreateVec2D(m->B->GetVelocity().x + invMass2 *frictionImpulse.x,m->B->GetVelocity().y + invMass2 *frictionImpulse.y));
		*/
}
/**
* 
*
*/
void Entity::ResolveInterpenetration(Manifold *m)
{
	/*
	float inverseMass = 1/mBody->mass;
	Vec2D movementB;
	if(m->penetration == 0)
		return;
	float totalInverseMass = inverseMass;
	if(m->B->mass != 0)
		totalInverseMass += 1/m->B->mass;
	if(totalInverseMass == 0)
		return;
	Vec2D sep = CreateVec2D(-m->normal.x,-m->normal.y) *(m->penetration/totalInverseMass);
	Vec2D movementA = sep * inverseMass;
	if(movementA.y > 0)
	{
		std::cout << "AHH" << std::endl; 
	}
	if(m->B->mass != 0)
		movementB = sep *(-1/m->B->mass);
	m->A->SetPosition(CreateVec2D(m->A->GetPosition().x + movementA.x,m->A->GetPosition().y + movementA.y));
	if(m->B->mass != 0)
		m->B->SetPosition(CreateVec2D(m->B->GetPosition().x + movementB.x,m->B->GetPosition().y + movementB.y));
	
	*/
	if(m->penetration > 0)
		{
			const float k_slop = 0.01f;
			const float percent= .8f;
			float max = std::max(m->penetration - k_slop,0.0f);
			float totalInverseMass = 1/m->A->mass;
			if(m->B->mass != 0)
				totalInverseMass += 1/m->B->mass;
			Vec2D correction = CreateVec2D(max/totalInverseMass *percent*m->normal.x,
				max /totalInverseMass*percent*m->normal.y) ;
			m->A->SetPosition(CreateVec2D(m->A->GetPosition().x - (correction.x*1/m->A->mass),
				m->A->GetPosition().y - (correction.y*1/m->A->mass)));
			if(m->B->mass != 0)
				m->B->SetPosition(CreateVec2D(m->B->GetPosition().x - (correction.x*1/m->B->mass),
				m->B->GetPosition().y - (correction.y*1/m->B->mass)));		}
}

float FindAxisLeastPenetration(int *faceIndex,Polygon *a, Polygon *b)
{
  float bestDistance = -FLT_MAX;
  int bestIndex;

  for(int i = 0; i < a->mVertexCount; ++i)
  {
    // Retrieve a face normal from A
    Vec2D n = a->mNormals[i];
    Vec2D nw = a->mat * n;

    // Transform face normal into B's model space
    Matrix22 buT = b->mat.Transpose( );
    n = buT * nw;

    // Retrieve support point from B along -n
	Vec2D Negn;
	Vec2DNegate(Negn,n);
    Vec2D s = b->GetSupport( Negn );

    // Retrieve vertex on face from A, transform into
    // B's model space
    Vec2D v = a->mVertices[i];
    v = a->mat * v + a->rbody->position;
    v -= b->rbody->position;
    v = buT * v;

    // Compute penetration distance (in B's model space)
    float d = n * ( s - v );

    // Store greatest distance
    if(d > bestDistance)
    {
      bestDistance = d;
      bestIndex = i;
    }
  }

  *faceIndex = bestIndex;
  return bestDistance;
}
void FindIncidentFace( Vec2D *v, Polygon *RefPoly, Polygon *IncPoly, int referenceIndex )
{
  Vec2D referenceNormal = RefPoly->mNormals[referenceIndex];

  // Calculate normal in incident's frame of reference
  referenceNormal = RefPoly->mat * referenceNormal; // To world space
  referenceNormal = IncPoly->mat.Transpose( ) * referenceNormal; // To incident's model space

  // Find most anti-normal face on incident polygon
  int incidentFace = 0;
  float minDot = FLT_MAX;
  for(int i = 0; i < IncPoly->mVertexCount; ++i)
  {
    float dot = referenceNormal * IncPoly->mNormals[i] ;
    if(dot < minDot)
    {
      minDot = dot;
      incidentFace = i;
    }
  }

  // Assign face vertices for incidentFace
  v[0] = IncPoly->mat * IncPoly->mVertices[incidentFace] + IncPoly->rbody->position;
  incidentFace = incidentFace + 1 >= (int)IncPoly->mVertexCount ? 0 : incidentFace + 1;
  v[1] = IncPoly->mat * IncPoly->mVertices[incidentFace] + IncPoly->rbody->position;
}
int Clip( Vec2D n, float c, Vec2D *face )
{
  int sp = 0;
  Vec2D out[2] = {
    face[0],
    face[1]
  };

  // Retrieve distances from each endpoint to the line
  // d = ax + by - c
  float d1 =  n * face[0] - c;
  float d2 =  n * face[1] - c;

  // If negative (behind plane) clip
  if(d1 <= 0.0f) out[sp++] = face[0];
  if(d2 <= 0.0f) out[sp++] = face[1];
  
  // If the points are on different sides of the plane
  if(d1 * d2 < 0.0f) // less than to ignore -0.0f
  {
    // Push interesection point
    float alpha = d1 / (d1 - d2);
    out[sp] = face[0] + ((face[1] - face[0]) * alpha );
    ++sp;
  }

  // Assign our new converted values
  face[0] = out[0];
  face[1] = out[1];

  assert( sp != 3 );

  return sp;
}
inline bool BiasGreaterThan( float a, float b )
{
  const float k_biasRelative = 0.95f;
  const float k_biasAbsolute = 0.01f;
  return a >= b * k_biasRelative + a * k_biasAbsolute;
}

Manifold* PolygonVsPolygon(RigidBody *a,RigidBody* b)
{
	
	Polygon *A = reinterpret_cast<Polygon *>(a->shape);
	Polygon *B = reinterpret_cast<Polygon *>(b->shape);
	Manifold* m = new Manifold;
	m->A = a;
	m->B = b;

	m->contact_count = 0;
	int faceA;
	float penetrationA = FindAxisLeastPenetration(&faceA,A,B);
	if (penetrationA >=  0.0f)
		return nullptr;

	int faceB;
	float penetrationB = FindAxisLeastPenetration(&faceB,B,A);
	if (penetrationB >=  0.0f)
		return nullptr;

	int referenceIndex;
	bool flip;

	Polygon *RefPoly; // Reference
	Polygon *IncPoly; // Incident
	  
	// Determine which shape contains reference face
	 if(BiasGreaterThan( penetrationA, penetrationB ))
	 {
	   RefPoly = A;
	   IncPoly = B;
	   referenceIndex = faceA;
	   flip = false;
	 }

	 else
	 {
	   RefPoly = B;
	   IncPoly = A;
	   referenceIndex = faceB;
	   flip = true;
	 }
	 
	 //World space incident face
	 Vec2D incidentFace[2];
	 FindIncidentFace( incidentFace, RefPoly, IncPoly, referenceIndex );

	  // Setup reference face vertices
	 Vec2D v1 = RefPoly->mVertices[referenceIndex];
	 referenceIndex = referenceIndex + 1 == RefPoly->mVertexCount ? 0 : referenceIndex + 1;
	 Vec2D v2 = RefPoly->mVertices[referenceIndex];

	 // Transform vertices to world space
	 v1 = RefPoly->mat * v1 + RefPoly->rbody->position;
	 v2 = RefPoly->mat * v2 + RefPoly->rbody->position;

	 // Calculate reference face side normal in world space
	 Vec2D sidePlaneNormal = (v2 - v1);
	 Vec2DNormalize(&sidePlaneNormal);

	 // Orthogonalize
	 Vec2D refFaceNormal = CreateVec2D( sidePlaneNormal.y, -sidePlaneNormal.x );

	 // ax + by = c
	 // c is distance from origin
	float refC = refFaceNormal * v1 ;
	float negSide = -(sidePlaneNormal * v1);
	float posSide =   sidePlaneNormal * v2 ;

	 // Clip incident face to reference face side planes
	Vec2D sidePlaneNormalNeg; 
	Vec2DNegate(sidePlaneNormalNeg,sidePlaneNormal);
	 if(Clip( sidePlaneNormalNeg, negSide, incidentFace ) < 2)
	   return nullptr; // Due to floating point error, possible to not have required points

	 if(Clip(  sidePlaneNormal, posSide, incidentFace ) < 2)
	   return nullptr; // Due to floating point error, possible to not have required points

	 // Flip
	 if(flip)
		 Vec2DNegate(m->normal,refFaceNormal);
	 else
		 m->normal = refFaceNormal;

	 // Keep points behind reference face
	 int cp = 0; // clipped points behind reference face
	 float separation =refFaceNormal * incidentFace[0] - refC;
	 if(separation <= 0.0f)
	 {
	   m->contacts[cp] = incidentFace[0];
	   m->penetration = -separation;
	   ++cp;
	 }
	 else
	   m->penetration = 0;

	 separation = refFaceNormal * incidentFace[1]  - refC;
	 if(separation <= 0.0f)
	 {
	   m->contacts[cp] = incidentFace[1];

	   m->penetration += -separation;
	   ++cp;

	   // Average penetration
	   m->penetration /= (float)cp;
	 }

	 m->contact_count = cp;
	
	 return m;
	 
}

/**
* 
*
*/

Manifold* AABB(Entity *ent1, Entity *ent2)
{
		/*
	Checks if the boxes intersect by comparing the boxes (x and widths) and (y and heights)
	*/
	if((ent1->getPosition().x + ent1->GetDimension().x) > ent2->getPosition().x 
		&& (ent2->getPosition().x + ent2->GetDimension().x) > ent1->getPosition().x
		&& (ent1->getPosition().y + ent1->GetDimension().y) > ent2->getPosition().y 
		&& (ent2->getPosition().y + ent2->GetDimension().y) > ent1->getPosition().y)
	{
		Vec2D center1 = CreateVec2D(ent1->getPosition().x + (ent1->GetDimension().x/2),
			ent1->getPosition().y + (ent1->GetDimension().y/2));
		Vec2D center2 = CreateVec2D(ent2->getPosition().x + (ent2->GetDimension().x/2),
			ent2->getPosition().y + (ent2->GetDimension().y/2));
		Manifold* m = new Manifold;
		
		
		m->normal.x = 0;
		m->normal.y = 0;
		m->A = ent1->mBody;
		m->B = ent2->mBody;
		float a_extent = (ent1->getPosition().x + ent1->GetDimension().x - ent1->getPosition().x) / 2 ;
		float b_extent = (ent2->getPosition().x + ent2->GetDimension().x - ent2->getPosition().x) / 2;
  
		// Calculate overlap on x axis
		float x_overlap = a_extent + b_extent - abs( ent2->getPosition().x - ent1->getPosition().x );
  
	// SAT test on x axis
	  if(x_overlap > 0)
	  {
	    // Calculate half extents along x axis for each object
		a_extent = (ent1->getPosition().y + ent1->GetDimension().y - ent1->getPosition().y) / 2 ;
		b_extent = (ent2->getPosition().y + ent2->GetDimension().y - ent2->getPosition().y) / 2;
	  
	    // Calculate overlap on y axis
	    float y_overlap = a_extent + b_extent - abs( ent2->getPosition().y - ent1->getPosition().y );
	  
	    // SAT test on y axis
	    if(y_overlap > 0)
	    {
	      // Find out which axis is axis of least penetration
	      if(x_overlap < y_overlap)
	      {
	        // Point towards B knowing that n points from A to B
	        if(ent2->getPosition().x - ent1->getPosition().x < 0)
	          m->normal = CreateVec2D( -1, 0 );
	        else
	          m->normal = CreateVec2D( 0, 0 );
	        m->penetration = x_overlap;
			return m;
	      }
	      else
	      {
	        // Point toward B knowing that n points from A to B
	        if(ent2->getPosition().y - ent1->getPosition().y< 0)
	          m->normal = CreateVec2D( 0, -1 );
	        else
	          m->normal = CreateVec2D( 0, 1 );
	        m->penetration = y_overlap;
			return m;
	      }
	    }
	  }
	}
	return nullptr;
}

/**
* 
*
*/
int CollisionResponseAABBvsAABB(Entity* ent1,Entity *ent2,Manifold *m)
{
	/*
		Vec2D rv;
		Vec2DSub(rv,ent2->GetVelocity(),ent1->GetVelocity());
		float penetration = 0;
		float velAlongNormal = Vec2DDotProduct(rv,m->normal);
		float invMass1;
		float invMass2;
		if(ent1->mBody->mass == 0)					//Check for infinite Mass
			invMass1 = 0;
		else
			invMass1 = (1/ent1->mBody->mass);
		if(ent2->mBody->mass == 0)
			invMass2 = 0;
		else
			invMass2 = (1/ent2->mBody->mass);
		if(velAlongNormal > 0)
		{
			return 0;
		}
		float e = ent1->mBody->restitution;			//Restitution
		float j = (1+e) * -velAlongNormal;


		j = (j/(invMass1+invMass2));

		Vec2D impulse = CreateVec2D(m->normal.x * j,m->normal.y * j);
		Vec2D result1,result2;
		
		result1.x = (invMass1  * impulse.x);
		result1.y = (invMass1  * impulse.y);
		
		e = ent2->mBody->restitution;			//Restitution
		j = (1+e) * -velAlongNormal;
		j = (j/(invMass1+invMass2));
		impulse = CreateVec2D(m->normal.x * j,m->normal.y * j);

		result2.x = (invMass2  * impulse.x);
		result2.y = (invMass2  * impulse.y);
 		

		ent1->SetVelocity(CreateVec2D(ent1->GetVelocity().x - result1.x,
			ent1->GetVelocity().y - result1.y));
		if(invMass2 != 0)
			ent2->SetVelocity(CreateVec2D(ent2->GetVelocity().x + result2.x,
				ent2->GetVelocity().y + result2.y));
		//Solve for friction
		FrictionResponseAABBvsAABB(ent1,ent2,m);

		//Solve for interpenetration
		if(m->penetration.x != 0)
			penetration = m->penetration.x;
		if(m->penetration.y != 0)
			penetration = m->penetration.y;
		if(penetration > 0)
		{
			const float k_slop = 0.01f;
			const float percent= .8f;
			Vec2D correction = CreateVec2D((std::max(m->penetration.x - k_slop,0.0f)/(invMass1 + invMass2))*percent*m->normal.x,
				(std::max(m->penetration.y - k_slop,0.0f)/(invMass1 + invMass2))*percent*m->normal.y) ;
			ent1->setPosition(ent1->getPosition().x - (correction.x*invMass1),ent1->getPosition().y - (correction.y*invMass1));
			if(invMass2 != 0)
				ent2->setPosition(ent2->getPosition().x + (correction.x*invMass2),ent2->getPosition().y + (correction.y*invMass2));
		}
	*/
	return 1;
	
	}
/**
 * @brief Constructer for Grid
 * @author	Jason
*/

Grid::Grid(int width, int height, int cellSize):
	m_width(width),
	m_height(height),
	m_cellSize(cellSize){
		m_numXCells = ceil((float) m_width / cellSize);
		m_numYCells = ceil((float) m_height / cellSize);

		//Allocate cells
		m_cells.resize(m_numYCells * m_numXCells);
}
/**
* 
*
*/
Grid::~Grid(void)
{
}

/**
 * @brief returns a pointer to a cell
 * @author	Jason
*/

Cell* Grid::getCell(int x, int y)
{
	if(x < 0) 
	{
		printf("Cell does not exist\n");
		x = 0;
	}
	if(x >= m_numXCells)
		x = m_numXCells - 1;
	if(y < 0) 
	{
		printf("Cell does not exist\n");
		y = 0;
	}
	if(y >= m_numYCells) 
		y = m_numYCells - 1;
	return &m_cells[y * m_numXCells + x];
}
/**
 *** @brief returns a pointer to a cell based on entities location
 *** @author	Jason
*/
Cell* Grid::getCell(Vec2D ent)
{
	int cellX = (int)(ent.x/m_cellSize);
	int cellY = (int)(ent.y/m_cellSize);

	return getCell(cellX,cellY);
}

/**
 * @brief Add entity to a cell
 * @author	Jason
*/

void Grid::addEntity(Entity* ent)
{
	Cell* cell = getCell(CreateVec2D(ent->getPosition().x,ent->getPosition().y));
	cell->entities.push_back(ent);
	ent->SetCell(cell);
	ent->mCellIndex = cell->entities.size() - 1;
	printf("Added to new cell,CellSize:%d",cell->entities.size());
}

/**
 * @brief Add entity to a specified cell
 * @author	Jason
 */

void Grid::addEntity(Entity* ent,Cell* cell)
{
	cell->entities.push_back(ent);
	ent->SetCell(cell);
	ent->mCellIndex = cell->entities.size() - 1;
	printf("Added to new cell,CellSize:%d",cell->entities.size());

}
/**
 * @brief Remove entity to a cell
 * @author	Jason
*/
void Grid::removeEntityFromCell(Entity* ent)
{
	std::vector<Entity*> & ents = ent->GetCell()->entities;
	
	//Vector swap, Need to check for 0 or else get error because you cant pop_back from an empty vector
	ents[ent->mCellIndex] = ents.back();					//Setting one vectors index to the last element in the other vector
	ents.pop_back();									//Deletes last element
	//update vector index
	if(ent->mCellIndex < ents.size())
	{
		ents[ent->mCellIndex]->mCellIndex = ent->mCellIndex;
	}
	
	//Set index of ent to 1
	ent->mCellIndex = -1;
	ent->SetCell(nullptr);
	printf("Removed from old cell\n");
}
/**
 * @brief Returns m_cells
 * @author	Jason
*/
std::vector<Cell> Grid::getM_Cells()
{
	return m_cells;
}

int Grid::getM_NumXCells()
{
	return m_numXCells;
}
int Grid::getM_NumYCells()
{
	
	return m_numYCells;
}

int Grid::getM_CellSize()
{
	return m_cellSize;
}

int Grid::getM_Width()
{
	return m_width;
}

int Grid::getM_Height()
{
	return m_height;
}

void RigidBody::AddForce(Vec2D amount)
{
	Vec2DAdd(force,force,amount);
}

/**
* 
*
*/
void UpdateCollision()
{
	//Loop Through Cells in Grid
	for(int i = 0; i < gGrid->getM_Cells().size();i++)
	{
		//printf("Mcells Size%d \n",gGrid->getM_Cells().size());
		int x = i % gGrid->getM_NumXCells();
		int y = i / gGrid->getM_NumXCells();

		Cell cell = gGrid->getM_Cells()[i];
		//Loop through Entities in Cell
		for(int j = 0;j < cell.entities.size();j++)
		{
			Entity* ent = cell.entities[j];
			//Do collision checks in the current cell than update
			if(cell.entities[j]->mBody != nullptr)
			{
				if(ent->mBody->isAwake == 1)
				{
					CheckCollision(ent,cell.entities,j+1);
					
					//Update Collision with neighbor cells
					if(x > 0)
					{	//Left
						CheckCollision(ent,gGrid->getCell(x-1,y)->entities,0);
						if(y > 0)
						{	
							//TopLeft
							CheckCollision(ent,gGrid->getCell(x-1,y-1)->entities,0);
						}
						if(y < gGrid->getM_NumYCells() -1)
						{
							//Bottom left
							CheckCollision(ent,gGrid->getCell(x-1,y+1)->entities,0);
						}
					}
					if(y > 0){
						//Top
						CheckCollision(ent,gGrid->getCell(x,y-1)->entities,0);
					}
				}
		
			}
		}
	}
	
}
/**
* 
*
*/
void CheckCollision(Entity* ent, std::vector<Entity*>& ents, int startIndex)
{
	Manifold *m = nullptr;

	if(ent->mBody->mass > 0)
	{
		for (int i = startIndex; i < ents.size();i++)
		{
			if(ents[i]->mBody != nullptr)
			{
				if(ents[i] != ent && ents[i]->mBody->isAwake == 1){		//Make sure player isnt checking collision with self and ent doesnt have infinite mass
					m = PolygonVsPolygon(ent->mBody,ents[i]->mBody);
					if(m != nullptr)
					{
						ent->ResolveContact(m);
					}
				}
			}
		}
	}
	delete(m);
}
