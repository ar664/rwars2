#include "sprite.h"



#include <SFML\Graphics.hpp>
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
		if(SpriteList[i].sfmlSprite != NULL)
		{
			SpriteList[i].FreeSprite();
		}
	}
	free(SpriteList);
	SpriteList = NULL;
}
void Sprite::SetFrameBB()
{
	int i = 0;
	int x = 0;
	int y = 0;
	int w = 0,h = 0;
	int started = 0;
	int ended = 0;
	int startX = 0;
	int startY = 0;
	int numFrame = 1;
	sf::IntRect rect(ANIMATION_FRAME_LENGTH,-1,-1,-1);
	sf::Color spriteMask(255,51,51,255);

	sf::Image image;
	image = sfmlSprite->getTexture()->copyToImage();
	for(int j = 0; j <(sfmlSprite->getTexture()->getSize().y / ANIMATION_FRAME_HEIGHT);++j)
	{
		for(int i = 0; i < (sfmlSprite->getTexture()->getSize().x / ANIMATION_FRAME_LENGTH);++i)
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
			frameBB[i+j].top = rect.top;
			frameBB[i+j].left = rect.left;
			frameBB[i+j].width = rect.width;
			frameBB[i+j].height = rect.height;
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
	sfmlSprite->setTexture(*texture,1);
	
}
Sprite *LoadSprite(char* filename)
{
	int i;
	void *mem;
	sf::Texture *texture = new sf::Texture;
	for(i = 0; i< MAX_SPRITES;i++)
	{
		if(strncmp(filename,SpriteList[i].filename,128) == 0)
		{
			SpriteList[i].refCount++;
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
		if(SpriteList[i].refCount <= 0)break;
	}
	texture->loadFromFile(filename);
	SpriteList[i].sfmlSprite = new sf::Sprite;
	SpriteList[i].sfmlSprite->setTexture(*texture,1);
	SpriteList[i].refCount +=1;
	mem = malloc(sizeof(sf::IntRect)*SpriteList[i].sfmlSprite->getTexture()->getSize().x / ANIMATION_FRAME_LENGTH);
	SpriteList[i].frameBB = (sf::IntRect*)mem;
	memset(SpriteList[i].frameBB,0,sizeof(Vec2D));
	strcpy(SpriteList[i].filename,filename);
	SpriteList[i].fpl = SpriteList[i].sfmlSprite->getTexture()->getSize().x / ANIMATION_FRAME_LENGTH;
	SpriteList[i].width = SpriteList[i].sfmlSprite->getTexture()->getSize().x;
	SpriteList[i].height = SpriteList[i].sfmlSprite->getTexture()->getSize().y;
	return &SpriteList[i];
}

void Sprite::FreeSprite()
{
	if(refCount > 0)
	{
		refCount--;
	}
	if(refCount <= 0)
	{
		delete sfmlSprite->getTexture();
		delete sfmlSprite;
		delete frameBB;
		strcpy(filename ,"\0");
		refCount = 0;
		sfmlSprite = NULL;
	}
}
 /**
*@brief Increases or decrease the current frame of the animation
*/
/*
void Animate(Animation* animation,int startFrame) {
	if(animation->oldTime + animation->frameRate > gClock.getElapsedTime().asSeconds()) {
        return;
    }
 
    animation->oldTime = gClock.getElapsedTime().asSeconds();
	if(animation->holdFrame == 1)
	{
		animation->currentFrame = animation->startFrame + animation->heldFrame - 1;
		return;
	}
    animation->currentFrame += animation->frameInc;
 
    if(animation->oscillate) {
        if(animation->frameInc > 0) {
			if(animation->currentFrame >= animation->maxFrames-1 + startFrame) 
			{
                animation->frameInc = -animation->frameInc;
            }
        }else{
            if(animation->currentFrame <= startFrame) 
			{
                animation->frameInc = -animation->frameInc;
            }
        }
    }else{
        if(animation->currentFrame >= animation->maxFrames + startFrame) {
            animation->currentFrame = startFrame;
        }
    }
}
*/
/**
*@brief Set framerate of specified animation
*/
void SetFrameRate(Animation* animation,int Rate) {
    animation->frameRate = Rate;
}
/**
*@brief Set current frame of animation
*/
void SetCurrentFrame(Animation* animation,int Frame) {
    if(Frame < 0 || Frame >= animation->maxFrames) return;
 
    animation->currentFrame = Frame;
}
/**
*@brief Returns current frame of specified animation
*/
int GetCurrentFrame(Animation* animation) {
    return animation->currentFrame;
}
void FreeAnimation(char* key,Animation* animation)
{
	free(animation);
}
Sprite::~Sprite(void)
{
	std::cout << "Sprite Deleted"<<std::endl;

}
