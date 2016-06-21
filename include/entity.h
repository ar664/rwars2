#ifndef _ENTITY_H
#define _ENTITY_H
#include <unordered_map>
#include "vectors.h"
#include "sprite.h"
struct Cell;
class Grid;

typedef class Entity: public sf::Transformable
{
private:
	int					speed;
	Vec2D				velocity;
	Vec2D				dimension;
	Cell*				mCell;
	Animation*			curAnim;	
public:
	int					cellIndex;
	int					frameNum;

	float				mass;
	float				restitution;

	Sprite				*rSprite;
	std::unordered_map<char*,Animation*>		animations;	

	void Draw(sf::RenderTarget &target)const
	{
		sf::IntRect rect(frameNum % rSprite->fpl * ANIMATION_FRAME_LENGTH,
			frameNum / rSprite->fpl * ANIMATION_FRAME_LENGTH,
			ANIMATION_FRAME_LENGTH,
			ANIMATION_FRAME_LENGTH);
		rSprite->sfmlSprite->setTextureRect(rect);
		target.draw(*rSprite->sfmlSprite,this->getTransform());
	}
	//Getters
	Cell*			GetCell();
	Grid*			GetGrid();
	Vec2D			GetDimension();
	Vec2D			GetVelocity();
	//Setters
	void			SetVelocity(Vec2D vec);
	void			SetCurrentAnimation(Animation* anim);
	void			SetCell(Cell* cell);
	void			SetPosition(Vec2D vec);
	void			SetDimensions(Vec2D vec);
};


#endif

