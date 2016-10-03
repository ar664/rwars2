#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "statemachine.h"
#include "player.h"
#include "globals.h"
CharacterStateMachine movement;

Player::Player()
{
	mLastStateChange = 0;
}
void Player::HandleInput()
{

}


void Player::ChangeState(PlayerState state)
{

	int delta		= 0;
	int atk_delta	= 0;
	int curr_time	= gClock.getElapsedTime().asMilliseconds();

	delta = curr_time - mLastStateChange;
	mNextStateChange -= delta;
	
	mState |= state;
	
	if(mNextStateChange <= 0)
	{
		//testing purpose
		//should be fixed later to manually unset state
		mState = 0;
		mCurrState = P_State_Neutral;
	}
	//can only change states when we are in neutral state
	//if(mCurrState != P_State_Neutral || state == P_State_Neutral)	return;
	if(mCurrState == state)
		return;
	
	//@TODO Need to find a better spot to put this line of code becuase its currently running
	//Every frame which could slow down the game
	
	if(state == P_State_Charge_Atk)
	{
		if(mAtkPressTime == 0)
		{
			mAtkPressTime = curr_time;
			return;
		}

		atk_delta		= curr_time - mAtkPressTime;
		//std::cout<<"Atk delta is: " << atk_delta << std::endl;
		//hard coding 400 miliseconds as time btn needs to be held
		if(atk_delta < 400)
		{
			return;
		}
	}
	//std:: cout <<"Last change time " << mLastStateChange << " Next change time " << mNextStateChange << std::endl;
	
	mAtkPressTime		= 0;

	mState				=state;
	mPrevState			= mCurrState;
	mCurrState			= state;


	mLastStateChange	= gClock.getElapsedTime().asMilliseconds();
	mNextStateChange	= 5000;//hard coded 500 miliseconds for now

	//std::cout <<"Character State is " << mCurrState << std::endl;

}

