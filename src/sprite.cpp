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
	free(SpriteList);
	SpriteList = NULL;
}

void Sprite::SetFrameBB()
{
	int i, j, x, y, w, h;
	int started = 0;
	int ended = 0;
	int startX = 0;
	int startY = 0;
	int rowCounter = 0;
	int columnCounter = 0;
	int numFrame = 1;
	sf::IntRect rect(ANIMATION_FRAME_LENGTH,-1,-1,-1);
	sf::Color spriteMask(255,51,51,255);
	sf::Image image;
	if(!mFrameBBSet)
	{
		image = mSfSprite->getTexture()->copyToImage();
		for(j = 0; j <(mSfSprite->getTexture()->getSize().y / ANIMATION_FRAME_HEIGHT);++j)
		{
			for(i = 0; i < (mSfSprite->getTexture()->getSize().x / ANIMATION_FRAME_LENGTH);++i)
			{
				for(y=startY;y < ANIMATION_FRAME_HEIGHT*(j+1);++y)
				{
					for(x=startX;x < ANIMATION_FRAME_LENGTH*(i+1);++x)
					{
						if(image.getPixel(x,y).a == spriteMask.a &&
							image.getPixel(x,y).r == spriteMask.r &&
							image.getPixel(x,y).g == spriteMask.g &&
							image.getPixel(x,y).b == spriteMask.b)
						{
							rect.height = y;
							if(x < rect.left)
							{
								started = 1;
								rect.left = x;
								if(rect.top == 0)
								{
									rect.top = y;
								}
							}
						} 
						if(started == 1 &&
							((image.getPixel(x,y).a != 255) || (x+1 == ANIMATION_FRAME_LENGTH*(i+1))))
						{
							started = 0;
							rect.width = x+i - rect.left;
							break;
						}
						if(x > rect.left && started != 1)
						{
							break;
						}
					}
					if(started == 1)
					{
						started = 0;
						rect.width = x - rect.left;
					}
				}
				mFrameBB[i+rowCounter+j].top = rect.top;
				mFrameBB[i+rowCounter+j].left = rect.left;
				mFrameBB[i+rowCounter+j].width = rect.width;
				if(rect.height != 0)
					mFrameBB[i+rowCounter+j].height = rect.height - startY;
				std::cout<<"Frame " << i+j <<" :"<< "x: "<< rect.top << " y:" << rect.left << " w:" << rect.width<<
					" h:" << rect.height << std::endl;
				if(x*(i+1) >= mSfSprite->getTexture()->getSize().x)
				{
					startX = 0;
				}
				else
				{
					startX += ANIMATION_FRAME_LENGTH;
				}
				rect.left = ANIMATION_FRAME_LENGTH*(i+2);
				rect.top = 0;
				rect.width = 0;
				rect.height = 0;
			}
			startY += ANIMATION_FRAME_HEIGHT+1;
			rowCounter++;
		}
		
		//Creates color mask
		/*
		sf::Color color(255,51,51,255);
		image.createMaskFromColor(color);
		sf::Texture *texture = new sf::Texture();
		texture->loadFromImage(image);
		mSfSprite->setTexture(*texture,1);
		mFrameBBSet = 1;
		*/
	}
}


void SetData(Sprite* sprite,const char* charName)
{
	Document document;
	char fileName[155];
	char spriteFileName[155];
	int numFrames;

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
						else if(strcmp(data->name.GetString(),"HitBoxes") == 0 )
						{
							boxes += data->value.GetInt();
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
											hurtData[frameIndex++].mDimensions= b2Vec2(fData2[j].GetInt(),fData2[j++].GetInt());
										}
									}
								}
							}
						}
						
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
	sprite->mAnimation.heldFrame = 0;
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
		delete mFrameBB;
		strcpy(mFileName ,"\0");
		mRefCount = 0;
	}
}

Sprite::~Sprite(void)
{
	std::cout << "Sprite Deleted"<<std::endl;

}


