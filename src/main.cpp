#include <stdio.h>
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
Entity* ent1, *ent2,*ent3;
Entity test, test2;
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
	gGrid = new Grid(6,6,100);

	ent1 = CreateEntity();	
	ent1->LoadSprites(test_files);
	ent1->SetDimensions(CreateVec2D(127,127));
	ent1->SetCurrentAnimation(0);
	ent1->mCurrentSprite->SetFrameBB();
	ent1->mCurrentFrame = 1;
	ent1->setPosition(0,0);
	ent1->SetVelocity(CreateVec2D(2,0));
	ent1->mBody.mass = 10;
	ent1->mBody.restitution = 17.5;
	ent1->mBody.staticFriction = .0001;
	ent1->mBody.dynamicFriction = .0002;
	//test.mBody.AddForce(CreateVec2D(6,0));

	ent2 =CreateEntity();
	ent2->LoadSprites(test_files);
	ent2->SetDimensions(CreateVec2D(127,127));
	ent2->SetCurrentAnimation(0);
	ent2->mCurrentSprite->SetFrameBB();
	ent2->mCurrentFrame = 1;
	ent2->SetVelocity(CreateVec2D(0,0));
	ent2->setPosition(0,400);
	ent2->mBody.mass = 0;
	ent2->mBody.restitution = 20;
	ent2->mBody.staticFriction =0.5;
	ent2->mBody.dynamicFriction = 0.5;

	ent3 = CreateEntity();	
	ent3->LoadSprites(test_files);
	ent3->SetDimensions(CreateVec2D(127,127));
	ent3->SetCurrentAnimation(0);
	ent3->mCurrentSprite->SetFrameBB();
	ent3->mCurrentFrame = 1;
	ent3->SetVelocity(CreateVec2D(0,0));
	ent3->setPosition(0,273-100);
	ent3->mBody.mass =50;
	ent3->mBody.restitution = 10;
	ent3->mBody.staticFriction = .0003;
	ent3->mBody.dynamicFriction = 0;

	LoadAssets();
	CallbackInitSystem();
	gClock.restart();

}
void Loop()
{
	float accumulator = 0;				//For Physics Update
	//Entitys First FrameBB
	sf::Image image,image2;
	image.create(ent1->mCurrentSprite->mFrameBB[ent1->mCurrentFrame].width,
	ent1->mCurrentSprite->mFrameBB[ent1->mCurrentFrame].height,sf::Color::Blue);

	image2.create(ent2->mCurrentSprite->mFrameBB[ent2->mCurrentFrame].width,
	ent2->mCurrentSprite->mFrameBB[ent2->mCurrentFrame].height,sf::Color::Blue);

	sf::Texture *texture = new sf::Texture;
	texture->loadFromImage(image);
	sf::Sprite *sprite = new sf::Sprite;
	sprite->setTexture(*texture,1);
	sprite->setPosition(gMouseX,gMouseY);

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
			UpdatePhysics(frameStart);
			std::cout << "Physics Update Goes here" << std::endl;
			accumulator -= gDeltaTime;
		}
		// Remeber to handle the discrete jump in time every 6th frames or so with linear interpolation! To: Jason
		
		gRenderWindow.draw(*sprite);
		gRenderWindow.draw(*sprite2);
		ent1->Draw(gRenderWindow);
		ent2->Draw(gRenderWindow);
		ent3->Draw(gRenderWindow);
		sprite->setPosition(ent1->getPosition().x,ent1->getPosition().y);
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
}
void UpdatePhysics(float deltaTime)
{
	int i;
	Entity* ent;
	Vec2D someOfForces;
	for(i = 0;i < numEntities;i++ )
	{
		ent = &gEntities[i];
		//PrePhysics
		ent->mBody.acceleration.x += (ent->mBody.force.x / ent->mBody.mass)*gDeltaTime;
		ent->mBody.acceleration.y += (ent->mBody.force.y / ent->mBody.mass)*gDeltaTime;

		someOfForces.y = ent->GetVelocity().y*(gClock.getElapsedTime().asSeconds() /deltaTime)+ ((ent->mBody.mass*Gravity*(gClock.getElapsedTime().asSeconds() /deltaTime)));
		someOfForces.x = ent->GetVelocity().x*(gClock.getElapsedTime().asSeconds() /deltaTime);
		ent->SetVelocity(someOfForces);
		ent->move(someOfForces.x,someOfForces.y);

		ent->mBody.force.x = 0;

		//Grid Detection via Cells
		Cell *newCell = gGrid->getCell(CreateVec2D(ent->getPosition().x,ent->getPosition().y));
		if(newCell != ent->GetCell())
			{
				if(ent->GetCell() == NULL)
			{
				gGrid->addEntity(ent,newCell);
			}
			else if(newCell != ent->GetCell())
			{
				gGrid->removeEntityFromCell(ent);
				gGrid->addEntity(ent,newCell);
			}
		}
	}
	//Post Physics;
	UpdateCollision();
}

