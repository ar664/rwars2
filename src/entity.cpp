#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string.h>
#include <malloc.h>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
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
	if(mSpriteArray != NULL)
	{
		for(i = 0; i < MAX_SPRITE_ARRAY; i++)
		{
			if(mSpriteArray[i] != NULL)
				mSpriteArray[i]->FreeSprite();
		}
		delete(mSpriteArray);
		mSpriteArray =nullptr;
	}
	if(mBody != nullptr)
	{
		delete(mBody);
		mBody = nullptr;
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
		gEntities[i].mSpriteArray = new Sprite* [MAX_SPRITE_ARRAY];
		gEntities[i].mSpriteMachine = nullptr;
		memset(gEntities[i].mSpriteArray,0,sizeof(Sprite*)*MAX_SPRITE_ARRAY);

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
	b2Vec2 p = b2Vec2((mBody->GetBody()->GetPosition().x + 
		static_cast<b2PolygonShape*>(mBody->GetBaseFixture()->GetShape())->m_centroid.x)*PPM,
		(mBody->GetBody()->GetPosition().y + 
		static_cast<b2PolygonShape*>(mBody->GetBaseFixture()->GetShape())->m_centroid.y)*PPM);

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
	if(mIsFlipped)
	{
		rect = sf::IntRect(rect.left+ANIMATION_FRAME_LENGTH,rect.top,-rect.width,rect.height);
		t.setPosition(p.x-(ANIMATION_FRAME_LENGTH-mCurrentSprite->mSpriteAxis.x-20),
			p.y-mCurrentSprite->mSpriteAxis.y);
	}
	else
	{
	t.setPosition(p.x-mCurrentSprite->mSpriteAxis.x-20,
		p.y-mCurrentSprite->mSpriteAxis.y);
	}
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
		if(mCurrentFrame == mCurrentSprite->mAnimation.heldFrame-1)
		{
			mCurrentSprite->mAnimation.holdFrame = 1;
		}
		if(mCurrentSprite->mAnimation.holdFrame == 1)
			return;
		mCurrentFrame+= mCurrentSprite->mAnimation.frameInc;

		if(mCurrentSprite->mAnimation.oscillate == 1)
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
				if(mCurrentFrame >= mCurrentSprite->mAnimation.maxFrames)
				{
					mCurrentFrame = 0;
				}
			}
		mNextFrameTime = mCurrentSprite->mAnimation.mpf;
	}
}
void Entity::SetSprite(int n)
{
	mCurrentFrame = 0;
	mCurrentSprite->mAnimation.holdFrame = 0;
	mCurrentSprite->mAnimation.frameInc = std::abs(mCurrentSprite->mAnimation.frameInc);
	mCurrentSprite = mSpriteArray[n];

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

	//if(mMask & COMPONENT_PLAYER == COMPONENT_PLAYER)
	//{
	//	if(mBody != nullptr)
	//		//gScene->Players[mID].mMoveData->mTouchingGround = mBody->mTouchingGround;
	//}
	/*
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
		*/
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
*@brief Takes in a shape that contains the body for physics
*@param The body/shape
*/
void Entity::SetBody(pShape* shape)
{
	mBody = shape;
	mBody->GetBody()->SetUserData(this);
}


void Entity::SetBodyFixtures(FixtureData* data)
{
	mBody->GetBody()->DestroyFixture(mBody->GetBody()->GetFixtureList());
	b2Fixture* f;
	for (f = mBody->GetBody()->GetFixtureList(); f; )
	{
	    b2Fixture* fixtureToDestroy = f;
	    f = f->GetNext();
	    mBody->GetBody()->DestroyFixture( fixtureToDestroy );
	}


	for(int i = 0; i < mCurrentSprite->mHurtBoxCount;i++)
	{
		b2PolygonShape polygonShape;
		if(data[i].mType != BaseBox)
		{
		polygonShape.SetAsBox((data[i].mDimensions.x/2)/PPM,(data[i].mDimensions.y/2)/PPM
			,b2Vec2(data[i].mOffset.x/PPM,data[i].mOffset.y/PPM),0);
		}
		else
		{
			//continue;
			polygonShape.SetAsBox((data[i].mDimensions.x/2)/PPM,(data[i].mDimensions.y/2)/PPM);
		}

		b2FixtureDef Fixture;

		Fixture.shape = &polygonShape;
		Fixture.density = 1.0f;
		Fixture.friction = 0.3f;

		f= mBody->GetBody()->CreateFixture(&Fixture);
		f->SetUserData(&data[i]);	
		if(data[i].mType == BaseBox)
			mBody->SetBaseBody(f);
	}
	if(mIsFlipped == 1)
		FlipFixtures(gEntities[mID].mBody->GetBody()->GetFixtureList());
	
}
