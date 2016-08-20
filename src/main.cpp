#include <stdio.h>
#include <string.h>
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
#include "include\scene.h"
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
Scene *gScene;
Entity* ent1, *ent2,*ent3 ,*ent4,*ent5,*ent6;
Entity test, test2;
GameState gGameState;
//Character *test_char;
Polygon *p,*p2,*p3;

int main(int argc,char *argv[])
{
	Start();
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
				//PlayerLoad(characters_loaded, files);
				//temp = (char*) malloc(sizeof(char)*256);
				//files = (char**) malloc(sizeof(char*)*16);
				//i = 0; j = 0;
				//characters_loaded++;
				//continue;
			}
			
			if(sounds_loaded < ASSETS_CHARACTERS)
			{
				//gCharacters[sounds_loaded]->LoadSounds(files);
				//temp = (char*) malloc(sizeof(char)*256);
				//files = (char**) malloc(sizeof(char*)*16);
				//i = 0; j = 0;
				//sounds_loaded++;
				//continue;
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
void Start()
{
	Init_Graphics(WINDOW_WIDTH,WINDOW_HEIGHT,"RWARS");
	SpriteListInit();
	EntitySystemInit();
	gGameState = Playing;
	//LoadAssets();
	gGrid = new Grid(6,6,100);
	gScene = new Scene;

	
	p = new Polygon();
	ent1 = CreateEntity();	
	ent1->mBody = new RigidBody(p);
	p->SetOrientation(0);

	ent1->LoadSprites(test_files);
	ent1->SetCurrentAnimation(0);
	ent1->SetPosition(CreateVec2D(0,300));
	ent1->SetVelocity(CreateVec2D(3,0));
	ent1->mBody->SetColor(10,100,255);
	p->SetBox(50,50);
	ent1->mBody->shape->ComputeMass(1);
	ent1->mBody->zConstraint = 1;
	ent1->AddComponent(COMPONENT_PLAYER);
	ent1->AddComponent(COMPONENT_PLAYER);


	p2 = new Polygon();
	ent2 = CreateEntity();	
	ent2->mBody = new RigidBody(p2);
	p2->SetOrientation(0);
	
	ent2->LoadSprites(test_files);
	ent2->SetCurrentAnimation(0);
	ent2->SetPosition(CreateVec2D(300,450));
	ent2->mBody->SetColor(255,0,0);
	ent2->mBody->zConstraint = 1;
	p2->SetBox(50,50);
	ent2->mBody->shape->ComputeMass(2);

	
	
	p3 = new Polygon();
	ent3 = CreateEntity();	
	ent3->mBody = new RigidBody(p3);
	p3->SetOrientation(0);

	ent3->LoadSprites(test_files);
	ent3->SetCurrentAnimation(0);
	ent3->mCurrentFrame = 0;
	ent3->mBody->SetPosition(CreateVec2D(0,500));
	ent3->SetVelocity(CreateVec2D(0,0));
	ent3->mBody->SetColor(0,255,0);
	p3->SetBox(500,50);
	ent3->mBody->shape->ComputeMass(0);


	forceRegistry.add(ent1->mBody,&gravity);
	forceRegistry.add(ent2->mBody,&gravity);

	/*
	test_char = gCharacters[0];
	test_char->SetDimensions(CreateVec2D(127,127));
	test_char->SetCurrentAnimation(0);
	test_char->mCurrentSprite->SetFrameBB();
	test_char->mCurrentFrame = 0;
	test_char->SetVelocity(CreateVec2D(0,0));
	test_char->setPosition(300,300);
	test_char->mBody->mass = 10;
	test_char->mBody->restitution = 17.5;
	test_char->mBody->staticFriction = .5;
	test_char->mBody->dynamicFriction = .5;
	//test.mBody.AddForce(CreateVec2D(6,0));
	*/
	CallbackInitSystem();
	gClock.restart();
	while(!IsExiting())
		Loop();
	gRenderWindow.close();
}
bool IsExiting()
{
	if(gGameState == Exiting)
		return true;
	else
		return false;
}
void Loop()
{
	float accumulator = 0;				//For Physics Update
	int i;
	gClock.restart();
	float frameStart = gClock.getElapsedTime().asSeconds();
	
	while(gRenderWindow.isOpen())
		{
			gRenderWindow.clear();		//Clears the window
			gRenderWindow.pollEvent(gEvent);
			//HandleEvent(gEvent);
			if(gRenderWindow.hasFocus())
				HandleInput();
			//	Handle Physics -- This is so that there is a fixed update for physics
	
			accumulator += gClock.getElapsedTime().asSeconds() - frameStart; // Store Time of last frame
			frameStart = gClock.getElapsedTime().asSeconds();				// Store Time of new frame
			if(accumulator > 0.2f)
				accumulator = 0.2f;
			if(accumulator > gDeltaTime)
			{

				deltaTime = gClock.getElapsedTime().asSeconds() /frameStart;
				UpdatePhysics(deltaTime);
				//std::cout << "Physics Update Goes here" << std::endl;
				accumulator -= gDeltaTime;
			}
	
			for(i = 0; i < ASSETS_CHARACTERS; i++)
			{
				//gCharacters[i]->Update();
				//gCharacters[i]->Draw(gRenderWindow);
	
			}
			// Remeber to handle the discrete jump in time every 6th frames or so with linear interpolation! To: Jason
			ent1->mBody->shape->Draw(gRenderWindow);
			ent2->mBody->shape->Draw(gRenderWindow);
			ent3->mBody->shape->Draw(gRenderWindow);
	
			gRenderWindow.display();						//Displays whatever is drawn to the window
	
			//CallbackRunSystem();
		}

	
}
void HandleEvent(sf::Event Event)
{
	//Close window
	//gScene->Players[ent1->mID].HandleInput(Event);
	switch(Event.type)
	{
	case sf::Event::Closed:
		gRenderWindow.close();
		exit(1);
		break;
	default:
		break;
	}
}
void HandleInput()
{
	int i;
	for(i = 0;i < numEntities;i++ )
	{
		if(gEntities[i].HasComponent(COMPONENT_PLAYER))
		{
			gScene->Players[gEntities[i].mID].HandleInput();
		}
	}
}
void UpdatePhysics(float deltaTime)
{
	int i;
	Entity* ent;
	forceRegistry.updateForces(deltaTime);
	for(i = 0;i < numEntities;i++ )
	{
		if(gEntities[i].mBody->isAwake == 1)
			gEntities[i].PhysicsUpdate(deltaTime);
	}
	//Post Physics;
	UpdateCollision();
}

