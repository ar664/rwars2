#ifndef _ENTITY_H
#define _ENTITY_H
#include <unordered_map>
#include "vectors.h"
#include "sprite.h"
struct RigidBody;
struct Cell;
class Grid;

struct RigidBody
{
	// We are assuming for now that all Rigidbody shapes are rectangles
	float		mass;
	float		restitution;
};

typedef class Entity: public sf::Transformable
{
private:
	int					mSpeed;
	Vec2D				velocity;
	Vec2D				dimension;
	Cell*				mCell;
	Animation*			curAnim;	
public:
	int					mCellIndex;
	int					mFrameNum;
	
	RigidBody			mBody;

	Sprite				*rSprite;
	std::unordered_map<char*,Animation*>		animations;	
	void Draw(sf::RenderTarget &target);
	//Getters
	Cell*			GetCell();
	Grid*			GetGrid();
	Vec2D			GetDimension();
	Vec2D			GetVelocity();
	//Setters
	void			SetVelocity(Vec2D vec);
	void			SetCurrentAnimation(Animation* anim);
	void			SetCell(Cell* cell);
	void			SetPosition(Vec2D vec);
	void			SetDimensions(Vec2D vec);
};


#endif

