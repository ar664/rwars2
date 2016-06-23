#include <stdio.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "physics.h"
#include "entity.h"
#include "graphics.h"
#include "globals.h"
#include "main.h"

const char *ANIMATION_IDLE_STR = "idle";
static int gDeltaTime = 1/gFrameRate;
int gMouseX = 0,gMouseY = 0;
char *test_files[] = {"sprites/Enemies3.png", 0};
Entity* ent1;
Entity test;
int main(int argc,char *argv[])
{
	Init_All();
	Loop();
	return 0;
}
void Init_All()
{
	Init_Graphics(WINDOW_WIDTH,WINDOW_HEIGHT,"RWARS");
	SpriteListInit();
	EntitySystemInit();
	ent1 = CreateEntity();
	test.Load(test_files);
	test.SetDimensions(CreateVec2D(2,2));
	test.SetCurrentAnimation(0);
	test.mCurrentSprite->SetFrameBB();
	test.mCurrentFrame = 1;
	test.SetVelocity(CreateVec2D(2,0));

}
void Loop()
{
	float accumulator = 0;				//For Physics Update
	//Entitys First FrameBB
	sf::Image image;
	//image.create(test.mCurrentSprite->mFrameBB[test.mCurrentFrame].width,
	//test.mCurrentSprite->mFrameBB[test.mCurrentFrame].height,sf::Color::Blue);

	image.create(ent1->mCurrentSprite->mFrameBB[ent1->mCurrentFrame].width,
	ent1->mCurrentSprite->mFrameBB[ent1->mCurrentFrame].height,sf::Color::Blue);

	sf::Texture *texture = new sf::Texture;
	texture->loadFromImage(image);
	sf::Sprite *sprite = new sf::Sprite;
	sprite->setTexture(*texture,1);
	sprite->setPosition(gMouseX,gMouseY);
	gClock.restart();

	float frameStart = gClock.getElapsedTime().asSeconds();
	while(gRenderWindow.isOpen())
	{
		gRenderWindow.clear();		//Clears the window

		//	Handle Physics -- This is so that there is a fixed update for physics

		accumulator += gClock.getElapsedTime().asSeconds() - frameStart; // Store Time of last frame
		frameStart = gClock.getElapsedTime().asSeconds();				// Store Time of new frame
		if(accumulator > 0.2f)
			accumulator = 0.2f;
		if(accumulator > gDeltaTime)
		{
			std::cout << "Physics Update Goes here" << std::endl;
			accumulator -= gDeltaTime;
		}
		// Remeber to handle the discrete jump in time every 6th frames or so with linear interpolation! To: Jason
		
		gRenderWindow.draw(*sprite);
		test.Draw(gRenderWindow);
		test.move(test.GetVelocity().x,test.GetVelocity().y);
		test.setPosition(gMouseX,gMouseY);
		sprite->setPosition(gMouseX,gMouseY);
		gRenderWindow.display();						//Displays whatever is drawn to the window
		while(gRenderWindow.pollEvent(gEvent))
		{
			HandleEvent(gEvent);
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

