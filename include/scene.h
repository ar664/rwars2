#ifndef _SCENE_H
#define _SCENE_H		
//#include "include\entity.h"
#include "include\Box2D\Box2D.h"
#include "include\components.h"




class Scene
{
public:
	Scene();
	~Scene();
	PlayerComponent			Players		[MAX_ENTITIES];
	void Draw(sf::RenderTarget &target);
	void Update();
	void RemoveEntity(Entity* ent);
private:
	Entity *mEntityList;
	b2World* m_world;
	std::vector<Entity*> EntitiesScheduledForRemoval;
};

#endif