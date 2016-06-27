#include <SFML\Graphics.hpp>
#include <iostream>
#include <cmath>
#include "globals.h"
#include "vectors.h"
#include "physics.h"


Manifold* AABB(Entity *ent1, Entity *ent2)
{
		/*
	Checks if the boxes intersect by comparing the boxes (x and widths) and (y and heights)
	*/
	if((ent1->getPosition().x + ent1->GetDimension().x) > ent2->getPosition().x 
		&& (ent2->getPosition().x + ent2->GetDimension().x) > ent1->getPosition().x
		&& (ent1->getPosition().y + ent1->GetDimension().y) > ent2->getPosition().y 
		&& (ent2->getPosition().y + ent2->GetDimension().y) > ent1->getPosition().y)
	{
		Vec2D center1 = CreateVec2D(ent1->getPosition().x + (ent1->GetDimension().x/2),
			ent1->getPosition().y + (ent1->GetDimension().y/2));
		Vec2D center2 = CreateVec2D(ent2->getPosition().x + (ent2->GetDimension().x/2),
			ent2->getPosition().y + (ent2->GetDimension().y/2));
		Manifold* m = new Manifold;
		m->A = ent1;
		m->B = ent2;
		int w,h,dx,dy,wy,hx;
		w = .5*(ent1->GetDimension().x + ent2->GetDimension().x);
		h = .5*(ent1->GetDimension().y + ent2->GetDimension().y);
		dx = center1.x - center2.x;
		dy = center1.y - center2.y;
		wy = w *dy;
		hx = h*dx;
			if(wy > hx)
				if(wy > -hx){
					//Top Collision
					m->penetration.y = (ent2->getPosition().y + ent2->GetDimension().y) - ent1->getPosition().y;
					m->normal.y = 1; 
				}
				else{
					//Right Collision
					m->penetration.x = (ent1->getPosition().x + ent1->GetDimension().x) - ent2->getPosition().x;
					m->normal.x = 1;
				}
			else
				if(wy > -hx)
				{
					//Left Collision
					m->penetration.x = (ent2->getPosition().x + ent2->GetDimension().x) - ent1->getPosition().x;
					m->normal.y = -1;
				}
				else{
					//Bottom Collisions
					m->penetration.y = (ent1->getPosition().y + ent1->GetDimension().y) - ent2->getPosition().y;
					m->normal.y = -1;
				}
		std::cout << "Collision Detected" << std::endl;
		return m;
	}
	return nullptr;
}

void FrictionResponse(Entity* ent1, Entity* ent2,Manifold* m)
{
	Vec2D rv,tangent,frictionImpulse;
	float jt,mu,dynamicFriction,invMass1,invMass2,velAlongNormal;

	if(ent1->mBody.mass == 0)					//Check for infinite Mass
		invMass1 = 0;
	else
		invMass1 = (1/ent1->mBody.mass);
	if(ent2->mBody.mass == 0)
		invMass2 = 0;
	else
		invMass2 = (1/ent2->mBody.mass);


	//Find resolution Vector
	Vec2DSub(rv,ent2->GetVelocity(),ent1->GetVelocity());
	//Solve for tangent Vector
	velAlongNormal = Vec2DDotProduct(rv,m->normal);
	tangent = CreateVec2D(rv.x - velAlongNormal * m->normal.x,rv.y - velAlongNormal * m->normal.y);

	Vec2DNormalize(&tangent);

	// Solve for magnitude to apply along the friction vector
	jt = -Vec2DDotProduct( rv, tangent);
	jt = jt / (invMass1 + invMass2);
	//Use pythag to solve for mu (Doing Coulumbs law)
	mu = sqrt(pow(ent1->mBody.staticFriction,(float)2) + pow(ent2->mBody.staticFriction,(float)2));

	float e = std::min(ent1->mBody.restitution,ent2->mBody.restitution);			//Restitution
	float j = -(1+e) * velAlongNormal;

	j = (j/(invMass1+invMass2));

	if(abs(jt) < j * mu)
		//Assuming the object is at rest, this code will execute
		frictionImpulse = CreateVec2D(tangent.x * jt,tangent.y * jt);
	else
	{
		dynamicFriction =sqrt(pow(ent1->mBody.dynamicFriction,(float)2) + pow(ent2->mBody.dynamicFriction,(float)2));
		frictionImpulse = CreateVec2D(-j*tangent.x*dynamicFriction,-j*tangent.y*dynamicFriction);
	}
	frictionImpulse.x = frictionImpulse.x /10;
	frictionImpulse.y = frictionImpulse.y /10;

	ent1->SetVelocity(
		CreateVec2D(ent1->GetVelocity().x - invMass1 *frictionImpulse.x,ent1->GetVelocity().y - invMass1 *frictionImpulse.y));
	ent2->SetVelocity(
		CreateVec2D(ent2->GetVelocity().x + invMass2 *frictionImpulse.x,ent2->GetVelocity().y + invMass2 *frictionImpulse.y));

}
int CollisionResponse(Entity* ent1,Entity *ent2,Manifold *m)
{
	Vec2D rv;

	Vec2DSub(rv,ent1->GetVelocity(),ent2->GetVelocity());

	float velAlongNormal = Vec2DDotProduct(rv,m->normal);
	float invMass1;
	float invMass2;
	//Check for infinite mass...Objects with Infinite Mass wont move, Good for platforms
	if(ent1->mBody.mass == 0)					
		invMass1 = 0;
	else
		invMass1 = (1/ent1->mBody.mass);
	if(ent2->mBody.mass == 0)
		invMass2 = 0;
	else
		invMass2 = (1/ent2->mBody.mass);
	
	//This is to make sure we dont resolve collisions if they will be resolved on the next frame anyways
	if(velAlongNormal > 0)
	{
		return 0;
	}

	float e = std::min(ent1->mBody.restitution,ent2->mBody.restitution);			//Restitution
	float j = -(1+e) * velAlongNormal;

	j = (j/(invMass1+invMass2));

	Vec2D impulse = CreateVec2D(m->normal.x * j,m->normal.y * j);
	Vec2D result1,result2;
	impulse.y = impulse.y/10;
	impulse.x = impulse.x/10;
	if(impulse.x == 0)
	{
		result1.x = 0;
		result2.x = 0;
	}
	else
	{
		result1.x = invMass1  * impulse.x;
		result2.x = invMass2  * impulse.x;
	}
	if(impulse.y == 0)
	{
		result1.y = 0;
		result2.y = 0;
	}
	else
	{
		result1.y = invMass1 * impulse.y;
		result2.y = invMass2 * impulse.y;
	}
	float mass_sum = ent1->mBody.mass + ent2->mBody.mass;
	float ratio = ent1->mBody.mass / mass_sum;
 	ent1->SetVelocity(CreateVec2D(ent1->GetVelocity().x + result1.x*ratio,
		ent1->GetVelocity().y + result1.y*ratio));
	
	ratio = ent2->mBody.mass / mass_sum;

	ent2->SetVelocity(CreateVec2D(ent2->GetVelocity().x - result2.x*ratio,
		ent2->GetVelocity().y - result2.y*ratio));
	
	//Solve for friction
		FrictionResponse(ent1,ent2,m);
	//Solve for floating point errors and such(Linear Projection) due to gravity
	const float slop = .1;
	const float percent= .6f;
	Vec2D correction = CreateVec2D(std::max(m->penetration.x-slop , 0.0f)/(invMass1 + invMass2)*percent*m->normal.x,
		std::max(m->penetration.y-slop,0.0f)/(invMass1 + invMass2)*percent*m->normal.y) ;
	ent1->setPosition(ent1->getPosition().x - (correction.x*invMass1),ent1->getPosition().y + (correction.y*invMass1));
	ent2->setPosition(ent2->getPosition().x + (correction.x*invMass2),ent2->getPosition().y + (correction.y*invMass2));
	
	return 1;
	}
/**
 * @brief Constructer for Grid
 * @author	Jason
*/

Grid::Grid(int width, int height, int cellSize):
	m_width(width),
	m_height(height),
	m_cellSize(cellSize){
		m_numXCells = ceil((float) m_width / cellSize);
		m_numYCells = ceil((float) m_height / cellSize);

		//Allocate cells
		m_cells.resize(m_numYCells * m_numXCells);
}

Grid::~Grid(void)
{
}

/**
 * @brief returns a pointer to a cell
 * @author	Jason
*/

Cell* Grid::getCell(int x, int y)
{
	if(x < 0) 
	{
		printf("Cell does not exist\n");
		x = 0;
	}
	if(x >= m_numXCells)
		x = m_numXCells - 1;
	if(y < 0) 
	{
		printf("Cell does not exist\n");
		y = 0;
	}
	if(y >= m_numYCells) 
		y = m_numYCells - 1;
	return &m_cells[y * m_numXCells + x];
}
/**
 *** @brief returns a pointer to a cell based on entities location
 *** @author	Jason
*/
Cell* Grid::getCell(Vec2D ent)
{
	int cellX = (int)(ent.x/m_cellSize);
	int cellY = (int)(ent.y/m_cellSize);

	return getCell(cellX,cellY);
}

/**
 * @brief Add entity to a cell
 * @author	Jason
*/

void Grid::addEntity(Entity* ent)
{
	Cell* cell = getCell(CreateVec2D(ent->getPosition().x,ent->getPosition().y));
	cell->entities.push_back(ent);
	ent->SetCell(cell);
	ent->mCellIndex = cell->entities.size() - 1;
	printf("Added to new cell,CellSize:%d",cell->entities.size());
}

/**
 * @brief Add entity to a specified cell
 * @author	Jason
 */

void Grid::addEntity(Entity* ent,Cell* cell)
{
	cell->entities.push_back(ent);
	ent->SetCell(cell);
	ent->mCellIndex = cell->entities.size() - 1;
	printf("Added to new cell,CellSize:%d",cell->entities.size());

}
/**
 * @brief Remove entity to a cell
 * @author	Jason
*/
void Grid::removeEntityFromCell(Entity* ent)
{
	std::vector<Entity*> & ents = ent->GetCell()->entities;
	
	//Vector swap, Need to check for 0 or else get error because you cant pop_back from an empty vector
	ents[ent->mCellIndex] = ents.back();					//Setting one vectors index to the last element in the other vector
	ents.pop_back();									//Deletes last element
	//update vector index
	if(ent->mCellIndex < ents.size())
	{
		ents[ent->mCellIndex]->mCellIndex = ent->mCellIndex;
	}
	
	//Set index of ent to 1
	ent->mCellIndex = -1;
	ent->SetCell(nullptr);
	printf("Removed from old cell\n");
}
/**
 * @brief Returns m_cells
 * @author	Jason
*/
std::vector<Cell> Grid::getM_Cells()
{
	return m_cells;
}

int Grid::getM_NumXCells()
{
	return m_numXCells;
}
int Grid::getM_NumYCells()
{
	
	return m_numYCells;
}

int Grid::getM_CellSize()
{
	return m_cellSize;
}

int Grid::getM_Width()
{
	return m_width;
}

int Grid::getM_Height()
{
	return m_height;
}

void RigidBody::AddForce(Vec2D amount)
{
	Vec2DAdd(force,force,amount);
}