#include <stdio.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "entity.h"
#include "graphics.h"
#include "globals.h"
#include "main.h"

const char *ANIMATION_IDLE_STR = "idle";
int gMouseX = 0,gMouseY = 0;
char *test_files[] = {"sprites/Enemies3.png", 0};
sf::Music **playlist;
Entity test_ent;

int main(int argc,char *argv[])
{
	Init_All();
	Loop();
	return 0;
}
void Init_All()
{
	Init_Graphics(WINDOW_WIDTH,WINDOW_HEIGHT,"RWARS");
	InitSpriteList();
	gClock.restart();

	test_ent.Load(test_files);
	test_ent.SetDimensions(CreateVec2D(2,2));
	test_ent.SetCurrentAnimation(0);
	test_ent.mCurrentSprite->SetFrameBB();
	test_ent.mCurrentFrame = 2;
	
	std::cout << test_ent.mCurrentSprite->mSfSprite->getTexture()->getSize().x << " "<<
		test_ent.mCurrentSprite->mSfSprite->getTexture()->getSize().y <<  std::endl;

}

void Loop()
{
	sf::Image image;
	image.create(test_ent.mCurrentSprite->mFrameBB[test_ent.mCurrentFrame].width,
	test_ent.mCurrentSprite->mFrameBB[test_ent.mCurrentFrame].height,sf::Color::Blue);

	sf::Texture *texture = new sf::Texture;
	texture->loadFromImage(image);
	sf::Sprite *sprite = new sf::Sprite;
	sprite->setTexture(*texture,1);
	sprite->setPosition(gMouseX,gMouseY);
	while(gRenderWindow.isOpen())
	{
		while(gRenderWindow.pollEvent(gEvent))
		{
			HandleEvent(gEvent);
			gRenderWindow.clear();		//Clears the window
			gRenderWindow.draw(*sprite);
			test_ent.Draw(gRenderWindow);
			sprite->setPosition(gMouseX,gMouseY);
			test_ent.setPosition(gMouseX,gMouseY);
			std::cout << test_ent.getPosition().x << " , " << 
				test_ent.getPosition().y << std::endl;
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
		gMouseX = Event.mouseMove.x;
		gMouseY = Event.mouseMove.y;

	}
}

