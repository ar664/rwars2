#ifndef __PHYSICS_H
#define __PHYSICS_H
#include <vector>
#include "entity.h"

const float Gravity = .2;

/**
*This struct stores information about 2 entities that collided and is used to 
*physics updates
*/
struct Manifold
{
	Entity* A;
	Entity* B;
	Vec2D penetration;		//How much A is penetrating B.... *wink* *wink*
	Vec2D normal;			//Vector along the normal
};

struct Cell{
	
	std::vector<Entity*> entities;
	//Entity* EntityList[264];
};

class Grid
{
public:
	Grid(int width, int height, int cellSize);
	~Grid();
	Cell* getCell(int x, int y);				//Get cell based on coordinates
	Cell* getCell(Vec2D ent);				//Overload- get cell based on Entity
	void addEntity(Entity* ent);				//Add entity to cell
	void addEntity(Entity* ent,Cell* cell);		//Add ent to specified cell
	void removeEntityFromCell(Entity* ent);		//remove entity from cell
	std::vector<Cell> getM_Cells();				//Return m_cells
	int getM_NumXCells();
	int getM_NumYCells();
	int getM_CellSize();
	int getM_Width();
	int getM_Height();

private:
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
};


Manifold* AABB(Entity *ent1, Entity *ent2);
int CollisionResponse(Entity* ent1,Entity *ent2,Manifold* m);
void FrictionResponse(Entity* ent1, Entity* ent2,Manifold* m);
int SweptAABB(Entity *ent1, Entity *ent2, float& normalx, float& normaly);
void UpdatePhysics(float deltaTime);
void UpdateCollision();
void CheckCollision(Entity* ent, std::vector<Entity*>& ents, int startIndex);


#endif