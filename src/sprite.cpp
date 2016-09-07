#include "sprite.h"
#include <SFML/Graphics.hpp>
#include <malloc.h>
#include <string.h>
#include <iostream>
#include "include\rapidjson\filereadstream.h"
#include "include\rapidjson\document.h"
#include "include\resourcemanager.h"
#include "shape.h"
#include "vectors.h"
#include "globals.h"
#include "sprite.h"
Sprite *SpriteList = NULL;
int numSprites = 0;

using namespace rapidjson;

void SpriteListInit()
{
	int x,j;
	SpriteList = (Sprite*)malloc(sizeof(Sprite) * MAX_SPRITES);
	memset(SpriteList,0,sizeof(Sprite) * MAX_SPRITES);
	atexit(CloseSpriteList);


}

void CloseSpriteList()
{
	int i,j;
	if(!SpriteList)
	{
		return;
	}
	ResourceManager::FreeCaches();
	for(int i = 0; i < MAX_SPRITES;i++)
	{
		SpriteList[i].FreeSprite();
	}
	free(SpriteList);
	SpriteList = NULL;
}

void SetData(Sprite* sprite,const char* charName)
{
	Document document;
	char fileName[155];
	char spriteFileName[155];
	int numFrames = 0;

	strcpy(fileName,charName);
	strcat(fileName,".json");
	
	FILE *file = fopen(fileName,"rb");

	char buffer[65536];
	FileReadStream frs(file,buffer,sizeof(buffer));
	//Parse the FileReadStream and close file
	document.ParseStream(frs);
	fclose(file);
	assert(document.IsObject());
	assert(document.HasMember("AnimationData"));

	FixtureData* hurtData;
	FixtureData* hitData;
	int boxes = 0;

	for(Value::ConstMemberIterator AnimationData =document.MemberBegin();
		AnimationData != document.MemberEnd(); ++AnimationData)
	{
		if(AnimationData->value.IsArray())
		{
			const Value& AnimData = document["AnimationData"];
			for (SizeType i = 0; i < AnimData.Size(); i++)
			{
				//Iterate through the members of the object
				if(strcmp(AnimData[i].MemberBegin()->value.GetString(),sprite->mFileName) == 0)
				{
					for(Value::ConstMemberIterator data =AnimData[i].MemberBegin();
						data != AnimData[i].MemberEnd(); ++data)
					{
						if(strcmp(data->name.GetString(),"BaseBoxes") == 0)
						{
							boxes += data->value.GetInt();
						}
						else if(strcmp(data->name.GetString(),"HurtBoxes") == 0 )
						{
							boxes += data->value.GetInt();
						}
						else if(strcmp(data->name.GetString(),"NumFrames") == 0 )
						{
							numFrames += data->value.GetInt();
						}
						else if(strcmp(data->name.GetString(),"BaseBoxDef") == 0 )
						{
							hurtData = new FixtureData[boxes];
							sprite->mHurtBoxCount = boxes;
							const Value& fData = data->value;
							for (SizeType j = 0; j < fData.Size(); j++)
							{
								hurtData[0].mOffset = b2Vec2(fData[j].GetInt(),fData[j++].GetInt());
								hurtData[0].mDimensions = b2Vec2(50,20);
								hurtData[0].mColor= sf::Color(0,255,0,100);
								hurtData[0].mType= BaseBox;
							}
						}
						else if(strcmp(data->name.GetString(),"SpriteAxis") == 0 )
						{	
							const Value& fData = data->value;
							for (SizeType j = 0; j < fData.Size(); j++)
							{
								sprite->mSpriteAxis = sf::Vector2f(fData[j].GetInt(),fData[j+1].GetInt());
								j++;
							}
						}
						else if(strcmp(data->name.GetString(),"HurtBoxDef") == 0 )
						{
							const Value& fData = data->value;
							int frameIndex = 1;
							//Iterate through members
							for(Value::ConstMemberIterator data2 =fData.MemberBegin();
								data2 != fData.MemberEnd(); ++data2)
							{
								for(Value::ConstMemberIterator data3 =data2->value.MemberBegin();
									data3 != data2->value.MemberEnd(); ++data3)
								{
									if(strcmp(data3->name.GetString(),"HurtBoxOffset") == 0)
									{
										const Value& fData2 = data3->value;
										for (SizeType j = 0; j < fData2.Size(); j++)
										{
											hurtData[frameIndex].mType = HurtBox;
											hurtData[frameIndex].mColor = sf::Color(0,0,255,100);
											hurtData[frameIndex].mOffset = b2Vec2(fData2[j].GetInt(),fData2[j+1].GetInt());
											j++;
										}
									}
									else if(strcmp(data3->name.GetString(),"HurtBoxDimensions") == 0)
									{
										const Value& fData2 = data3->value;
										for (SizeType j = 0; j < fData2.Size(); j++)
										{
											hurtData[frameIndex].mDimensions= b2Vec2(fData2[j].GetInt(),fData2[j+1].GetInt());
											j++;
											frameIndex++;
										}
									}
								}
							}
						}
						else if(strcmp(data->name.GetString(),"MPF") == 0)
						{
							sprite->mAnimation.mpf = data->value.GetInt();
						}
						else if(strcmp(data->name.GetString(),"HeldFrame") == 0)
						{
							sprite->mAnimation.heldFrame= data->value.GetInt();
						}
						//else if(strcmp(data->name.GetString(),"HitBoxDef") == 0)
						//{
						//	const Value& fData = data->value;
						//	int frameIndex = 0;
						//	hitData = new FixtureData[numFrames];
						//	memset(hitData,0,sizeof(hitData));
						//
						//	//Iterate through members
						//	for(Value::ConstMemberIterator data2 =fData.MemberBegin();
						//		data2 != fData.MemberEnd(); ++data2)
						//	{
						//		for(Value::ConstMemberIterator data3 =data2->value.MemberBegin();
						//			data3 != data2->value.MemberEnd(); ++data3)
						//		{
						//			if(strcmp(data3->name.GetString(),"Frame") == 0)
						//			{
						//				hitData[frameIndex].mFrame = data3->value.GetInt();
						//			}
						//			else if(strcmp(data3->name.GetString(),"HitBoxOffset") == 0)
						//			{
						//				const Value& fData2 = data3->value;
						//				for (SizeType j = 0; j < fData2.Size(); j++)
						//				{
						//					hitData[frameIndex].mType = HitBox;
						//					hitData[frameIndex].mColor = sf::Color(255,0,0,100);
						//					hitData[frameIndex].mOffset = b2Vec2(fData2[j].GetInt(),fData2[j+1].GetInt());
						//					j++;
						//				}
						//			}
						//			else if(strcmp(data3->name.GetString(),"HitBoxDimensions") == 0)
						//			{
						//				const Value& fData2 = data3->value;
						//				for (SizeType j = 0; j < fData2.Size(); j++)
						//				{
						//					hitData[frameIndex].mDimensions= b2Vec2(fData2[j].GetInt(),fData2[j+1].GetInt());
						//					j++;
						//					frameIndex++;
						//				}
						//			}
						//		}
						//	}
						//
						//}
					}
				}
			}
		}
	}

	sprite->mHurtBoxData = hurtData;
}

Sprite *LoadSprite(char* filename)
{
	int i;
	void *rect;
	Sprite* sprite;
	for(i = 0; i< MAX_SPRITES;i++)
	{
		if(strncmp(filename,SpriteList[i].mFileName,128) == 0)
		{
			SpriteList[i].mRefCount++;
			return &SpriteList[i];
		}
	}
	if(numSprites + 1 >= MAX_SPRITES)
	{
		fprintf(stderr,"Max Sprites Reached.\n");
		exit(1);
	}
	numSprites++;
	 for(i = 0;i <= MAX_SPRITES;i++)
	{
		if(SpriteList[i].mRefCount <= 0)break;
	}
	sprite = &SpriteList[i];
	sprite->mSfSprite = ResourceManager::GetmfSprite(filename);
	sprite->mSfSprite->setTexture(*ResourceManager::GetTexture(filename));
	sprite->mRefCount +=1;
	sprite->mAnimation.frameInc = 1;
	strcpy(sprite->mFileName,filename);
	//Set Physics Dimensions
	int l = sprite->mSfSprite->getTexture()->getSize().x / ANIMATION_FRAME_LENGTH;
	int w = sprite->mSfSprite->getTexture()->getSize().y / ANIMATION_FRAME_LENGTH;

	rect = malloc(sizeof(sf::IntRect)* l * w);
	sprite->mFrameBB = (sf::IntRect*)rect;
	memset(sprite->mFrameBB,0,sizeof(Vec2D));
	strcpy(sprite->mFileName,filename);

	//Set Animation Dimensions
	sprite->mFramesPerLine = sprite->mSfSprite->getTexture()->getSize().x / ANIMATION_FRAME_LENGTH;
	sprite->mWidth = sprite->mSfSprite->getTexture()->getSize().x;
	sprite->mHeight = sprite->mSfSprite->getTexture()->getSize().y;

	//Setup Animation Data
	sprite->mAnimation.maxFrames = sprite->mFramesPerLine * (sprite->mHeight / ANIMATION_FRAME_HEIGHT);
	sprite->mAnimation.heldFrame = 100;
	sprite->mAnimation.mpf = ANIMATION_DEFAULT_MPF;
	sprite->mAnimation.oscillate = 0;

	//sprite->SetFrameBB();

	return &SpriteList[i];
}

void Sprite::FreeSprite()
{
	if(mRefCount > 0)
	{
		mRefCount--;
	}
	if(mRefCount <= 0)
	{
		ResourceManager::FreemfSprite(mFileName);
		delete mHurtBoxData;
		mHurtBoxData = nullptr;
		delete mAttackBoxData;
		mAttackBoxData = nullptr;
		strcpy(mFileName ,"\0");
		mRefCount = 0;
	}
	
}

Sprite::~Sprite(void)
{
	std::cout << "Sprite Deleted"<<std::endl;

}


