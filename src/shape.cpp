#include "globals.h"
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
	mBaseFixture = mBody->CreateFixture(&mFixture);

	FixtureData* fixData = new FixtureData;
	fixData->mColor = sf::Color(0,255,0,110);
	fixData->mType = BaseBox;
	mBody->GetFixtureList()->SetUserData(fixData);
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
void pShape::SetBaseBody(b2Fixture* f)
{
	mBaseFixture = f;
	//b2Vec2 baseOffset = static_cast<FixtureData*>(f->GetUserData())->mOffset;
	FixtureData *d = new FixtureData();
	d->mOffset = b2Vec2(0,5);
	d->mDimensions = b2Vec2(50,15);
	d->mColor= sf::Color(255,0,0,100);
	d->mType= GroundSensor;

	//Handle TouchGround
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox((d->mDimensions.x/2)/PPM,(d->mDimensions.y/2)/PPM,
		b2Vec2(d->mOffset.x/PPM,d->mOffset.y/PPM),0);


	b2FixtureDef Fixture;

	Fixture.shape = &polygonShape;
	Fixture.density = 1.0f;
	Fixture.friction = 0.3f;
	
	b2Fixture *fix = mBody->CreateFixture(&Fixture);
	fix->SetUserData(d);
	fix->SetSensor(true);
}

/**
*@brief This function Flips the fixtures of a b2Body on the x-axis
*@param A list of fixtures to flip
*/
void FlipFixtures(b2Fixture* fixtures)
{
	b2PolygonShape* shape;
	FixtureData* fData;
	for(b2Fixture* f = fixtures;f; f = f->GetNext())
	{
		fData = (FixtureData*)f->GetUserData();
		if(fData->mType == BaseBox)
			continue;
		int sizex,sizey;
		sizex = static_cast<FixtureData*>(f->GetUserData())->mDimensions.x;
		sizey = static_cast<FixtureData*>(f->GetUserData())->mDimensions.y;
		shape = (b2PolygonShape*)f->GetShape();
	
		b2Vec2 prev = b2Vec2(shape->m_centroid.x,shape->m_centroid.y);
		prev.x = -prev.x;
		shape->SetAsBox((sizex/2)/PPM,(sizey/2)/PPM,prev,0);
	}

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

