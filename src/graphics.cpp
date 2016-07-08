#include <SFML/Graphics.hpp>
#include "globals.h"
#include "graphics.h"
sf::RenderWindow gRenderWindow;
sf::Clock gClock;
static const int gFrameRate = 30;


void Init_Graphics(int WindowWidth,int WindowHeight,char* WindowName)
{
	gRenderWindow.create(sf::VideoMode(WindowWidth,WindowHeight),WindowName);
	gRenderWindow.setFramerateLimit(gFrameRate);
}
float GetFrameRate()
{
	return 1.f/gClock.getElapsedTime().asSeconds();
}
