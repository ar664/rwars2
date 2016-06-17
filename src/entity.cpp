#include <SFML/Graphics.hpp>
#include <string.h>
#include <malloc.h>
#include "physics.h"
#include "entity.h"

Entity *gEntities = NULL;


void Entity::Load(char **Sprites)
{
	//Load Sprites
}

void Entity::Free()
{
	//Free Sprites

	//Reset you own memory
	memset(this, 0, sizeof(Entity));
}

bool EntitySystemInit()
{
	if(gEntities)
	{
		return false;
	}
	gEntities = (Entity*) malloc(sizeof(Entity)*MAX_ENTITIES);
	if(!gEntities)
	{
		return false;
	}
	memset(gEntities, 0, sizeof(Entity)*MAX_ENTITIES);
	atexit(EntitySystemShutdown);
	return true;
}

void EntitySystemShutdown()
{
	int i;
	if(!gEntities)
	{
		return;
	}
	for(i = 0; i < MAX_ENTITIES; i++)
	{
		if(gEntities[i].mInUse)
		{
			if(gEntities[i].rSprite)
			{
				gEntities[i].Free();
			}
		}

	}
}

void Entity::SetCell(Cell* cell)
{
	mCell = cell;
}
void Entity::SetDimensions(Vec2D vec)
{
	mDimension.x = vec.x;
	mDimension.y= vec.y;
}
Cell* Entity::GetCell()
{
	return mCell;
}
Vec2D Entity::GetDimension()
{
	return mDimension;
}
Vec2D Entity::GetVelocity()
{
	return mVelocity;
}
void Entity::SetCurrentAnimation(Animation* anim)
{
	curAnim = anim;

}

//Empty Functions Tbd (To be defined)
void			SetPosition(Vec2D)
{
	
}

void			SetDimensions(Vec2D)
{
	
}

void			SetVelocity(Vec2D)
{
	
}
