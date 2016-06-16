#include <SFML\Graphics.hpp>
#include "physics.h"
#include "entity.h"


void Entity::SetCell(Cell* cell)
{
	mCell = cell;
}
void Entity::SetDimensions(Vec2D vec)
{
	dimension.x = vec.x;
	dimension.y= vec.y;
}
Cell* Entity::GetCell()
{
	return mCell;
}
Vec2D Entity::GetDimension()
{
	return dimension;
}
Vec2D Entity::GetVelocity()
{
	return velocity;
}
void Entity::SetCurrentAnimation(Animation* anim)
{
	curAnim = anim;

}


