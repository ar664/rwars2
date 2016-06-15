#ifndef _ENTITY_H
#define _ENTITY_H
#include "vectors.h"
#include "sprite.h"
struct Cell;
class Grid;

typedef class Entity :public sf::Transformable , public sf::Drawable
{
private:
	int				speed;
	Vec2D			position;
	Vec2D			velocity;
	Vec2D			dimension;
	Cell*			mCell;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const
	{
	    states.transform *= getTransform();
		target.draw(rSprite->sfmlSprite, states);
	}
public:
	int				cellIndex;
	Vec2D			GetPosition();
	Vec2D			GetDimension();
	Vec2D			GetVelocity();


	Cell*			GetCell();
	Grid*			GetGrid();
	
	Sprite			*rSprite;
	//Setters
	void			SetCell(Cell* cell);
	void			SetPosition(Vec2D);
	void			SetDimensions(Vec2D);
	void			SetVelocity(Vec2D);
};


#endif

