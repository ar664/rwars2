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

Sprite *LoadSprite(char* filename,int sizex,int sizey,int fpl)
{
	int i;
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
	SpriteList[i].refCount +=1 ;
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
