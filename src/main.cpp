
#include <stdio.h>
#include <SFML\Graphics.hpp>
#include <iostream>
#include "graphics.h"
#include "globals.h"
#include "main.h"

int main(int argc,char *argv[])
{
	Init_All();
	Loop();
	return 0;
}
void Init_All()
{
	Init_Graphics(WINDOW_WIDTH,WINDOW_HEIGHT,"RWARS");
}

void Loop()
{
	while(gRenderWindow.isOpen())
	{
		while(gRenderWindow.pollEvent(gEvent))
		{
			HandleEvent(gEvent);

			gRenderWindow.clear();							//Clears the window
			gRenderWindow.display();						//Displays whatever is drawn to the window
		}
	}
}
void HandleEvent(sf::Event Event)
{
	//Close window
	if(Event.type == sf::Event::EventType::Closed)
	{
		gRenderWindow.close();						
		exit(1);
	}
}

