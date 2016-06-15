#ifndef _SPRITE_H
#define _SPRITE_H

#include "vectors.h"
const int MAX_SPRITES = 500;

typedef class Sprite : sf::Sprite
{

public:
	int				refCount;
	int				fpl;
	char*			filename;
	sf::Sprite		*sfmlSprite;
	Vec2D			frameBB;
	~Sprite(void);

};
Sprite *LoadSprite(char* filename,int sizex,int sizey,char*,int fpl);
void CloseSpriteList();
void InitSpriteList();
#endif
