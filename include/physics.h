#ifndef __PHYSICS_H
#define __PHYSICS_H
#include <vector>
#include "entity.h"
#include "shape.h"
#include "Box2D/Box2D.h"

const float Gravity_constant = 6.3;
const float Damping_constant = .99;	//Acts like wind resistance

struct Polygon;
/**
* This Class overiddes some of Box2d's functions to define behaviors
*	On contact and at the end of contact
*/
class ContactListener :public b2ContactListener
{
public:
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
    // Get the first fixture in this contact
    b2Fixture* GetFixtureA();
  
    // Get the second fixture in this contact
    b2Fixture* GetFixtureB();


};


/**
*This struct stores information about 2 entities that collided and is used to 
*physics updates
*/
/*
struct Manifold
{
	RigidBody* A;
	RigidBody* B;
	float penetration;		//How much A is penetrating B.... *wink* *wink*
	Vec2D normal;			//Vector along the normal
	Vec2D contacts[2];
	int contact_count;

	void Solve();

};
*/
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


void	UpdatePhysics(float deltaTime);
void	UpdateCollision();
void	CheckCollision(Entity* ent, std::vector<Entity*>& ents, int startIndex);



#endif
