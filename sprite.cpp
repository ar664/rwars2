#include <SFML\Graphics.hpp>
#include <iostream>
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
	int diff = 0;
	sf::Image image;
	image = sfmlSprite->getTexture()->copyToImage();
	while(i  < fpl)
	{
		for(y = 0;y < height;++y)
		{
			if(image.getPixel(x,y).a == 0)
				{
					if(started == 1)
					{
						ended = 1;
						break;
					}
					continue;
				}
			else
			{
				started = 1;
				h += 1;
			}
			if(w == 0)
			{
				diff = x;
				for(x = x;x < width;++x)
				{
					if(image.getPixel(x,y).a != 0)
					{
						continue;
					}
					else
					{
						break;
					}
				}
				if(x == width)
					diff -=1;
				w = x -1 - diff;
				x -= 1;
			}
		}
		if(started == 1 &&(ended == 1 || y == height))
		{
			if(i == 0)
				frameBB[i] = CreateVec2D(w+1,h+1);
			else
				frameBB[i] = CreateVec2D(w+i-1,h+1);
			w = 0;
			h = 0;
			started = 0;
			ended = 0;
			i+=1;
		}
		x++;
 	}

}
Sprite *LoadSprite(char* filename,int sizex,int sizey,int fpl)
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
	SpriteList[i].width = sizex;
	SpriteList[i].height = sizey;
	SpriteList[i].refCount +=1;
	mem = malloc(sizeof(Vec2D)*fpl);
	SpriteList[i].frameBB = (Vec2D*)mem;
	//SpriteList[i].frameBB = (Vec2D*)malloc(sizeof(Vec2D) * fpl);
	memset(SpriteList[i].frameBB,0,sizeof(Vec2D));
	strcpy(SpriteList[i].filename,filename);
	SpriteList[i].fpl = fpl;

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
		strcpy(filename ,"\0");
		refCount = 0;
		sfmlSprite = NULL;
	}
}

Sprite::~Sprite(void)
{
	std::cout << "Sprite Deleted"<<std::endl;

}
