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
	void Draw(sf::RenderTarget &target);
	void Update();
private:
	Entity *mEntityList;
	b2World* m_world;
};


#endif