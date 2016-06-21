#include "sprite.h"
#include <SFML/Graphics.hpp>
#include <malloc.h>
#include <string.h>
#include <iostream>
#include "vectors.h"
#include "globals.h"
#include "sprite.h"
Sprite *SpriteList = NULL;
int numSprites = 0;

void InitSpriteList()
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
	for (i = 0;i < MAX_SPRITES;i++)
	{
		if(SpriteList[i].mSfSprite != NULL)
		{
			SpriteList[i].FreeSprite();
		}
	}
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
	int numFrame = 1;
	sf::IntRect rect(ANIMATION_FRAME_LENGTH,-1,-1,-1);
	sf::Color spriteMask(255,51,51,255);

	sf::Image image;
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
							if(rect.top == -1)
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
			mFrameBB[i+j].top = rect.top;
			mFrameBB[i+j].left = rect.left;
			mFrameBB[i+j].width = rect.width;
			mFrameBB[i+j].height = rect.height;
			std::cout<<"Frame " << i+j <<" :"<< "x: "<< rect.top << " y:" << rect.left << " w:" << rect.width<<
				" h:" << rect.height << std::endl;
			startX += ANIMATION_FRAME_LENGTH+1;
			rect.left = ANIMATION_FRAME_LENGTH*(i+2);
			rect.top = -1;
			rect.width = -1;
			rect.height = -1;
		}
		startY += ANIMATION_FRAME_HEIGHT+1;
	}
	
	//Creates color mask
	sf::Color color(255,51,51,255);
	image.createMaskFromColor(color);
	sf::Texture *texture = new sf::Texture;
	texture->loadFromImage(image);
	mSfSprite->setTexture(*texture,1);
	
}

Sprite *LoadSprite(char* filename)
{
	int i;
	void *rect;
	Sprite* sprite;
	sf::Texture *texture = new sf::Texture;
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
	texture->loadFromFile(filename);
	sprite->mSfSprite = new sf::Sprite;
	sprite->mSfSprite->setTexture(*texture,1);
	sprite->mRefCount +=1;

	//Set Physics Dimensions
	rect = malloc(sizeof(sf::IntRect)*sprite->mSfSprite->getTexture()->getSize().x / ANIMATION_FRAME_LENGTH);
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

	sprite->SetFrameBB();

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
		delete mSfSprite->getTexture();
		delete mSfSprite;
		delete mFrameBB;
		strcpy(mFileName ,"\0");
		mRefCount = 0;
		mSfSprite = NULL;
	}
}

Sprite::~Sprite(void)
{
	std::cout << "Sprite Deleted"<<std::endl;

}
