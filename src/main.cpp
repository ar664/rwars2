#include <stdio.h>
#include <string.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string.h>
#include "include\resourcemanager.h"
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
char *test_files[] = {"sprites/Crate.png", 0};
float deltaTime = 0;
Scene *gScene;
Entity test, test2;
GameState gGameState;

Sprite splashSprite;
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
	//CallbackInitSystem();
	
	gGameState =Playing;
	//LoadAssets();
	gGrid = new Grid(6,6,100);
	gScene = new Scene;
	gEntities[10].AddComponent(COMPONENT_PLAYER);
	gClock.restart();
	splashSprite = *LoadSprite("sprites/rwars_title_pixel.png");
	
	//Load Assets Testing
	ResourceManager::AddAsset("Assets.json");
	ResourceManager::LoadCharacterSpriteAssets("Gill-Sawfish",gEntities[10].mSpriteArray); 
	
	//Entity Testing
	gEntities[10].mCurrentSprite = gEntities[10].mSpriteArray[0];
	SetData(gEntities[10].mCurrentSprite,"BoxData/Gill-Sawfish");
	gEntities[10].SetBodyFixtures(gEntities[10].mCurrentSprite->mHurtBoxData);
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
	float frameStart = gClock.getElapsedTime().asSeconds();
	splashSprite.mSfSprite->move(0,WINDOW_HEIGHT/4);
	while(gRenderWindow.isOpen())
	{
		gRenderWindow.clear();		//Clears the window
		//For Fixed Physics updates
		float currentTime = gClock.getElapsedTime().asSeconds();
		accumulator += gClock.getElapsedTime().asSeconds() - frameStart; // Store Time of last frame
		frameStart = currentTime;				// Store Time of new frame
		if(accumulator > 0.2f)
			accumulator = 0.2f;
		
		while(gRenderWindow.pollEvent(gEvent))
					{
						HandleEvent(gEvent);
						//doubleg
						//AudioLoop(0);
					}

		switch(gGameState)
		{
			case Splash:
				gRenderWindow.draw(*splashSprite.mSfSprite);
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
					gGameState = Playing;
				break;
			case Playing:
					while(accumulator >= gDeltaTime)
					{
						deltaTime = accumulator/gDeltaTime;
						gScene->Update();
						accumulator -= gDeltaTime;
					}
					gEntities[10].Draw(gRenderWindow);
					gScene->DebugDraw(gRenderWindow);
					
					CallbackRunSystem();
				break;
			default:
				break;
			}
		gRenderWindow.display();
	}

}
void HandleEvent(sf::Event Event)
{
	//Close window
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

