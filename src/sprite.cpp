#include "sprite.h"
#include <SFML/Graphics.hpp>
#include <malloc.h>
#include <string.h>
#include <iostream>
#include "include\rapidjson\filereadstream.h"
#include "include\rapidjson\document.h"
#include "include\resourcemanager.h"
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


Sprite* SetData(Sprite* spriteArray,const char* charName)
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
	assert(document.HasMember("HitBoxData"));
	for(Value::ConstMemberIterator hitBoxItr =document.MemberBegin();
		hitBoxItr != document.MemberEnd(); ++hitBoxItr)
	{
		if(hitBoxItr->value.IsArray())
		{
			const Value& hitBoxData = document["HitBoxData"];
			assert(hitBoxData.IsArray());
			//Start iterating through each sprites Hitboxs
			for (SizeType i = 0; i < hitBoxData.Size(); i++)
			{
				//Iterate through the members of the object
				for(Value::ConstMemberIterator hitBoxContent =hitBoxData[i].MemberBegin();
					hitBoxContent != hitBoxData[i].MemberEnd(); ++hitBoxContent)
				{
					if(strcmp(hitBoxContent->name.GetString(),"AnimName") == 0)
					{
						strcpy(spriteFileName,"");
						strcat(spriteFileName,"sprites/");
						strcat(spriteFileName,hitBoxContent->value.GetString());
						strcat(spriteFileName,".png");
						printf("%s\n",spriteFileName);
					}
					else if(strcmp(hitBoxContent->name.GetString(),"NumFrames") == 0)
					{
						numFrames = hitBoxContent->value.GetInt();
					}
					else if(strcmp(hitBoxContent->name.GetString(),"BoxData") == 0)
					{
						sf::IntRect* rects = new sf::IntRect[numFrames];
						//Start Getting the HitBox Data for Each Frame of the Animation
						assert(hitBoxContent->value.IsArray());
						const Value& BoxDataArray = hitBoxContent->value;
						for (SizeType j = 0; j < BoxDataArray.Size(); j++)
						{
							//Make sure the Object has member FrameNum
							assert(BoxDataArray[j].HasMember("FrameNum"));
							//Iterate Through BoxData Members
							for(Value::ConstMemberIterator BoxDataMember =BoxDataArray[j].MemberBegin();
								BoxDataMember != BoxDataArray[j].MemberEnd(); ++BoxDataMember)
								{
									int frameNum;
									if(strcmp(BoxDataMember->name.GetString(),"FrameNum") == 0)
									{
										frameNum = BoxDataMember->value.GetInt()-1;
										printf("%s\n",BoxDataMember->name.GetString());
									}
									else if(strcmp(BoxDataMember->name.GetString(),"TopLeft") == 0)
									{
										const Value& b = BoxDataMember->value;
										printf("%d\n",frameNum);
										rects[frameNum].left = b[0].GetInt();
										rects[frameNum].top  = b[1].GetInt();
									}
									else if(strcmp(BoxDataMember->name.GetString(),"BottomRight") == 0)
									{
										const Value& b = BoxDataMember->value;
										printf("%d\n",frameNum);
										rects[frameNum].width =  b[0].GetInt() - rects[frameNum].left ;
										rects[frameNum].height = b[1].GetInt() - rects[frameNum].top ;
									}
								}
						}
						spriteArray->mFrameBB = rects;
					}
				}

			}
		}
	}
	return spriteArray;
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
	sprite->mAnimation.oscillate = false;

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


