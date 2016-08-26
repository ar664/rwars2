#ifndef _SCENE_H
#define _SCENE_H		
//#include "include\entity.h"
#include "include\Box2D\Box2D.h"
#include "include\components.h"


/**
* This class is used to manage Entities and components.
*/


class Scene
{
public:
	Scene();
	~Scene();
	PlayerComponent			Players		[MAX_ENTITIES];
	void Draw(sf::RenderTarget &target);
	void Update();
	void RemoveEntity(Entity* ent);
	b2World*		GetWorld(){return m_world;};
private:
	Entity *mEntityList;
	b2World* m_world;
	std::vector<Entity*> EntitiesScheduledForRemoval;
};

#endif