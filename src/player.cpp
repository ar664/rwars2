
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "include\statemachine.h"
#include "player.h"
#include "globals.h"
CharacterStateMachine movement;
//
//
//Character **gCharacters = (Character**) malloc(sizeof(Character*)*ASSETS_CHARACTERS);
//
//Character::Character()
//{
//	mHealth					= 0;
//
//	mState					= PlayerState::P_State_Neutral;
//	mCurrState				= PlayerState::P_State_Neutral;
//
//	mAtkPressTime			= 0;
//	
//	mLastStateChange		= 0;
//	mNextStateChange		= 0;
//
//	mInUse = 1;
//	//mBody = new RigidBody();
//}
//

Player::Player()
{

}
void Player::HandleInput()
{
	/*
	mMoveData->mTouchingGround = gEntities[mID].mBody->mTouchingGround;
	if (sf::Keyboard::isKeyPressed(KEY_MOVE_RIGHT))
	{
		mMoveData->key = KEY_MOVE_RIGHT;
		movement.MoveF(mMoveData);
		
	}	
	else if (sf::Keyboard::isKeyPressed(KEY_MOVE_UP))
	{
			//ChangeState(P_State_Lane_Switch);
	}
	else if (sf::Keyboard::isKeyPressed(KEY_MOVE_DOWN))
	{
			//ChangeState(P_State_Lane_Switch);
	}
	else if (sf::Keyboard::isKeyPressed(KEY_MOVE_LEFT))
	{
		mMoveData->key = KEY_MOVE_LEFT;
		movement.MoveF(mMoveData);
	}
	else
	{
		mMoveData->key = sf::Keyboard::Unknown;
		if(mMoveData->mTouchingGround == 1)
		{
			movement.IdleF(mMoveData);
		}
		
	}
	if (sf::Keyboard::isKeyPressed(KEY_JUMP))
	{
		gEntities[mID].mBody->GetBody()->SetLinearVelocity(b2Vec2(gEntities[mID].mBody->GetBody()->GetLinearVelocity().x,
			-5));	
 		mMoveData->mJumped = 1;
		mMoveData->mTouchingGround = 0;
		gEntities[mID].mBody->mTouchingGround = 0;
		movement.JumpF(mMoveData);
	}
	*/
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
////incomplete. Created to set up state changes
//void Character::Update()
//{
//	unsigned int animation;
//	ChangeState(P_State_Neutral);
//	printf("Player State is %i \n", mCurrState);
//	if(this->GetVelocity().y < 0)
//	{
//	}
//	animation = (unsigned int) (log((double)mCurrState) / log(2.0));
//	this->SetCurrentAnimation( animation);
//}
////needs to be completed
////only changes states so far
//void Character::Touch(Entity *other)
//{
//	//needs a team check so ally doesn't hurt 
//	this->ChangeState(P_State_Hurt);
//}
//
//void Character::Think()
//{
//	//Yeah I'm not doing this right now.
//}
//
//Character * PlayerLoad(int num, char** sprites)
//{
//	Character *player;
//	if(num > ASSETS_CHARACTERS || !gCharacters)
//	{
//		return NULL;
//	}
//
//	gCharacters[num] = new Character;
//	player = gCharacters[num];
//	
//	player->LoadSprites(sprites);
//
//	return player;
//}
