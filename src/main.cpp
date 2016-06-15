
#include <stdio.h>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <iostream>
#include "entity.h"
#include "graphics.h"
#include "globals.h"
#include "main.h"

int mouseX = 0,mouseY = 0;
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
	Entity ent;
	sf::Image image;
	image.create(75,75,sf::Color::Blue);

	sf::Texture texture;
	texture.loadFromImage(image);
	sf::Sprite sprite(texture);
	
	sprite.setPosition(mouseX,mouseY);
	ent.rSprite = (Sprite*)malloc(sizeof(Sprite));
	ent.rSprite->sfmlSprite = sprite;

	//delete ent.sp;
	while(gRenderWindow.isOpen())
	{
		while(gRenderWindow.pollEvent(gEvent))
		{
			HandleEvent(gEvent);

			gRenderWindow.clear();		//Clears the window
			gRenderWindow.draw(ent);
			//gRenderWindow.draw(sprite);
			ent.setPosition(mouseX,mouseY);
			std::cout << ent.getPosition().x << " , " << ent.getPosition().y << std::endl;
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
	else if(Event.type == sf::Event::EventType::MouseMoved)
	{
		mouseX = Event.mouseMove.x;
		mouseY = Event.mouseMove.y;

	}
}

