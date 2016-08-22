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
	memset(mSounds, 0, sizeof(sf::SoundBuffer*)*(MAX_ANIMATIONS+1));

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
		//for(i = 0; i < mNumSprites; i++)
	//	{
		//	mSpriteArray[i]->FreeSprite();
	//	}
		free(mSpriteArray);
	}
	if(mBody)
	{
		free(mBody);
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
		gEntities[i].mID = i;
		numEntities +=1;
		
		gEntities[i].mMask = COMPONENT_ENTITY;
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
/*
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
	if(mBody != nullptr)
		mBody->velocity = vec;
}
*/
void Entity::Draw(sf::RenderTarget& target)
{
	int delta = 0;
	sf::Transformable t;
	//t.setPosition(mBody->position.x,mBody->position.y);
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
	target.draw(*mCurrentSprite->mSfSprite,t.getTransform());

	//Update Frames for animating
	if(!paused && mLastDrawTime)
	{
		delta = gClock.getElapsedTime().asMilliseconds() - mLastDrawTime;
	}
	mLastDrawTime = gClock.getElapsedTime().asMilliseconds();
	mNextFrameTime -= delta;
	if(mNextFrameTime <= 0)
	{
		if( mCurrentFrame < mCurrentSprite->mAnimation.maxFrames -1)
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
void Entity::Update(float deltaTime)
{
		//PrePhysics
		mBody->GetShape()->setRotation( mBody->GetBody()->GetAngle()*(180/3.14159265359)  );
		mBody->GetShape()->setPosition( mBody->GetBody()->GetPosition().x*PPM, mBody->GetBody()->GetPosition().y*PPM);
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

/*
Vec2D Entity::GetDimension()
{
	return mDimension;
}

Vec2D Entity::GetVelocity()
{
	return mBody->velocity;
}
*/
void Entity::SetCurrentAnimation(int anim)
{
	if(!mSpriteArray)
	{
		printf("Entity does not have sprites, attempted to set animation to NULL Sprite Array \n");
		return;
	}
	if(anim >= mNumSprites || anim < 0)
	{
		printf("Entity assigned unset or unknown animation. Sprites Loaded: %d. Animation# = %d \n", mNumSprites, anim);
		return;
	}
	if(mCurrentSprite == mSpriteArray[anim])
	{
		return;
	}
	mCurrentSprite = mSpriteArray[anim];
	mCurrentFrame = 0;
}
/*

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
RigidBody::RigidBody(rShape* s)
{
	s->rbody = this;
	shape = s;
	isAwake = 1;
	SetVelocity(CreateVec2D(0,0));
	angularVelocity = 0;
	torque = 0;
	force = CreateVec2D( 0, 0 );
	restitution =0;
	staticFriction = .03;
	dynamicFriction = .03;
	zConstraint = 0;
	//shape->Initialize( );

}
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
void RigidBody::SetAngVelocity(float i)
{
	angularVelocity = i;
}

float RigidBody::GetAngVelocity()
{
	return angularVelocity;
}
void RigidBody::SetColor(float red,float green , float blue)
{
	r = red;
	g = green;
	b = blue;

}*/

bool Entity::HasComponent(sf::Int64 component)
{
	if (mMask & component == component)
		return true;
	else
		return false;

}
void Entity::AddComponent(sf::Int64 component)
{
	if(gScene != nullptr)
	{
		if(mID > MAX_ENTITIES)
		{
			std::printf("Exceed EntityLimit..EXIT");
			exit(0);
		}
		else if (mMask & component == component)
		{
			std::printf("Entity already has this component");
		}
		else
		{
			mMask = mMask | component;
		}
	switch(component)
	{
		case COMPONENT_PLAYER:
			gScene->Players[mID].mID = mID;
	}

	}
}
