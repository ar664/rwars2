#include <SFML\Graphics.hpp>
#include <iostream>
#include "vectors.h"
#include "physics.h"


int AABB(Entity *ent1, Entity *ent2)
{
	float colNormalx;
	float colNormaly;
	Vec2D rv;
		/*
	Checks if the boxes intersect by comparing the boxes (x and widths) and (y and heights)
	*/
	if((ent1->getPosition().x + ent1->GetDimension().x) > ent2->getPosition().x 
		&& (ent2->getPosition().x + ent2->GetDimension().x) > ent1->getPosition().x
		&& (ent1->getPosition().y + ent1->GetDimension().y) > ent2->getPosition().y 
		&& (ent2->getPosition().y + ent2->GetDimension().y) > ent1->getPosition().y)
	{
		std::cout << "Collision Detected" << std::endl;
		Vec2D center1 = CreateVec2D(ent1->getPosition().x + (ent1->GetDimension().x/2),
			ent1->getPosition().y + (ent1->GetDimension().y/2));
		Vec2D center2 = CreateVec2D(ent2->getPosition().x + (ent2->GetDimension().x/2),
			ent2->getPosition().y + (ent2->GetDimension().y/2));
		Vec2D normal;
		Vec2DSub(normal,center1,center2);

		if(ent1->GetVelocity().x == 0 &&
			ent1->GetVelocity().y == 0 && 
			ent2->GetVelocity().x == 0 &&
			ent2->GetVelocity().y == 0 )
		{


		}
		Vec2DNormalize(&normal);
		Vec2DSub(rv,ent1->GetVelocity(),ent2->GetVelocity());

		float velAlongNormal = Vec2DDotProduct(rv,normal);
		float invMass1;
		float invMass2;
		if(ent1->mass == 0)					//Check for infinite Mass
			invMass1 = 0;
		else
			invMass1 = (1/ent1->mass);
		if(ent2->mass == 0)
			invMass2 = 0;
		else
			invMass2 = (1/ent2->mass);
		if(velAlongNormal >= 0)
		{

			Vec2D penetration = CreateVec2D(
				(ent1->getPosition().x + ent1->GetDimension().x) - (ent2->getPosition().x + ent2->GetDimension().x),
			 (ent1->getPosition().y + ent1->GetDimension().y) - (ent2->getPosition().y + ent2->GetDimension().y));
			const float k_slop = 0.1f;
			const float percent= .2f;
			Vec2D correction = CreateVec2D((std::max(penetration.x - k_slop,0.0f)/(invMass1 + invMass2)*percent*normal.x),
				(std::max(penetration.y - k_slop,0.0f)/(invMass1 + invMass2)*percent*normal.y)) ;
			ent1->setPosition(ent1->getPosition().x + (correction.x*invMass1),ent1->getPosition().y + (correction.y*invMass1));
			ent2->setPosition(ent2->getPosition().x - (correction.x*invMass1),ent2->getPosition().y - (correction.y*invMass1));
			return 0;
		}
		float e = 10;
		float j = -(1+e) * velAlongNormal;

		j = (j/(invMass1+invMass2));

		Vec2D impulse = CreateVec2D(normal.x * j,normal.y * j);
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
 		ent1->SetVelocity(CreateVec2D(ent1->GetVelocity().x + result1.x,
			ent1->GetVelocity().y + result1.y));
		ent2->SetVelocity(CreateVec2D(ent2->GetVelocity().x - result2.x,
			ent2->GetVelocity().y - result2.y));
		//Solve for floating point errors and such(Linear Projection)

		Vec2D penetration = CreateVec2D((
			ent1->getPosition().x + ent1->GetDimension().x) - (ent2->getPosition().x + ent2->GetDimension().x),
			 (ent1->getPosition().y + ent1->GetDimension().y) - (ent2->getPosition().y + ent2->GetDimension().y));
		const float k_slop = 0.01;
		const float percent= .2f;
		Vec2D correction = CreateVec2D((std::max(penetration.x - k_slop,0.0f)/(invMass1 + invMass2))*percent*normal.x,
			(std::max(penetration.y - k_slop,0.0f)/(invMass1 + invMass2))*percent*normal.y) ;
		ent1->setPosition(ent1->getPosition().x + correction.x,ent1->getPosition().y + correction.y);
		ent2->setPosition(ent2->getPosition().x - correction.x,ent2->getPosition().y - correction.y);
		return 1;
	}
	return 0;

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
	ent->cellIndex = cell->entities.size() - 1;
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
	ent->cellIndex = cell->entities.size() - 1;
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
	ents[ent->cellIndex] = ents.back();					//Setting one vectors index to the last element in the other vector
	ents.pop_back();									//Deletes last element
	//update vector index
	if(ent->cellIndex < ents.size())
	{
		ents[ent->cellIndex]->cellIndex = ent->cellIndex;
	}
	
	//Set index of ent to 1
	ent->cellIndex = -1;
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

