#include <stdio.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "entity.h"
#include "graphics.h"
#include "globals.h"
#include "main.h"

int mouseX = 0,mouseY = 0;
Entity ent;

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

	ent.rSprite = LoadSprite("sprites/Enemies3.png");
	ent.SetDimensions(CreateVec2D(2,2));
	ent.rSprite->SetFrameBB();
	ent.mFrameNum = 2;
	Animation* anim = new Animation;
	anim->currentFrame = 0;
	anim->frameInc = 1;
	anim->frameRate = 10;
	anim->maxFrames = 2;
	anim->oscillate = 1;
	ent.animations.insert(std::make_pair<char*,Animation*>((char*)("idle"),anim));
	ent.SetCurrentAnimation(anim);
	std::cout << ent.rSprite->sfmlSprite->getTexture()->getSize().x << " "<<
		ent.rSprite->sfmlSprite->getTexture()->getSize().y <<  std::endl;
	//ent.rSprite = (Sprite*)malloc(sizeof(Sprite));
	//ent.rSprite->sfmlSprite = sprite;

}

void Loop()
{
	sf::Image image;
	image.create(ent.rSprite->frameBB[ent.mFrameNum].width,
	ent.rSprite->frameBB[ent.mFrameNum].height,sf::Color::Blue);

	sf::Texture *texture = new sf::Texture;
	texture->loadFromImage(image);
	sf::Sprite *sprite = new sf::Sprite;
	sprite->setTexture(*texture,1);
	sprite->setPosition(mouseX,mouseY);
	while(gRenderWindow.isOpen())
	{
		while(gRenderWindow.pollEvent(gEvent))
		{
			HandleEvent(gEvent);
			gRenderWindow.clear();		//Clears the window
			gRenderWindow.draw(*sprite);
			ent.Draw(gRenderWindow);
			sprite->setPosition(mouseX,mouseY);
			ent.setPosition(mouseX,mouseY);
			std::cout << ent.getPosition().x << " , " << 
				ent.getPosition().y << std::endl;
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

