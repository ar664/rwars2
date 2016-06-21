#ifndef _GLOBALS_H
#define _GLOBALS_H
/**
 * @brief Contains all the globals variables!
*/

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
extern int numSprites;
extern sf::RenderWindow gRenderWindow;
extern sf::Event gEvent;
extern sf::Clock gClock;

enum AnimationCharacter
{
	ANIMATION_IDLE,
	ANIMAITON_ATTACK,
	ANIMATION_DOODLE
};


#endif
