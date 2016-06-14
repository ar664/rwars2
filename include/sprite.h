#ifndef __SPRITE_H
#define __SPRITE_H

#include <SFML/Graphics.hpp>


class RSprite : public sf::Image
{
public:
//Declaration
	RSprite(char *file, int frame_width, int frame_height, int fpl);
	~RSprite();
//Variables	
	int mFrameWidth;
	int mFrameHeight;
	int mFramesPerLine;
//Functions
	void Draw(int frame, int x, int y);

};

#endif
