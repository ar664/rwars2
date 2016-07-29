#include <stdio.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string.h>
#include "fgen.h"
#include "physics.h"
#include "audio.h"
#include "entity.h"
#include "player.h"
#include "graphics.h"
#include "globals.h"
#include "shape.h"
#include "main.h"

const char *ANIMATION_IDLE_STR = "idle";
float gDeltaTime = (float)1/(float)gFrameRate;
float deltaTime = 0;
int gMouseX = 0,gMouseY = 0;
char *test_files[] = {"sprites/Crate.png", 0};
Entity* ent1, *ent2,*ent3 ,*ent4,*ent5,*ent6;
Entity test, test2;
Polygon *p;

int main(int argc,char *argv[])
{
	Init_All();
	Loop();
	return 0;
}

void LoadAssets()
{
	int i = 0, j = 0, songs_loaded = 0, sounds_loaded = 0, characters_loaded = 0;
	char *temp = (char*) malloc(sizeof(char)*256);
	char **files = (char**) malloc(sizeof(char*)*16);
	FILE *file = fopen(ASSET_FILE, "r");

	//First line is expected to be a comment, we get it and ignore it.
	fgets(temp, 255, file);							
	memset(temp, 0, 255);

	while(!feof(file))
	{
		temp[i] = fgetc(file);
		if(i > 255 || j > 15)
		{
			printf("Loading Assets went wrong, format invalid! Exiting...");
			std::cin.get(temp, 0);
			exit(0);
		}

		//Check if commented line
		if(temp[i] == '\\')
		{
			fgets(temp, 255, file);							
			memset(temp, 0, 255);
			continue;
		}

		//Files of one character are seperated by spaces
		if(temp[i] == ' ')
		{
			temp[i] = NULL;
			files[j] = temp;
			temp = (char*) malloc(sizeof(char)*256);
			i = 0;
			j++;
			continue;
		}

		if(temp[i] == '\n')
		{
			//Check for blank line
			if(i == 0)
			{
				temp[0] = 0;
				continue;
			}
			temp[i] = NULL;
			files[j] = temp;
			files[j+1] = NULL;
			if(characters_loaded < ASSETS_CHARACTERS)
			{
				PlayerLoad(characters_loaded, files);
				temp = (char*) malloc(sizeof(char)*256);
				files = (char**) malloc(sizeof(char*)*16);
				i = 0; j = 0;
				characters_loaded++;
				continue;
			}
			
			if(sounds_loaded < ASSETS_CHARACTERS)
			{
				Characters[sounds_loaded]->LoadSounds(files);
				temp = (char*) malloc(sizeof(char)*256);
				files = (char**) malloc(sizeof(char*)*16);
				i = 0; j = 0;
				sounds_loaded++;
				continue;
			}
			
			if(songs_loaded < ASSETS_SONGS)
			{
				AudioLoadSongs(files);				
				temp = (char*) malloc(sizeof(char)*256);
				files = (char**) malloc(sizeof(char*)*16);
				i = 0; j = 0;
				songs_loaded++;
				continue;
			}
		}

		i++;
	}


}

void Init_All()
{

	Init_Graphics(WINDOW_WIDTH,WINDOW_HEIGHT,"RWARS");
	SpriteListInit();
	EntitySystemInit();
	LoadAssets();
	gGrid = new Grid(6,6,100);


	//p = new Polygon();
	ent1 = CreateEntity();	
	//ent1->mBody = new RigidBody(p);
	//ent1->mBody->position.x = 100;
	//ent1->mBody->position.y = 0;
	//p->SetOrientation(0);


	//ent1->LoadSprites(test_files);
	//ent1->SetDimensions(CreateVec2D(127,127));
	//ent1->SetCurrentAnimation(0);
	//ent1->mCurrentFrame = 0;
	//ent1->setPosition(0,0);
	//ent1->SetVelocity(CreateVec2D(0,0));
	//ent1->mBody->mass = 1;
	//ent1->mBody->restitution =.8;
	//ent1->mBody->staticFriction = 0.001;
	//ent1->mBody->dynamicFriction = 0.001;
	/*
	Vec2D * arr = new Vec2D[4];
	arr[0] = CreateVec2D(1,1);
	arr[1] = CreateVec2D(300,100);
	arr[2] = CreateVec2D(1,300);
	arr[3] = CreateVec2D(400,400);
	p->rbody->r = 10;
	p->rbody->g = 100;
	p->rbody->b = 255;

	p->Set(arr,4);
	*/

	//ent3 = CreateEntity();	
	//ent3->LoadSprites(test_files);
	//ent3->SetDimensions(CreateVec2D(127,127));
	//ent3->SetCurrentAnimation(0);
	//ent3->mCurrentFrame = 0;
	//ent3->SetVelocity(CreateVec2D(0,0));
	//ent3->setPosition(700,0);
	//ent3->mBody->mass = 40;
	//ent3->mBody->restitution = .8;
	//ent3->mBody->staticFriction = .001;
	//ent3->mBody->dynamicFriction = .001;
	
	ent4 = CreateEntity();	
	ent4->LoadSprites(test_files);
	ent4->SetDimensions(CreateVec2D(127,127));
	ent4->SetCurrentAnimation(0);
	ent4->mCurrentFrame = 0;
	ent4->SetVelocity(CreateVec2D(0,0));
	ent4->setPosition(400,150);
	ent4->mBody->mass = 40;
	ent4->mBody->restitution = .8;
	ent4->mBody->staticFriction = .001;
	ent4->mBody->dynamicFriction = .001;
	
	ent5 = CreateEntity();	
	ent5->LoadSprites(test_files);
	ent5->SetDimensions(CreateVec2D(127,127));
	ent5->SetCurrentAnimation(0);
	ent5->mCurrentFrame = 0;
	ent5->SetVelocity(CreateVec2D(0,0));
	ent5->setPosition(400,300);
	ent5->mBody->mass = 40;
	ent5->mBody->restitution = .8;
	ent5->mBody->staticFriction = .001;
	ent5->mBody->dynamicFriction = .001;
	
	//ent6 = CreateEntity();	
	//ent6->LoadSprites(test_files);
	//ent6->SetDimensions(CreateVec2D(127,127));
	//ent6->SetCurrentAnimation(0);
	//ent6->mCurrentFrame = 0;
	//ent6->SetVelocity(CreateVec2D(0,0));
	//ent6->setPosition(400,-150);
	//ent6->mBody->mass = 40;
	//ent6->mBody->restitution = .8;
	//ent6->mBody->staticFriction = .001;
	//ent6->mBody->dynamicFriction = .001;


	ent2 =CreateEntity();
	ent2->LoadSprites(test_files);
	ent2->SetDimensions(CreateVec2D(4000,127));
	ent2->SetCurrentAnimation(0);
	ent2->mCurrentFrame = 0;
	ent2->SetVelocity(CreateVec2D(0,0));
	ent2->setPosition(0,400);
	ent2->mBody->mass = 0;
	ent2->mBody->restitution = 0;
	ent2->mBody->staticFriction =.001;
	ent2->mBody->dynamicFriction = .001;

	//forceRegistry.add(ent1->mBody,&gravity);
	//forceRegistry.add(ent3->mBody,&gravity);
	forceRegistry.add(ent4->mBody,&gravity);
	forceRegistry.add(ent5->mBody,&gravity);
	//forceRegistry.add(ent6->mBody,&gravity);

	CallbackInitSystem();
	gClock.restart();
}
void Loop()
{
	float accumulator = 0;				//For Physics Update
	//Entitys First FrameBB
	sf::Image image,image2;
	//image.create(ent1->mCurrentSprite->mFrameBB[ent1->mCurrentFrame].width,
	//ent1->mCurrentSprite->mFrameBB[ent1->mCurrentFrame].height,sf::Color::Blue);

	image2.create(800,
	ent2->mCurrentSprite->mFrameBB[ent2->mCurrentFrame].height,sf::Color::Blue);

	//sf::Texture *texture = new sf::Texture;
	//texture->loadFromImage(image);
	//sf::Sprite *sprite = new sf::Sprite;
	//sprite->setTexture(*texture,1);
	//sprite->setPosition(gMouseX,gMouseY);

	sf::Texture *texture2 = new sf::Texture;
	texture2->loadFromImage(image2);
	sf::Sprite *sprite2 = new sf::Sprite;
	sprite2->setTexture(*texture2,1);
	sprite2->setPosition(gMouseX,gMouseY);

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
			deltaTime = gClock.getElapsedTime().asSeconds() /frameStart;
			UpdatePhysics(deltaTime);
			std::cout << "Physics Update Goes here" << std::endl;
			accumulator -= gDeltaTime;
		}
		// Remeber to handle the discrete jump in time every 6th frames or so with linear interpolation! To: Jason
		
		//gRenderWindow.draw(*sprite);
		gRenderWindow.draw(*sprite2);
		//ent1->Draw(gRenderWindow);
		ent2->Draw(gRenderWindow);
		
		//ent1->mBody->shape->Draw(gRenderWindow);
		//ent1->mBody->position.x = gMouseX;
		//ent1->mBody->position.y = gMouseY;

		//ent3->Draw(gRenderWindow);
		ent4->Draw(gRenderWindow);
		ent5->Draw(gRenderWindow);
		//ent6->Draw(gRenderWindow);
		//sprite->setPosition(ent1->getPosition().x,ent1->getPosition().y);
		sprite2->setPosition(ent2->getPosition().x,ent2->getPosition().y);
		gRenderWindow.display();						//Displays whatever is drawn to the window
		while(gRenderWindow.pollEvent(gEvent))
		{
			HandleEvent(gEvent);
			//AudioLoop(0);
		}
		//CallbackRunSystem();
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
	else if(Event.type == sf::Event::EventType::MouseButtonPressed)
	{
		//Make entity jump!
		//ent1->mBody->velocity.y= -80;

		//make player move
		ent1->mBody->velocity.x += 10;

	}
}
void UpdatePhysics(float deltaTime)
{
	int i;
	Entity* ent;
	forceRegistry.updateForces(deltaTime);
	for(i = 0;i < numEntities;i++ )
	{
		gEntities[i].PhysicsUpdate(deltaTime);
	}
	//Post Physics;
	UpdateCollision();
}

