#include <stdio.h>
#include <string.h>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <random>
#include "include\globals.h"
#include "include\shape.h"
#include "include\scene.h"

Scene::Scene()
{
	mEntityList = gEntities;
	memset(Players,0,sizeof(PlayerComponent)*MAX_ENTITIES);
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xDist(0,500);
	std::uniform_real_distribution<float> yDist(0,400);
	std::uniform_real_distribution<float> size(30,50);

	const int numBoxes = 10;
	
	b2Vec2 gravity(0,9.8);
	m_world = new b2World(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0/PPM,500.0f/PPM);
	b2Body *groundBody = m_world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(900/PPM,10/PPM);
	groundBody->CreateFixture(&groundBox,0);
	Entity* ent;
	Box *newBox;
	for(int i = 0; i< numBoxes;i++)
	{
		ent = CreateEntity();
		newBox = new Box();
		newBox->init(m_world,CreateVec2D(xDist(randGenerator),yDist(randGenerator))
			,CreateVec2D(50,20));
		ent->mBody = newBox;

	}
	Polygon* poly = new Polygon();
	sf::Vector2f points[6];
	points[0] = sf::Vector2f(0,0);
	points[1] = sf::Vector2f(10,0);
	points[2] = sf::Vector2f(20,10);
	points[3] = sf::Vector2f(20,30);
	points[4] = sf::Vector2f(0,30);
	points[5] = sf::Vector2f(-10,20);
	poly->SetPoints(points,6);
	poly->init(m_world,CreateVec2D(100,0),CreateVec2D(0,0));
	ent = CreateEntity();
	ent->mBody = poly;
}
Scene::~Scene(){


}

void Scene::Draw(sf::RenderTarget &target)
{
	sf::Transformable t;
	for(int i = 0; i < MAX_ENTITIES;++i)
	{
		if(gEntities[i].mBody != nullptr)
			target.draw(*gEntities[i].mBody->GetShape());
	}

}

void Scene::Update()
{
	m_world->Step(gDeltaTime,8,6);
	for(int i = 0; i < MAX_ENTITIES;++i)
	{
		if(gEntities[i].mBody != nullptr)
			gEntities[i].Update(gDeltaTime);
	}

}

