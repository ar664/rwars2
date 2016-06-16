#ifndef _ENTITY_H
#define _ENTITY_H
#include <unordered_map>
#include "vectors.h"
#include "sprite.h"
struct Cell;
class Grid;

#define MAX_ENTITIES 1000

typedef class Entity: public sf::Transformable
{
private:
	int					mSpeed;
	Vec2D				mVelocity;
	Vec2D				mDimension;
	Cell*				mCell;
	Animation*			curAnim;	
public:
	int					mInUse;
	int					mCellIndex;
	int					mFrameNum;
	Vec2D				GetDimension();
	Vec2D				GetVelocity();
	Sprite				*rSprite;
	std::unordered_map<char*,Animation*>		animations;	

	void Draw(sf::RenderTarget &target)const
	{
		sf::IntRect rect(mFrameNum % rSprite->fpl * ANIMATION_FRAME_LENGTH,
			mFrameNum / rSprite->fpl * ANIMATION_FRAME_LENGTH,
			ANIMATION_FRAME_LENGTH,
			ANIMATION_FRAME_LENGTH);
		rSprite->sfmlSprite->setTextureRect(rect);
		target.draw(*rSprite->sfmlSprite,this->getTransform());
	}
	//Getters
	Cell*			GetCell();
	Grid*			GetGrid();
	//Setters
	void			SetCurrentAnimation(Animation* anim);
	void			SetCell(Cell* cell);
	void			SetPosition(Vec2D);
	void			SetDimensions(Vec2D);
	void			SetVelocity(Vec2D);

	//Constructors
	void	Load(char **Sprites);
	void	Free();
};

extern Entity *gEntities;

bool EntitySystemInit();
void EntitySystemShutdown();


#endif

