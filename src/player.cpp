#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#include "include\components.h"
#include "player.h"
#include "globals.h"
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
void PlayerComponent::HandleInput()
{
	
	if (sf::Keyboard::isKeyPressed(KEY_MOVE_RIGHT))
	{
		gEntities[mID].mBody->GetBody()->SetLinearVelocity(b2Vec2(2,
			gEntities[mID].mBody->GetBody()->GetLinearVelocity().y));
		gEntities[mID].mIsFlipped = 1;
		gScene->Players[mID].ChangeState(P_State_Running);
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
		gEntities[mID].mBody->GetBody()->SetLinearVelocity(b2Vec2(-2,
			gEntities[mID].mBody->GetBody()->GetLinearVelocity().y));
		gEntities[mID].mIsFlipped = 0;
		gScene->Players[mID].ChangeState(P_State_Running);
	}
	if (sf::Keyboard::isKeyPressed(KEY_JUMP))
	{
		gEntities[mID].mBody->GetBody()->SetLinearVelocity(b2Vec2(gEntities[mID].mBody->GetBody()->GetLinearVelocity().x,
			-5));	
		gScene->Players[mID].ChangeState(P_State_Jump);
	}
	else
	{
		gScene->Players[mID].ChangeState(P_State_Neutral);
	}
	/*
	switch(sf::Keyboard::)
		{
		case KEY_JUMP :
			//ChangeState(P_State_Jump);
			break;
		case KEY_MOVE_UP :
			//ChangeState(P_State_Lane_Switch);
			break;
		case KEY_MOVE_DOWN :
			//ChangeState(P_State_Lane_Switch);
			break;
		case KEY_MOVE_LEFT :
			//move left
			gEntities[mID].mBody->SetVelocity(CreateVec2D(-3,0));
			printf("MoveLeft\n");
			break;
		case KEY_MOVE_RIGHT :
			//move right
			gEntities[mID].mBody->SetVelocity(CreateVec2D(3,0));
			printf("MoveRight\n");
			break;
		case KEY_ATTACK :
			//ChangeState(P_State_Charge_Atk);
			break;
		default:
			gEntities[mID].mBody->SetVelocity(CreateVec2D(0,0));
			break;
		}
		*/
}
int PlayerComponent::GetState()
{
	return mCurrState;
}

void PlayerComponent::ChangeState(PlayerState state)
{

	int delta		= 0;
	int atk_delta	= 0;
	int curr_time	= gClock.getElapsedTime().asMilliseconds();

	if(mPrevState == state)
	{
		return;
	}
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
	if(mCurrState != P_State_Neutral || state == P_State_Neutral)	return;

	if(state == P_State_Charge_Atk)
	{
		if(mAtkPressTime == 0)
		{
			mAtkPressTime = curr_time;
			return;
		}

		atk_delta		= curr_time - mAtkPressTime;
		std::cout<<"Atk delta is: " << atk_delta << std::endl;
		//hard coding 400 miliseconds as time btn needs to be held
		if(atk_delta < 400)
		{
			return;
		}
	}
	std:: cout <<"Last change time " << mLastStateChange << " Next change time " << mNextStateChange << std::endl;
	
	mAtkPressTime		= 0;

	mState				=state;
	mPrevState			= mCurrState;
	mCurrState			= state;


	mLastStateChange	= gClock.getElapsedTime().asMilliseconds();
	mNextStateChange	= 5000;//hard coded 500 miliseconds for now

	std::cout <<"Character State is " << mCurrState << std::endl;
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
