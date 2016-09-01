#ifndef __SPRITE_H
#define __SPRITE_H

#include <SFML/Graphics.hpp>
const int MAX_SPRITES = 500;
#define ANIMATION_FRAME_LENGTH	255
#define ANIMATION_FRAME_HEIGHT	255
#define ANIMATION_DEFAULT_MPF	100
#include "vectors.h"


struct FixtureData;
typedef struct AnimationData{
	int				mpf;		//milliseconds per frame
	int				frameInc;
	int				heldFrame;
	int				maxFrames;
	int				holdFrame;
	int				oscillate;
}Animation;


class Sprite
{
public:
	int							mRefCount;
	int							mFramesPerLine;
	
	int							mFramesCount;
	int							mBaseBoxCount;
	int							mHurtBoxCount;
	int							mAttackBoxCount;
	sf::Vector2f				mSpriteAxis;


	int							mWidth;
	int							mHeight;
	int							mFrameBBSet;
	char						mFileName[128];
	Animation					mAnimation;
	sf::Sprite*					mSfSprite;
	sf::IntRect*				mFrameBB;		//Frame Bounding Box
	FixtureData*				mHurtBoxData;
	~Sprite(void);
	void FreeSprite();
	void SetFrameBB();
};
void SetData(Sprite* sprite,const char* charName);
Sprite* LoadSprite(char* filename);
void CloseSpriteList();
void SpriteListInit();

#endif
