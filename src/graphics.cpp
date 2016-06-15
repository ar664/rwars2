#include <SFML\Graphics.hpp>
#include "globals.h"
#include "graphics.h"
sf::RenderWindow gRenderWindow;
sf::Clock gClock;
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;


void Init_Graphics(int WindowWidth,int WindowHeight,char* WindowName)
{
	gRenderWindow.create(sf::VideoMode(WindowWidth,WindowHeight),WindowName);
}
