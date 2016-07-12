#include <stdio.h>
#include <string.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "physics.h"
#include "audio.h"
#include "entity.h"
#include "player.h"
#include "graphics.h"
#include "globals.h"
#include "main.h"

const char *ANIMATION_IDLE_STR = "idle";
float gDeltaTime = (float)1/(float)gFrameRate;
int gMouseX = 0,gMouseY = 0;
char *test_files[] = {"sprites/Enemies3.png", 0};
Entity* ent1;
Entity test, test2;
Character *test_char;
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
				gCharacters[sounds_loaded]->LoadSounds(files);
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

	test_char = gCharacters[0];
	test_char->SetDimensions(CreateVec2D(127,127));
	test_char->SetCurrentAnimation(0);
	test_char->mCurrentSprite->SetFrameBB();
	test_char->mCurrentFrame = 0;
	test_char->SetVelocity(CreateVec2D(0,0));
	test_char->mBody.mass = 10;
	test_char->mBody.restitution = 17.5;
	test_char->mBody.staticFriction = .5;
	test_char->mBody.dynamicFriction = .5;
	//test.mBody.AddForce(CreateVec2D(6,0));

	test_char = gCharacters[1];
	test_char->SetDimensions(CreateVec2D(127,127));
	test_char->SetCurrentAnimation(0);
	test_char->mCurrentSprite->SetFrameBB();
	test_char->mCurrentFrame = 0;
	test_char->SetVelocity(CreateVec2D(0,0));
	test_char->setPosition(0,300);
	test_char->mBody.mass = 0;
	test_char->mBody.restitution = 1000;
	test_char->mBody.staticFriction = 1;
	test_char->mBody.dynamicFriction = 1;
	CallbackInitSystem();
	gClock.restart();

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

		//	Handle Physics -- This is so that there is a fixed update for physics

		accumulator += gClock.getElapsedTime().asSeconds() - frameStart; // Store Time of last frame
		frameStart = gClock.getElapsedTime().asSeconds();				// Store Time of new frame
		if(accumulator > 0.2f)
			accumulator = 0.2f;
		if(accumulator > gDeltaTime)
		{
			UpdatePhysics(frameStart);
			//std::cout << "Physics Update Goes here" << std::endl;
			accumulator -= gDeltaTime;
		}
		// Remeber to handle the discrete jump in time every 6th frames or so with linear interpolation! To: Jason

		for(i = 0; i < ASSETS_CHARACTERS; i++)
		{
			gCharacters[i]->Draw(gRenderWindow);

		}
		gRenderWindow.display();						//Displays whatever is drawn to the window
		while(gRenderWindow.pollEvent(gEvent))
		{
			HandleEvent(gEvent);
			//doubleg
			//AudioLoop(0);
		}
		//CallbackRunSystem();
	}
}
void UpdatePhysics(float deltaTime)
{
	/*
	int i;
	Vec2D someOfForces;
	Manifold *m = nullptr;
	//PrePhysics
	test.mBody.acceleration.x += (test.mBody.force.x / test.mBody.mass)*gDeltaTime;
	test.mBody.acceleration.y += (test.mBody.force.y / test.mBody.mass)*gDeltaTime;

	someOfForces.y = test.GetVelocity().y*(gClock.getElapsedTime().asSeconds() /deltaTime)+ ((test.mBody.mass*Gravity*(gClock.getElapsedTime().asSeconds() /deltaTime)));
	someOfForces.x = test.GetVelocity().x*(gClock.getElapsedTime().asSeconds() /deltaTime);
	test.SetVelocity(someOfForces);
	test.move(someOfForces.x,someOfForces.y);

	test.mBody.force.x = 0;
	//someOfForces.y = test2.GetVelocity().y*(gClock.getElapsedTime().asSeconds() /deltaTime) + ((Gravity*(gClock.getElapsedTime().asSeconds() /deltaTime)));
	//someOfForces.x = test2.GetVelocity().x*(gClock.getElapsedTime().asSeconds() /deltaTime);
	//test2.SetVelocity(someOfForces);
	//test2.move(someOfForces.x,someOfForces.y);
	
	//Post Physics;
	m = AABB(&test,&test2);
	if(m != nullptr)
	{
		CollisionResponse(&test,&test2,m);
		delete(m);
	}
	*/
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
	//double Garry test code
	else if((Event.type == sf::Event::EventType::KeyPressed) || (Event.type == sf::Event::EventType::KeyReleased))
	{
		test_char->HandleInput(Event);
	}
	// end
}

