#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "globals.h"
#include "vectors.h"
#include "physics.h"

Grid* gGrid;

void ContactListener::BeginContact(b2Contact* contact) {
  
      //check if fixture A was a Entity
      void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
      if ( bodyUserData )
	  {
        static_cast<Entity*>( bodyUserData )->IncrementContact();
		static_cast<Entity*>( bodyUserData )->ChangeBodyColor(sf::Color(0,255,0,255));
	  }
      //check if fixture B was a Entity
      bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
      if ( bodyUserData )
	  {
        static_cast<Entity*>( bodyUserData )->IncrementContact();
		static_cast<Entity*>( bodyUserData )->ChangeBodyColor(sf::Color(0,255,0,255));
	  }
    };

void ContactListener::EndContact(b2Contact* contact) {
  
      //check if fixture A was a Entity
      void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
      if ( bodyUserData )
	  {
        static_cast<Entity*>( bodyUserData )->DecrementContact();
	  }
      //check if fixture B was a Entity
      bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
      if ( bodyUserData )
	  {
        static_cast<Entity*>( bodyUserData )->DecrementContact();

	  }
  
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
/**
* 
*
*/
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


/**
* 
*
*/
void UpdateCollision()
{
	//Loop Through Cells in Grid
	for(int i = 0; i < gGrid->getM_Cells().size();i++)
	{
		//printf("Mcells Size%d \n",gGrid->getM_Cells().size());
		int x = i % gGrid->getM_NumXCells();
		int y = i / gGrid->getM_NumXCells();

		Cell cell = gGrid->getM_Cells()[i];
		//Loop through Entities in Cell
		for(int j = 0;j < cell.entities.size();j++)
		{
			Entity* ent = cell.entities[j];
			//Do collision checks in the current cell than update
			if(cell.entities[j]->mBody != nullptr)
			{
					CheckCollision(ent,cell.entities,j+1);
					
					//Update Collision with neighbor cells
					if(x > 0)
					{	//Left
						CheckCollision(ent,gGrid->getCell(x-1,y)->entities,0);
						if(y > 0)
						{	
							//TopLeft
							CheckCollision(ent,gGrid->getCell(x-1,y-1)->entities,0);
						}
						if(y < gGrid->getM_NumYCells() -1)
						{
							//Bottom left
							CheckCollision(ent,gGrid->getCell(x-1,y+1)->entities,0);
						}
					}
					if(y > 0){
						//Top
						CheckCollision(ent,gGrid->getCell(x,y-1)->entities,0);
					}
				}
		
			}
	}
	
}
/**
* 
*
*/
void CheckCollision(Entity* ent, std::vector<Entity*>& ents, int startIndex)
{
	Manifold *m = nullptr;
		for (int i = 0; i < ents.size();i++)
		{
			if(ents[i] != ent){		//Make sure player isnt checking collision with self and ent doesnt have infinite mass
				//m = PolygonVsPolygon(ent->mBody,ents[i]->mBody);
				if(m != nullptr)
				{
					//ent->ResolveContact(m);
				}
			}
		}
	delete(m);
}
