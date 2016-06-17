#include <SFML/Graphics.hpp>
#include "globals.h"
#include "graphics.h"
sf::RenderWindow gRenderWindow;
sf::Clock gClock;


void Init_Graphics(int WindowWidth,int WindowHeight,char* WindowName)
{
	gRenderWindow.create(sf::VideoMode(WindowWidth,WindowHeight),WindowName);
}
