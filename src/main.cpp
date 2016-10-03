#include <stdio.h>
#include <string.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string.h>
#include "resourcemanager.h"
#include "physics.h"
#include "audio.h"
#include "entity.h"
#include "scene.h"
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
	gClock.restart();
	splashSprite = *LoadSprite("sprites/rwars_title_pixel.png");
	
	//Load Assets Testing
	ResourceManager::AddAsset("Assets.json");
	ResourceManager::LoadCharacterSpriteAssets("Gill-Sawfish",gEntities[10].mSpriteArray); 
	
	//Entity Testing
	SetData(gEntities[10].mSpriteArray[0],"BoxData/Gill-Sawfish");
	SetData(gEntities[10].mSpriteArray[1],"BoxData/Gill-Sawfish");
	SetData(gEntities[10].mSpriteArray[2],"BoxData/Gill-Sawfish");
	
	gEntities[10].mCurrentSprite = gEntities[10].mSpriteArray[0];
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
			case Exiting:
				delete gScene;
				gRenderWindow.close();
				exit(1);
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
		gGameState = Exiting;
		break;
	default:
		break;
	}
}

