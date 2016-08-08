#ifndef _ENTITY_H
#define _ENTITY_H

#include <SFML/Audio.hpp>

#include "vectors.h"
#include "sprite.h"
#include "include\components.h"

struct rShape;
struct Cell;
struct Manifold;

class Grid;

#define MAX_ENTITIES	1000
#define MAX_ANIMATIONS	15

/**
 * The Entity class which is used for inheritance. Basic properties.
 * 
 *	Basic:
 *		mThinkRate		-	The frequency this entity updates itself.
 *		mNextThinkTime	-	The next time this entity should update.
 *	Graphics:
 *		mSpriteArray	-	Each sprite in this array represents a different animation.
 *		mCurrentSprite	-	The current sprite that the entity is drawing.
 *		mCurrentFrame	-	The current frame the entity is at.
 *		mNumSprites		-	The number of sprites in the spritearray.
 *		mLastDrawTime	-	Keeps track of the last time the frame was drawn, used to calculate delta time for frame updates.
 *		mNextFrameTime	-	Frames are updated based on whether next frame time is <= 0. (Milliseconds)
 *
 *	Physics:
 *		sf::Transformable-	This is a use of the sfml library where they provide us with some useful functions for position
 *		mSpeed			-	The speed that the entity is going.
 *		mDimension		-	The bounding box for the character.
 *
 *	Audio:
 *		mSounds			-	This is the sounds from the sfml library along with Sprites they sideload the work we have to do to get them to work.
 *		*mCallbacks		-	*This may be neccessary for some entities that want to make a specific action happen at the end of certain sounds.
 */

/**
* RigidBody struct used for physics calculations
*	float		mass			-	Entities with infinite mass have a mass of 0
*	Vec2D		velocity		-	Every physics update, this will be used in conjunction with other forces to move the body
*	Vec2D		force			-	This is the amount of force applied to the object by external forces
*	float		staticFriction
*	float		dynamicFriction
*	*Note that rigidBodys have two frictions. One is for when the object is at rest and the other is used when the 
*		object is in motion
*	float		restitution		-	Used for Bounce(higher the value,the higher the bounce).Values over 20 start to get wierd bounce behavior
*	float		density			-	Not being used at the moment but may switch to it inorder to determine mass
*/

struct RigidBody
{
	//Constructor
	RigidBody(rShape* s);

	rShape* shape;
	
	float		mass;

	Vec2D		position;
	Vec2D		velocity;

	Vec2D		force;			
	Vec2D		acceleration;

	// Material Structure 
	float		staticFriction;
	float		dynamicFriction;
	float		restitution;
	float		density;		/*<-- Use density* volume to determine the currect mass of an object */
	// Angular Components
	float		orientation;	//radians
	float		angularVelocity;
	float		torque;
	float		MomentOfInertia;
	float		invMomentOfIntertia;

	int			r,g,b;

	bool		zConstraint;
	bool		isAwake;

	void	AddForce(Vec2D amount);
	void	SetVelocity(Vec2D vec);
	void	SetAcceleration(Vec2D vec);
	void	SetPosition(Vec2D vec);
	void	SetOrientation	(float	radians);
	void	SetStatic();
	void	SetAngVelocity(float i);
	void	SetColor(float red,float green , float blue);
	
	Vec2D	GetPosition();
	Vec2D	GetVelocity();
	Vec2D	GetAcceleration();
	float	GetAngVelocity();
	
	
};

enum Shape{
	RECT,
	CIRCLE
};

class Entity: public sf::Transformable 
{
private:
	sf::Uint32			mLastDrawTime;
	int					mNextFrameTime;
	int					mThinkRate;
	int					mSpeed;
	Vec2D				mDimension;
	Cell*				mCell;
	void				ResolveVelocity(Manifold *m);


public:
	sf::Int64			mMask;
	int					mID;
	int					mInUse;
	int					mCellIndex;
	
	int					mNextThinkTime;
	int					mCurrentFrame;
	int					mNumSprites;
	Sprite**			mSpriteArray;
	Sprite*				mCurrentSprite;	
	RigidBody*			mBody;
	sf::SoundBuffer**	mSounds;



	//Functions
/**
 * @breif Draws the entity and updates the graphics timings
 *
 * @param RenderTarget,	Used for telling where to render the entity to
 *
 * @note Graphics timings don't get updated when paused.
 */
	void Draw(sf::RenderTarget &target);
	void PhysicsUpdate(float deltaTime);

/**
 * @breif Empty think function, to be overridden by children who inherit it.
 *
 */
	virtual void Think();


	//Getters
	Cell*			GetCell();
	Grid*			GetGrid();
	Vec2D			GetDimension();
	Vec2D			GetVelocity();
	Vec2D			GetPosition();
	//Setters
	void			SetCurrentAnimation(int anim);
	void			SetCell(Cell* cell);
	void			SetPosition(Vec2D vec);
	void			SetDimensions(Vec2D vec);
	void			SetVelocity(Vec2D vec);

	//Constructors
	void	LoadSprites(char **SpriteFiles);
	void	LoadSounds(char **SoundFiles);
	void	Free();
	
	//Methods for physics
	void	ResolveContact(Manifold *m);
	void    ResolveInterpenetration(Manifold *m);

	//Component Methods
	void	AddComponent(sf::Int64 component);

};



extern Entity *gEntities;

Entity *EntityGetFree();
Entity* CreateEntity();
bool EntitySystemInit();
void EntitySystemStep();
void EntitySystemShutdown();

#endif

