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

}
/*
Sprite *LoadSprite(char* filename,int sizex,int sizey,char*,int fpl)
{
	sf::Texture texture;
	for(int i = 0; i< MAX_SPRITES;i++)
	{
		if(strncmp(filename,SpriteList[i].filename,40) == 0)
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
	texture.loadFromFile(filename);
	
}*/


Sprite::~Sprite(void)
{
	std::cout << "Sprite Deleted"<<std::endl;

}
