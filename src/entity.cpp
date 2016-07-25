#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string.h>
#include <malloc.h>
#include "shape.h"
#include "globals.h"
#include "physics.h"
#include "entity.h"

Entity *gEntities = NULL;
int numEntities = 0;
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
		gEntities[i].mBody = new RigidBody();
		numEntities +=1;
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
	if(mBody != nullptr)
	{
		mBody->SetPosition(vec);
	}
}

Vec2D Entity::GetPosition()
{
	if(mBody != nullptr)
	{
		return mBody->GetPosition();
	}
}
void Entity::SetDimensions(Vec2D vec)
{
	mDimension.x = vec.x;
	mDimension.y= vec.y;
}

void Entity::SetVelocity(Vec2D vec)
{
	mBody->velocity = vec;
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
		if( mCurrentFrame < mCurrentSprite->mAnimation.maxFrames-
			(mCurrentSprite->mSfSprite->getTexture()->getSize().x)/ANIMATION_FRAME_HEIGHT)
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
//initially this is going to be a void virtual because different entities will different physics behaviors
void Entity::PhysicsUpdate(float deltaTime)
{
		//PrePhysics
	if(mBody->mass != 0.0f)
	{
		//mBody->acceleration.y = mBody->mass != 0 ?Gravity_constant*deltaTime*deltaTime*.5:
			//mBody->acceleration.y*deltaTime*deltaTime*.5;
		//mBody->acceleration.x *= deltaTime;

		//Force of Gravity
		//mBody->force.y += Gravity_constant*mBody->mass;
		//Figure Out acceleration due to force
		mBody->acceleration.AddScaledVector(mBody->force,1/mBody->mass);
		mBody->acceleration = mBody->acceleration*deltaTime*deltaTime*.5;

		//Rotation
		//mBody->angularVelocity += mBody->torque * (mBody->invMomentOfIntertia) * deltaTime;
		//mBody->orientation += mBody->angularVelocity * deltaTime;

		/**
		*Calculating one floating point to the power of another is slow when it comes to multiple ents,
		*So if you need more speed, simply remove the power and times velocity by damping or calculate
		*the damping once and used it for all ents.
		*Hope it doesnt crash lol;
		*/
		SetVelocity(mBody->velocity*pow(Damping_constant,deltaTime)  + mBody->acceleration);
		

		move(mBody->velocity.x*deltaTime,mBody->velocity.y*deltaTime);	
		mBody->position.x = getPosition().x;
		mBody->position.y = getPosition().y;
		//Clear the forces
		mBody->force.x = mBody->force.y = 0;

	}
		//Grid Detection via Cells
		Cell *newCell = gGrid->getCell(CreateVec2D(getPosition().x,getPosition().y));
		if(newCell != GetCell())
			{
				if(GetCell() == NULL)
			{
				gGrid->addEntity(this,newCell);
			}
			else if(newCell != GetCell())
			{
				gGrid->removeEntityFromCell(this);
				gGrid->addEntity(this,newCell);
			}
		}
}


Vec2D Entity::GetDimension()
{
	return mDimension;
}

Vec2D Entity::GetVelocity()
{
	return mBody->velocity;
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


Vec2D RigidBody::GetVelocity()
{
	return velocity;
}
void RigidBody::SetVelocity(Vec2D vec)
{
	velocity = vec;
}
Vec2D RigidBody::GetAcceleration()
{
	return acceleration;
}
/*
RigidBody::RigidBody(rShape* s)
{
	shape = s;
	s->rbody = this;
}*/
void RigidBody::SetAcceleration(Vec2D vec)
{
	acceleration = vec;
}
void RigidBody::SetOrientation(float radians)
{
	shape->SetOrientation(radians);
	orientation = radians;
}

void RigidBody::SetPosition(Vec2D vec)
{
	position = vec;
}
Vec2D RigidBody::GetPosition()
{
	return position;
}