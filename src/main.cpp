
#include <stdio.h>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <iostream>
#include "physics.h"
#include "entity.h"
#include "graphics.h"
#include "globals.h"
#include "main.h"

int mouseX = 0,mouseY = 0;
Entity ent;
Entity ent2;
int numFrames = 1;
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

	ent.rSprite = LoadSprite("sprites/Enemies3.png");
	ent.SetDimensions(CreateVec2D(127,127));
	ent.rSprite->SetFrameBB();
	ent.mFrameNum = 2;
	ent.mBody.mass = 2;
	ent.mBody.restitution = 20;
	Animation* anim = new Animation;
	anim->currentFrame = 0;
	anim->frameInc = 1;
	anim->frameRate = 10;
	anim->maxFrames = 2;
	anim->oscillate = 1;
	ent.animations.insert(std::make_pair<char*,Animation*>("idle",anim));
	 std::unordered_map<char*,Animation*>::const_iterator got = ent.animations.find ("idle");
	 ent.SetCurrentAnimation((Animation*)(got->first));
	//ent.setPosition(320,300);
	
	//ent.setPosition(0,300);
	//ent.SetVelocity(CreateVec2D(2,0));
	
	ent.setPosition(200,0);
	ent.SetVelocity(CreateVec2D(2,2));

	ent2.rSprite = LoadSprite("sprites/Enemies3.png");
	ent2.SetDimensions(CreateVec2D(127,127));
	ent2.mFrameNum = 2;
	ent2.mBody.mass = 15;
	ent2.mBody.restitution = 50;
	ent2.animations.insert(std::make_pair<char*,Animation*>("idle",anim));
	ent2.SetCurrentAnimation(anim);
	//ent2.setPosition(300,300);
	
	ent2.setPosition(550,300);
	ent2.SetVelocity(CreateVec2D(-2,0));
	
	//ent.rSprite = (Sprite*)malloc(sizeof(Sprite));
	//ent.rSprite->sfmlSprite = sprite;

}

void Loop()
{
	//First Entity FrameBB
	sf::Image image;
	image.create(ent.rSprite->frameBB[ent.mFrameNum].width,
	ent.rSprite->frameBB[ent.mFrameNum].height,sf::Color::Blue);

	sf::Texture *texture = new sf::Texture;
	texture->loadFromImage(image);
	sf::Sprite *sprite = new sf::Sprite;
	sprite->setTexture(*texture,1);
	sprite->setPosition(mouseX,mouseY);
	//Second Entity FrameBB	
	sf::Image image2;
	image2.create(ent2.rSprite->frameBB[ent2.mFrameNum].width,
	ent2.rSprite->frameBB[ent2.mFrameNum].height,sf::Color::Blue);

	sf::Texture *texture2 = new sf::Texture;
	texture2->loadFromImage(image2);
	sf::Sprite *sprite2 = new sf::Sprite;
	sprite2->setTexture(*texture2,1);
	sprite2->setPosition(100,100);

	float normalx,normaly;
	while(gRenderWindow.isOpen())
	{
			gRenderWindow.clear();		//Clears the window
			gRenderWindow.draw(*sprite);
			gRenderWindow.draw(*sprite2);
			ent2.Draw(gRenderWindow);
			ent.Draw(gRenderWindow);
			sprite2->setPosition(ent2.getPosition());
			sprite->setPosition(ent.getPosition());

			ent.move(ent.GetVelocity().x,ent.GetVelocity().y);
			ent2.move(ent2.GetVelocity().x,ent2.GetVelocity().y);
			//sprite->setPosition(mouseX,mouseY);
			//ent.setPosition(mouseX,mouseY);
			//Collision Test
			AABB(&ent,&ent2);
			gClock.restart();
			//Collision Test
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
		mouseX = Event.mouseMove.x;
		mouseY = Event.mouseMove.y;

	}
}

