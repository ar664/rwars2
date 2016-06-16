#ifndef _SPRITE_H
#define _SPRITE_H

#include "vectors.h"
const int MAX_SPRITES = 500;
#define ANIMATION_FRAME_LENGTH 128
#define ANIMATION_FRAME_HEIGHT 128

typedef struct AnimationData{
	char			name[20];
	int				startFrame;
	int				currentFrame;
	int				frameInc;
	int				frameRate;
	int				heldFrame;
	long			oldTime;
	int				maxFrames;
	int				oscillate;
	int				holdFrame;
}Animation;

typedef class Sprite 
{
public:
	int				refCount;
	int				fpl;
	int				width;
	int				height;
	char			filename[128];
	sf::Sprite*     sfmlSprite;
	sf::IntRect*	frameBB;
	~Sprite(void);
	void FreeSprite();
	void SetFrameBB();
};
Sprite *LoadSprite(char* filename);
void CloseSpriteList();
void InitSpriteList();
/**
* @brief steps through an animation by its frameInc
*/
void		Animate(Animation* animation,int startFrame);
void		SetCurrentFrame(Animation* animation,int Frame);
void		FreeAnimation(char* key,Animation* animation);
void		SetFrameRate(Animation* animation);
int			GetCurrentFrame(Animation* animation);
#endif
