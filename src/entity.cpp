#include <SFML/Graphics.hpp>
#include <string.h>
#include <malloc.h>
#include "globals.h"
#include "physics.h"
#include "entity.h"

Entity *gEntities = NULL;
bool	paused = false;		//temp variable until pause gamestate is a thing

void Entity::Load(char **SpriteFiles)
{
	//Load Sprites
	int i;
	mSpriteArray = (Sprite**) malloc(sizeof(Sprite*)*(MAX_ANIMATIONS+1));
	memset(mSpriteArray, 0, sizeof(Sprite*)*(MAX_ANIMATIONS+1));			//NULL terminating before hand.
	for(i = 0; SpriteFiles[i]; i++)
	{
		mSpriteArray[i] = LoadSprite(SpriteFiles[i]);
	}
	mNumSprites = MemoryCount(mSpriteArray, sizeof(Sprite*));				//Counting and saving value
	mCurrentSprite = mSpriteArray[0];
	mNextFrameTime = mCurrentSprite->mAnimation.mpf;
	mLastDrawTime = 0;
}

void Entity::Free()
{
	int i;

	//Free Sprites
	if(mSpriteArray)
	{
		for(i = 0; i < mNumSprites; i++)
		{
			mSpriteArray[i]->FreeSprite();
		}
		free(mSpriteArray);
	}
	
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
			if(gEntities[i].mSpriteArray)
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

void Entity::SetPosition(Vec2D vec)
{
	this->setPosition(vec.x, vec.y);
}

void Entity::SetDimensions(Vec2D vec)
{
	mDimension.x = vec.x;
	mDimension.y= vec.y;
}

void Entity::SetVelocity(Vec2D vec)
{
	mVelocity = vec;
}

void Entity::Draw(sf::RenderTarget& target)
{
	int delta = 0;
	if(!mSpriteArray)
	{
		return;
	}
	if(!mCurrentSprite)
	{
		mCurrentSprite = mSpriteArray[0];
	}
	sf::IntRect rect(mCurrentFrame % mCurrentSprite->mFramesPerLine * ANIMATION_FRAME_LENGTH,
		mCurrentFrame / mCurrentSprite->mFramesPerLine * ANIMATION_FRAME_LENGTH,
		ANIMATION_FRAME_LENGTH,
		ANIMATION_FRAME_LENGTH);
	mCurrentSprite->mSfSprite->setTextureRect(rect);
	target.draw(*mCurrentSprite->mSfSprite,this->getTransform());

	//Update Frames for animating
	if(!paused && mLastDrawTime)
	{
		delta = gClock.getElapsedTime().asMilliseconds() - mLastDrawTime;
	}
	mLastDrawTime = gClock.getElapsedTime().asMilliseconds();
	mNextFrameTime -= delta;
	if(mNextFrameTime <= 0)
	{
		if( mCurrentFrame < mCurrentSprite->mAnimation.maxFrames)
		{
			mCurrentFrame++;
		} else
		{
			mCurrentFrame = 0;
		}
		mNextFrameTime = mCurrentSprite->mAnimation.mpf;
	}
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

void Entity::SetCurrentAnimation(int anim)
{
	if(!mSpriteArray)
	{
		return;
	}
	if(anim >= mNumSprites)
	{
		return;
	}
	if(mCurrentSprite == mSpriteArray[anim])
	{
		return;
	}
	mCurrentSprite = mSpriteArray[anim];
	mCurrentFrame = 0;
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
