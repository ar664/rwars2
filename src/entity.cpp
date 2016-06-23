#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string.h>
#include <malloc.h>
#include "globals.h"
#include "physics.h"
#include "entity.h"

Entity *gEntities = NULL;
bool	paused = false;		//temp variable until pause gamestate is a thing

void Entity::LoadSprites(char **SpriteFiles)
{
	int i;
	mSpriteArray = (Sprite**) malloc(sizeof(Sprite*)*(MAX_ANIMATIONS+1));
	memset(mSpriteArray, 0, sizeof(Sprite*)*(MAX_ANIMATIONS+1));			//NULL terminating before hand.
	if(!SpriteFiles)
	{
		return;
	}
	for(i = 0; SpriteFiles[i]; i++)
	{
		mSpriteArray[i] = LoadSprite(SpriteFiles[i]);
	}
	mNumSprites = MemoryCount(mSpriteArray, sizeof(Sprite*));				//Counting and saving value
	mCurrentSprite = mSpriteArray[0];
	mCurrentFrame = 0;
	mNextFrameTime = mCurrentSprite->mAnimation.mpf;
	mLastDrawTime = 0;
	this->setPosition(0, 0);
	this->setScale(1, 1);
}

void Entity::LoadSounds(char** SoundFiles)
{
	int i;
	void *temp_sound;
	mSounds = (sf::SoundBuffer**) malloc(sizeof(sf::SoundBuffer*)*(MAX_ANIMATIONS+1));
	memset(mSounds, 0, sizeof(sf::Sound*)*(MAX_ANIMATIONS+1));

	if(!SoundFiles)
	{
		return;
	}
	for(i = 0; SoundFiles[i]; i++)
	{
		mSounds[i] = new sf::SoundBuffer();
		mSounds[i]->loadFromFile(SoundFiles[i]);
	}
	mSounds[i] = NULL;
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
Entity* CreateEntity()
{
	int i;
	for( i = 0;i < MAX_ENTITIES;i++)
	{
		if(gEntities[i].mInUse)
			continue;
		gEntities[i].mInUse = 1;
		return &gEntities[i];

	}

}

Entity* EntityGetFree()
{
	int i;
	if(!gEntities)
	{
		return NULL;
	}

	for(i = 0; i < MAX_ENTITIES; i++)
	{
		if(!gEntities[i].mInUse)
		{
			return &gEntities[i];
		}
	}
	return NULL;
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

void EntitySystemStep()
{
	int i, time;
	if(!gEntities)
	{
		return;
	}
	time = gClock.getElapsedTime().asMilliseconds();
	for(i = 0; i < MAX_ENTITIES; i++)
	{
		if(gEntities[i].mInUse)
		{
			if(gEntities[i].mNextThinkTime < time)
			{
				gEntities[i].Think();
			}
		}
	}
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
	mBody.velocity = vec;
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

void Entity::Think()
{
	//Empty Think Should be replaced on inheritance
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
	return mBody.velocity;
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
