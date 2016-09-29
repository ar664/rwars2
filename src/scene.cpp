#include <stdio.h>
#include <string.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <random>
#include "globals.h"
#include "shape.h"
#include "physics.h"
#include "scene.h"

ContactListener CLInstance;

Scene::Scene()
{

	Entity* ent;
	Box *newBox;

	mEntityList = gEntities;
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
		,CreateVec2D(50,50),true);
	ent->SetBody(newBox);
}
Scene::~Scene(){
	delete m_world;
}

void Scene::RemoveEntity(Entity* ent)
{
	EntitiesScheduledForRemoval.push_back(ent);
}
/**
*@brief Used to draw the fixtures of a body(Assumes its a polygonshape)
*@param The render window
*/

void Scene::DebugDraw(sf::RenderTarget &target)
{
	b2PolygonShape* shape;
	sf::RectangleShape mShape(sf::Vector2f(0,0));

	for(int i = 0; i < MAX_ENTITIES;++i)
	{
		if(gEntities[i].mBody != nullptr)
		{
			
				for (b2Fixture* f = gEntities[i].mBody->GetBody()->GetFixtureList(); f; f = f->GetNext())
				{
					FixtureData *fixData;
					int sizex,sizey;
					fixData = (FixtureData*)f->GetUserData();
					shape = static_cast<b2PolygonShape*>(f->GetShape());
					sizex = shape->GetVertex(1).x*PPM - shape->GetVertex(0).x*PPM;
					sizey = shape->GetVertex(2).y*PPM - shape->GetVertex(1).y*PPM;
	
					mShape.setSize(sf::Vector2f(sizex,sizey));
					mShape.setOrigin(sizex/2,
						sizey/2);
					if(fixData != nullptr)
					{
						mShape.setFillColor(fixData->mColor);
					}
					else
					{
						mShape.setFillColor(sf::Color(255,0,0,100));
					}
					mShape.setRotation( gEntities[i].mBody->GetBody()->GetAngle()*(180/3.14159265359)  );
					mShape.setPosition( (f->GetBody()->GetPosition().x+shape->m_centroid.x)*PPM,
						(f->GetBody()->GetPosition().y+shape->m_centroid.y)*PPM);
					target.draw(mShape);
				}
			}
		}
}
void Scene::Draw(sf::RenderTarget &target)
{
	for(int i = 0; i < MAX_ENTITIES;++i)
	{
		
		if(gEntities[i].mBody != nullptr)
		{

			gEntities[i].Draw(target);
		}
		
	}
}
void Scene::Update()
{
	//Step through the physics simulation
	for(int i = 0; i < MAX_ENTITIES;++i)
	{
		if(gEntities[i].mInUse)
		{
			//Do something
		}
	}
	m_world->Step(gDeltaTime,8,6);
	//Process Entities Scheduled for removal
	std::vector<Entity*>::iterator it = EntitiesScheduledForRemoval.begin();
	std::vector<Entity*>::iterator end = EntitiesScheduledForRemoval.end();
	 for (; it!=end; ++it) {
			Entity* dyingEntity = *it;
			if(dyingEntity->mBody != nullptr)
			{
				b2Body* b = dyingEntity->mBody->GetBody();
				dyingEntity->Free();
				b->GetWorld()->DestroyBody(b);
			}
		}
  
	 //clear this list for next time
	EntitiesScheduledForRemoval.clear();
}

