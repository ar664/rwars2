#ifndef _ENTITY_H
#define _ENTITY_H
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
public:
	int					cellIndex;
	int					frameNum;
	Vec2D				GetDimension();
	Vec2D				GetVelocity();
	Sprite				*rSprite;

	void Draw(sf::RenderTarget &target)const
	{
		target.draw(*rSprite->sfmlSprite,this->getTransform());
	}
	//Getters
	Cell*			GetCell();
	Grid*			GetGrid();
	//Setters
	void			SetCell(Cell* cell);
	void			SetPosition(Vec2D);
	void			SetDimensions(Vec2D);
	void			SetVelocity(Vec2D);
};


#endif

