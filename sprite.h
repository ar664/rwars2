#ifndef _SPRITE_H
#define _SPRITE_H

#include "vectors.h"
const int MAX_SPRITES = 500;

typedef class Sprite 
{
public:
	int				refCount;
	int				fpl;
	int				width;
	int				height;
	char			filename[128];
	sf::Sprite		*sfmlSprite;
	Vec2D*			frameBB;
	~Sprite(void);
	void FreeSprite();
	void SetFrameBB();
};
Sprite *LoadSprite(char* filename,int sizex,int sizey,int fpl);
void CloseSpriteList();
void InitSpriteList();
#endif
