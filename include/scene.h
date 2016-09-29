#ifndef _SCENE_H
#define _SCENE_H		
//#include "include\entity.h"
#include <Box2D/Box2D.h>
#include "entity.h"


/**
* This class is used to manage Entities and components.
*/


class Scene
{
public:
	Scene();
	~Scene();
	Entity *mEntityList;
	

	void Draw(sf::RenderTarget &target);
	void DebugDraw(sf::RenderTarget &target);
	void Update();
	void RemoveEntity(Entity* ent);
	b2World*		GetWorld(){return m_world;};
	std::vector<Entity*> EntitiesScheduledForRemoval;
private:
	b2World* m_world;
};
void DestroyScene();
#endif
