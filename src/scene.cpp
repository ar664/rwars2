#include <stdio.h>
#include <string.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <random>
#include "include\globals.h"
#include "include\shape.h"
#include "include\physics.h"
#include "include\scene.h"

ContactListener CLInstance;

Scene::Scene()
{

	Entity* ent;
	Box *newBox;

	mEntityList = gEntities;
	memset(Players,0,sizeof(PlayerComponent)*MAX_ENTITIES);
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xDist(0,500);
	std::uniform_real_distribution<float> yDist(0,400);
	std::uniform_real_distribution<float> size(30,50);

	const int numBoxes = 10;
	
	//Create the world
	b2Vec2 gravity(0,GRAVITY);
	m_world = new b2World(gravity);
	m_world->SetContactListener(&CLInstance);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0/PPM,500.0f/PPM);
	b2Body *groundBody = m_world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(900/PPM,10/PPM);
	groundBody->CreateFixture(&groundBox,0);
	for(int i = 0; i< numBoxes;i++)
	{
		ent = CreateEntity();
		newBox = new Box();
		newBox->init(m_world,CreateVec2D(xDist(randGenerator),yDist(randGenerator))
			,CreateVec2D(50,20),false);
		ent->SetBody(newBox);

	}
	ent = CreateEntity();
	newBox = new Box();
	newBox->init(m_world,CreateVec2D(100,100)
		,CreateVec2D(114,114),true);
	ent->SetBody(newBox);
	//SetData(ent->mSpriteArray,"Gill-Sawfish");
	ent->mCurrentSprite = LoadSprite("sprites/Gill-Sawfish/idle.png");
}
Scene::~Scene(){
	m_world->~b2World();

}
void Scene::RemoveEntity(Entity* ent)
{
	EntitiesScheduledForRemoval.push_back(ent);
}

void Scene::Draw(sf::RenderTarget &target)
{
	for(int i = 0; i < MAX_ENTITIES;++i)
	{
		if(i == 10)
		{
			sf::RectangleShape mShape;
			Vec2D dim;
			
			mShape.setSize(sf::Vector2f(gEntities[10].mBody->GetDimensions().x,
				gEntities[10].mBody->GetDimensions().y));
			mShape.setOrigin(gEntities[10].mBody->GetDimensions().x/2,
				gEntities[10].mBody->GetDimensions().y/2);
			mShape.setFillColor(sf::Color(255,0,0,255));
			
			mShape.setRotation( gEntities[10].mBody->GetBody()->GetAngle()*(180/3.14159265359)  );
			mShape.setPosition( gEntities[10].mBody->GetBody()->GetPosition().x*PPM, gEntities[10].mBody->GetBody()->GetPosition().y*PPM);
			gEntities[i].mBody->SetShape(&mShape);
			
			target.draw(*gEntities[i].mBody->GetShape());
			gEntities[10].Draw(target);
		}
		else if(gEntities[i].mBody != nullptr)
		{
			gEntities[i].mBody->GetShape()->setRotation( gEntities[i].mBody->GetBody()->GetAngle()*(180/3.14159265359)  );
			gEntities[i].mBody->GetShape()->setPosition( gEntities[i].mBody->GetBody()->GetPosition().x*PPM, 
				gEntities[i].mBody->GetBody()->GetPosition().y*PPM);
		
			target.draw(*gEntities[i].mBody->GetShape());
		}
	}

}

void Scene::Update()
{
	//Step through the physics simulation
	for(int i = 0; i < MAX_ENTITIES;++i)
	{
		if(gEntities[i].mBody != nullptr)
		{
			if(gEntities[i].mMask & COMPONENT_PLAYER == COMPONENT_PLAYER)
			{
				gScene->Players[gEntities[i].mID].HandleInput();
			}
			gEntities[i].Update(gDeltaTime);
		}
	}
	m_world->Step(gDeltaTime,8,6);
	//Process Entities Scheduled for removal
	std::vector<Entity*>::iterator it = EntitiesScheduledForRemoval.begin();
	std::vector<Entity*>::iterator end = EntitiesScheduledForRemoval.end();
	 for (; it!=end; ++it) {
			Entity* dyingEntity = *it;
			dyingEntity->Free();
		}
  
	 //clear this list for next time
	EntitiesScheduledForRemoval.clear();
}

