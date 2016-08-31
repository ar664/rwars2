#include "include\globals.h"
#include "shape.h"



int Box::n = 0;

Box::Box()
{

	//cout << 300/PPM << " : " << -300/PPM-n*5/PPM << endl;
    mBodyDef.position.Set(300.0f/PPM,-300.0f/PPM-n*5/PPM);
    n++;
    mBodyDef.type = b2_dynamicBody;
    mBodyShape.SetAsBox(5.0f/PPM,5.0f/PPM);
    mBodyFix.shape = &mBodyShape;
    mBodyFix.density = 0.1f;
    mBodyFix.friction = 0.1f;


}

Box::~Box()
{


}
Polygon::Polygon()
{


}
Polygon::~Polygon()
{


}

void Box::init(b2World* world,const Vec2D position,const Vec2D dimensions,bool fixedRot)
{
	
	mDimensions = dimensions;
	//Make Body
	mBodyDef.fixedRotation = fixedRot;
	mBodyDef.type = b2_dynamicBody;
	mBodyDef.angle = 0;
	mBodyDef.position.Set(position.x/PPM,position.y/PPM-n*5/PPM);
	//Create Fixture
	mBodyShape.SetAsBox((dimensions.x/2)/PPM,(dimensions.y/2)/PPM);
	

	mFixture.shape = &mBodyShape;
	mFixture.density = 1.0f;
	mFixture.friction = 0.3f;
	mBody = world->CreateBody(&mBodyDef);
	mBody->CreateFixture(&mFixture);

}

void Box::UpdateBoxShape(Vec2D dimensions)
{
	mDimensions = dimensions;
	mBody->DestroyFixture(mBody->GetFixtureList());

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox((dimensions.x/2)/PPM,(dimensions.y/2)/PPM);
	mBodyShape = polygonShape;


	b2FixtureDef Fixture;

	Fixture.shape = &mBodyShape;
	Fixture.density = 1.0f;
	Fixture.friction = 0.3f;
	mFixture = Fixture;
	
	mBody->CreateFixture(&mFixture);
	

}


void Polygon::init(b2World* world,const Vec2D position,const Vec2D dimensions,bool fixedRot)
{

	if(mPoints == nullptr)
	{
		printf("Points not set for polygon\n");
		return;
	}
	//Make Body
	mBodyDef.type = b2_dynamicBody;
	//mBodyDef.fixedRotation = true;
	mBodyDef.angle = 0;
	mBodyDef.position.Set(position.x/PPM,position.y/PPM-n*5/PPM);
	//Create Fixture
	b2Vec2 *verts = new b2Vec2[mPointCount];
	for(int i = 0;i < mPointCount;++i)
	{
		verts[i] = b2Vec2(mPoints[i].x/PPM,mPoints[i].y/PPM);
	}
	mBodyShape.Set(verts,mPointCount);

	mFixture.shape = &mBodyShape;
	mFixture.density = 1.0f;
	mFixture.friction = 0.3f;
	mBody = world->CreateBody(&mBodyDef);
	mBody->CreateFixture(&mFixture);
}

void pShape::SetPoints(sf::Vector2f* points,int count)
{
	mPointCount = count;
	mPoints = new sf::Vector2f[count];
	for(int i = 0;i < count;++i)
	{
		mPoints[i] = points[i];
	}

}

