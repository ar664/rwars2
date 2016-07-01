#ifndef __SPRITE_H
#define __SPRITE_H

#include <SFML/Graphics.hpp>
const int MAX_SPRITES = 500;
#define ANIMATION_FRAME_LENGTH	128
#define ANIMATION_FRAME_HEIGHT	128
#define ANIMATION_DEFAULT_MPF	300
#include "vectors.h"

typedef struct AnimationData{
	int				mpf;		//milliseconds per frame
	int				heldFrame;
	int				maxFrames;
	int				holdFrame;
	bool			oscillate;
}Animation;

typedef class Sprite 
{
public:
	int				mRefCount;
	int				mFramesPerLine;
	int				mWidth;
	int				mHeight;
	int				mFrameBBSet;
	char			mFileName[128];
	Animation		mAnimation;
	sf::Sprite*     mSfSprite;
	sf::IntRect*	mFrameBB;		//Frame Bounding Box
	~Sprite(void);
	void FreeSprite();
	void SetFrameBB();
};

Sprite *LoadSprite(char* filename);
void CloseSpriteList();
void SpriteListInit();

#endif
