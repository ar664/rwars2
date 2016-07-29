#include <time.h>
#include "Math.h"
#include "AI.h"

void AI::Block()
{
	ChangeState(AI_State_Block);
}


void AI::Attack()
{
	ChangeState(AI_State_Attack);
}

void AI::SpecialAttack()
{
	
}

void AI::Retreat()
{
	ChangeState(AI_State_Retreat);
}

bool AI::FindTarget(Entity *target)
{
	mTargetEnemy = target;
}

void AI::ChangeState(Enemy_States new_state)
{
	mState = new_state;
}
//choose a random state
void AI::UpdateRandState()
{
	int randNum = rand() % 30;
	
	if(randNum <= 10)
	{
		Attack();
	}
	else if(randNum <= 20)
	{
		Block();
	}
	else
	{
		Retreat();
	}

}