#ifndef _ENTITY_H
#define _ENTITY_H
#include <unordered_map>
#include "vectors.h"
#include "sprite.h"
struct Cell;
class Grid;

#define MAX_ENTITIES 1000

/**
 * The Entity class which is used for inheritance. Basic properties.
 * 
 *	Graphics:
 *		mSprite	-	Used to keep info of current sprite
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
	Vec2D				GetDimension();
	Vec2D				GetVelocity();
	int					mFrameNum;
	Sprite**			mSpriteArray;
	Sprite*				mCurrentSprite;	

	//Functions
	void Draw(sf::RenderTarget &target);

	//Getters
	Cell*			GetCell();
	Grid*			GetGrid();

	//Setters
	void			SetCurrentAnimation(Animation* anim);
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

