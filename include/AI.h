#include "entity.h"
#include "physics.h"

enum Enemy_States
{
	AI_State_Neutral		= 0,
	AI_State_Attack			= 1,
	AI_State_Block			= 2,
	AI_State_Retreat		= 4
};

class AI : public Entity
{
private: 
	int					mState;
	bool				mFoundTarget;
	Entity				*mTargetEnemy;

public:	
	void				Block();
	void				Attack();
	void				Retreat();

	bool				FindTarget(Entity *target);

};