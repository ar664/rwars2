#ifndef _SCENE_H
#define _SCENE_H		
#include "include\entity.h"
#include "include\components.h"



class Scene
{
public:
	Scene(void);
	PlayerComponent			Players		[MAX_ENTITIES];


	unsigned int AddEntity();
	void RemoveEntity(unsigned int entity);

};

#endif