#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string.h>
#include <malloc.h>
#include "include\rapidjson\filereadstream.h"
#include "include\rapidjson\document.h"
#include "shape.h"
#include "globals.h"
#include "physics.h"
#include "entity.h"

Entity *gEntities = NULL;
int numEntities = 0;
bool	paused = false;		//temp variable until pause gamestate is a thing

using namespace rapidjson;


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
void Entity::SetSpriteArray(char* filePath)
{
	Document document;
	char fileName[155];
	char spriteFileName[155];
	int numFrames;

	strcpy(fileName,filePath);
	strcat(fileName,".json");
	
	FILE *file = fopen(fileName,"rb");

	char buffer[65536];
	FileReadStream frs(file,buffer,sizeof(buffer));
	//Parse the FileReadStream and close file
	document.ParseStream(frs);
	fclose(file);
	assert(document.IsObject());
	assert(document.HasMember("Assets"));
	

}

/**
*@brief Frees an Entity and its pointers
*/
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
		mBody->GetBody()->GetWorld()->DestroyBody(mBody->GetBody());
		free(mBody);
	}
	//Reset you own memory
	memset(this, 0, sizeof(Entity));
}
/**
*@brief Returns a free Entity from an array of Entities
*@return	Entity Pointer
*/
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
		gEntities[i].mSpriteArray = new Sprite* [25];
		gEntities[i].mMask = COMPONENT_ENTITY;
		return &gEntities[i];

	}

}
/**
*@brief Allocates Memory for Entity Array
*@returns	True if success, false if failed
*/
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
/**
*@brief	 Calls each Entities Free function
*/
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
				gEntities[i].Free();
		}

	}
}

void Entity::SetCell(Cell* cell)
{
	mCell = cell;
}
/**
*@brief Draws the players sprite at the given transform
*@param Window to draw on
*/
void Entity::Draw(sf::RenderTarget& target)
{
	int delta = 0;
	sf::Transformable t;
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

	//Check to see if flipped
	if(mIsFlipped)
	{
		mCurrentSprite->mSfSprite->setTextureRect(sf::IntRect(rect.left+ANIMATION_FRAME_LENGTH,
			rect.top,-ANIMATION_FRAME_LENGTH,ANIMATION_FRAME_LENGTH));
		
		t.setPosition(mBody->GetBody()->GetPosition().x*PPM-(ANIMATION_FRAME_LENGTH-mCurrentSprite->mFrameBB[mCurrentFrame].left)+mBody->GetDimensions().x/2,
			mBody->GetBody()->GetPosition().y*PPM-(mCurrentSprite->mFrameBB[mCurrentFrame].top)-mBody->GetDimensions().y/2);

	}	else
	{
		t.setPosition(mBody->GetBody()->GetPosition().x*PPM-(mCurrentSprite->mFrameBB[mCurrentFrame].left)-mBody->GetDimensions().x/2,
		mBody->GetBody()->GetPosition().y*PPM-(mCurrentSprite->mFrameBB[mCurrentFrame].top)-mBody->GetDimensions().y/2);
	
		mCurrentSprite->mSfSprite->setTextureRect(rect);
	}
			//target.draw(*mCurrentSprite->mSfSprite,t.getTransform());
	//Update Frames for animating
	if(!paused && mLastDrawTime)
	{
		delta = gClock.getElapsedTime().asMilliseconds() - mLastDrawTime;
	}
	mLastDrawTime = gClock.getElapsedTime().asMilliseconds();
	mNextFrameTime -= delta;
	
	if(mNextFrameTime <= 0)
	{
		mCurrentFrame+= mCurrentSprite->mAnimation.frameInc;

		if(mCurrentSprite->mAnimation.oscillate = true)
		{
			if(mCurrentSprite->mAnimation.frameInc > 0)
			{
				if(mCurrentFrame >= mCurrentSprite->mAnimation.maxFrames-1)
				{
					mCurrentSprite->mAnimation.frameInc = -mCurrentSprite->mAnimation.frameInc;
				}
			}
			else
			{
				if(mCurrentFrame <= 0)
				{
					mCurrentSprite->mAnimation.frameInc = -mCurrentSprite->mAnimation.frameInc;
				}

			}
		}else 
			{
				if(mCurrentFrame >= mCurrentSprite->mAnimation.maxFrames-1)
				{
					mCurrentFrame = 0;
				}
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

/**
*@brief This updates info for physics and whatnot
*/
void Entity::Update(float deltaTime)
{
		Vec2D dim = CreateVec2D(gEntities[10].mCurrentSprite->mFrameBB[gEntities[10].mCurrentFrame].width,
				gEntities[10].mCurrentSprite->mFrameBB[gEntities[10].mCurrentFrame].height);

		//static_cast<Box*>(gEntities[10].mBody)->UpdateBoxShape(dim);

		//PrePhysics
		//mBody->GetShape()->setRotation( mBody->GetBody()->GetAngle()*(180/3.14159265359)  );
		//mBody->GetShape()->setPosition( mBody->GetBody()->GetPosition().x*PPM, mBody->GetBody()->GetPosition().y*PPM);
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

/**
*@brief Used to set the current Animation 
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
/**
*@brief This is used to change the color of the body.
*		This is mostly used for debugging purposes
*@param The color
*/

/**
*@brief Takes in a shape that contains the body for physics
*@param The body/shape
*/
void Entity::SetBody(pShape* shape)
{
	mBody = shape;
	mBody->GetBody()->SetUserData(this);
}
/**
*@brief Checks to see if this entity contains this component
*@param The component
*@return true if it has, false if it doesnt
*/
bool Entity::HasComponent(sf::Int64 component)
{
	if (mMask & component == component)
		return true;
	else
		return false;

}
/**
*@brief Adds a component to the Entities component Mask
*@param The component ENUM
*/
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
