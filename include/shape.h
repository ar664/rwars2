#ifndef SHAPE_H
#define SHAPE_H
#define MaxPolyVertexCount 64
#include <assert.h>
#include <utility>
#include "globals.h"
#include "Box2D/Box2D.h"

enum TypeOfBox
{
		HurtBox,
		HitBox,
		BaseBox,
		GroundSensor
};


class pShape
{
public:
	int					mTouchingGround;

	virtual	void		init(b2World* world,const Vec2D position,const Vec2D dimensions,bool fixedRot) = 0;
	b2Body*				GetBody(){return mBody;};
	b2FixtureDef		GetFixture(){return mFixture;};
	b2Fixture*			GetBaseFixture(){return mBaseFixture;};
	b2PolygonShape		GetBodyShape(){return mBodyShape;};
	Vec2D				GetDimensions(){return mDimensions;};
	b2BodyDef*			GetBodyDef(){return &mBodyDef;};
	sf::Vector2f*		GetPoints(){return mPoints;};
	int getN()			{return n;};


	void				SetPoints(sf::Vector2f* points,int count);
	void				SetFixedRotation(bool b){mBodyDef.fixedRotation = b;};
	void				SetBaseBody(b2Fixture* f);
protected:
	b2Body*				mBody; // This handles the physics
	b2Fixture*			mBaseFixture;
	b2FixtureDef		mFixture;
	Vec2D				mDimensions;
	b2BodyDef			mBodyDef;
	b2PolygonShape		mBodyShape;
	b2FixtureDef		mBodyFix;
	sf::Vector2f*		mPoints;
	int					mPointCount;
	static int n;
};
void FlipFixtures(b2Fixture* fixtures);


class Box : public pShape
{
public:
	Box();
	~Box();
	virtual void		init(b2World* world,const Vec2D position,const Vec2D dimensions,bool fixedRot);
	void				UpdateBoxShape(Vec2D dimensions);
protected:

	
};

class Polygon : public pShape
{
public:
	Polygon();
	~Polygon();
	virtual void		init(b2World* world,const Vec2D position,const Vec2D dimensions,bool fixedRot);

protected:

	
};

struct FixtureData
{
	int					mFrame;
	TypeOfBox			mType;
	sf::Color			mColor;
	b2Vec2				mOffset;
	b2Vec2				mDimensions;

};

#endif
