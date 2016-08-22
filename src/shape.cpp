#include "shape.h"



int Box::n = 0;

Box::Box()
{
	mShape = new sf::RectangleShape(sf::Vector2f(10.f,10.f));//(0,0,10,10,sf::Color(255,255,255));
    mShape->setOrigin(5,5);
	mShape->setFillColor(sf::Color(255,0,0,255));
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

void Box::init(b2World* world,const Vec2D position,const Vec2D dimensions)
{
	mShape = new sf::RectangleShape(sf::Vector2f(dimensions.x,dimensions.y));
	mShape->setOrigin(dimensions.x/2,dimensions.y/2);
	mShape->setFillColor(sf::Color(255,0,0,255));
	
	mDimensions = dimensions;
	//Make Body
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
	mShape->setPosition(mBody->GetPosition().x*PPM, mBody->GetPosition().y*PPM);
	sprite = LoadSprite("sprites/Crate.png");

}

void Polygon::init(b2World* world,const Vec2D position,const Vec2D dimensions)
{

	if(mPoints == nullptr)
	{
		printf("Points not set for polygon\n");
		return;
	}
	sf::ConvexShape *convex = new sf::ConvexShape();
	convex->setPointCount(mPointCount);
	for(int i = 0;i < mPointCount;++i)
	{
		convex->setPoint(i,mPoints[i]);
	}
	mShape = convex;
	mShape->setOrigin(0,0);
	mShape->setFillColor(sf::Color(0,255,0,255));
	//Make Body
	mBodyDef.type = b2_dynamicBody;
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
	mShape->setPosition(mBody->GetPosition().x*PPM, mBody->GetPosition().y*PPM);
	sprite = LoadSprite("sprites/Crate.png");
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