#ifndef _ENTITY_H
#define _ENTITY_H
#include <unordered_map>
#include "vectors.h"
#include "sprite.h"
struct Cell;
class Grid;

#define MAX_ENTITIES	1000
#define MAX_ANIMATIONS	20

/**
 * The Entity class which is used for inheritance. Basic properties.
 * 
 *	Graphics:
 *		mSpriteArray	-	Each sprite in this array represents a different animation.
 *		mCurrentSprite	-	The current sprite that the entity is drawing.
 *		mCurrentFrame	-	The current frame the entity is at.
 *		mNumSprites		-	The number of sprites in the spritearray.
 *
 */


typedef class Entity: public sf::Transformable
{
private:
	int					mSpeed;
	Vec2D				mVelocity;
	Vec2D				mDimension;
	Cell*				mCell;
	
public:
	int					mInUse;
	int					mCellIndex;
	
	int					mCurrentFrame;
	int					mNumSprites;
	Sprite**			mSpriteArray;
	Sprite*				mCurrentSprite;	

	//Functions
	void Draw(sf::RenderTarget &target);

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
	void	Load(char **Sprites);
	void	Free();
};

extern Entity *gEntities;

bool EntitySystemInit();
void EntitySystemShutdown();


#endif

