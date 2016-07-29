#ifndef _AI_H_
#define _AI_H_
#include "entity.h"
#include "physics.h"

#include "player.h"


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

	int					mLastStateChange;
	int					mNextStateChange;

public:	
	void				Block();
	void				Attack();
	void				Retreat();

	bool				FindTarget(Entity *target);

	void				ChangeState(Enemy_States new_state);
	void				UpdateRandState();	
	void				SpecialAttack();
};
#endif