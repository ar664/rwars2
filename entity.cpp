#include <SFML\Graphics.hpp>
#include "physics.h"
#include "entity.h"


void Entity::SetCell(Cell* cell)
{
	mCell = cell;
}

Cell* Entity::GetCell()
{
	return mCell;
}

Vec2D Entity::GetPosition()
{
	return position;

}
Vec2D Entity::GetDimension()
{
	return dimension;
}
Vec2D Entity::GetVelocity()
{
	return velocity;
}



