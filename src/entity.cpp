#include <SFML\Graphics.hpp>
#include "physics.h"
#include "entity.h"

void Entity::Draw(sf::RenderTarget &target)
	{
		sf::IntRect rect(frameNum % rSprite->fpl * ANIMATION_FRAME_LENGTH,
			frameNum / rSprite->fpl * ANIMATION_FRAME_LENGTH,
			ANIMATION_FRAME_LENGTH,
			ANIMATION_FRAME_LENGTH);
		rSprite->sfmlSprite->setTextureRect(rect);
		target.draw(*rSprite->sfmlSprite,this->getTransform());
	}
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

void Entity::SetVelocity(Vec2D vec)
{
	velocity = vec;

}


