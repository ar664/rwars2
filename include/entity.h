#ifndef _ENTITY_H
#define _ENTITY_H

#include <SFML/Audio.hpp>
#include "vectors.h"
#include "sprite.h"
struct RigidBody;
struct Cell;
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
 *		mVelocity		-	The normalized 2D vector of their direction.
 *		mDimension		-	The bounding box for the character.
 *
 *	Audio:
 *		mSounds			-	This is the sounds from the sfml library along with Sprites they sideload the work we have to do to get them to work.
 *		*mCallbacks		-	*This may be neccessary for some entities that want to make a specific action happen at the end of certain sounds.
 */

struct RigidBody
{
	// We are assuming for now that all Rigidbody shapes are rectangles
	float		mass;
	Vec2D		velocity;
	Vec2D		force;			/*<-- Used to find acceleration, also is applied to the object every physics update until it eventually dies out */
	// Material Structure 
	float		staticFriction;
	float		dynamicFriction;
	float		restitution;
	float		density;		/*<-- Use density* volume to determine the currect mass of an object */


	void AddForce(float amount);
};

typedef class Entity: public sf::Transformable 
{
private:
	sf::Uint32			mLastDrawTime;
	int					mNextFrameTime;
	int					mThinkRate;
	int					mSpeed;
	Vec2D				mDimension;
	Cell*				mCell;
	
public:

	int					mInUse;
	int					mCellIndex;
	
	int					mNextThinkTime;
	int					mCurrentFrame;
	int					mNumSprites;
	Sprite**			mSpriteArray;

	Sprite*				mCurrentSprite;	
	RigidBody			mBody;
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
};

extern Entity *gEntities;

Entity *EntityGetFree();

bool EntitySystemInit();
void EntitySystemStep();
void EntitySystemShutdown();
Entity* CreateEntity();

#endif

