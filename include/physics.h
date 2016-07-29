#ifndef __PHYSICS_H
#define __PHYSICS_H
#include <vector>
#include "entity.h"

const float Gravity_constant = 6.3;
const float Damping_constant = .99;	//Acts like wind resistance

/**
*This struct stores information about 2 entities that collided and is used to 
*physics updates
*/
struct Manifold
{
	Entity* A;
	Entity* B;
	float penetration;		//How much A is penetrating B.... *wink* *wink*
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
float	CalculateSeperatingVelocity(Manifold *m);
int		CollisionResponseAABBvsAABB(Entity* ent1,Entity *ent2,Manifold* m);
void	ResolveFriction(Manifold* m);
int		SweptAABB(Entity *ent1, Entity *ent2, float& normalx, float& normaly);
void	UpdatePhysics(float deltaTime);
void	UpdateCollision();
void	CheckCollision(Entity* ent, std::vector<Entity*>& ents, int startIndex);


void	CollisionResponseCircleToCircle(Manifold *m,RigidBody* b1,RigidBody* b2);
void	CollisionResponseCircleToPolygon(Manifold *m,RigidBody* b1,RigidBody* b2);
void	CollisionResponsePolygonToCircle(Manifold *m,RigidBody* b1,RigidBody* b2);
void	CollisionResponsePolygonToPolygon(Manifold *m,RigidBody* b1,RigidBody* b2);


#endif