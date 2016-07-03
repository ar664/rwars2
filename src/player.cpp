#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#include "player.h"
#include "globals.h"

Character **gCharacters = (Character**) malloc(sizeof(Character*)*ASSETS_CHARACTERS);

Character::Character()
{
	mHealth					= 0;

	mState					= PlayerState::P_State_Neutral;
	mCurrState				= PlayerState::P_State_Neutral;

	mAtkPressTime			= 0;
	
	mLastStateChange		= 0;
	mNextStateChange		= 0;
}

void Character::HandleInput(sf::Event Event)
{
	if(Event.type == sf::Event::EventType::KeyPressed)
	{
		switch(Event.key.code)
		{
		case KEY_JUMP :
			ChangeState(P_State_Jump);
			break;
		case KEY_MOVE_UP :
			ChangeState(P_State_Lane_Switch);
			break;
		case KEY_MOVE_DOWN :
			ChangeState(P_State_Lane_Switch);
			break;
		case KEY_MOVE_LEFT :
			//move left
			break;
		case KEY_MOVE_RIGHT :
			//move right
			break;
		case KEY_ATTACK :
			ChangeState(P_State_Charge_Atk);
			break;
		default:
			break;
		}
	}
	if(Event.type == sf::Event::EventType::KeyReleased)
	{
		switch(Event.key.code)
		{
			case KEY_ATTACK :
				ChangeState(P_State_Attack);
			break;
		}
	}
}
int Character::GetState()
{
	return mCurrState;
}

void Character::ChangeState(PlayerState state)
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
	if(mCurrState != P_State_Neutral)	return;

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

	mState				|=state;
	mCurrState			= state;

	this->SetCurrentAnimation( (int)log((double)mCurrState) / log(2.0));

	mLastStateChange	= gClock.getElapsedTime().asMilliseconds();
	mNextStateChange	= 500;//hard coded 500 miliseconds for now

	std::cout <<"Character State is " << mCurrState << std::endl;
}
//incomplete. Created to set up state changes
void Character::Update()
{
	if(this->GetVelocity().y < 0)
	{
		ChangeState(P_State_Fall);
	}
}
//needs to be completed
//only changes states so far
void Character::Touch(Entity *other)
{
	//needs a team check so ally doesn't hurt 
	this->ChangeState(P_State_Hurt);
}

void Character::Think()
{
	//Yeah I'm not doing this right now.
}

Character * PlayerLoad(int num, char** sprites)
{
	Character *player;
	if(num > ASSETS_CHARACTERS || !gCharacters)
	{
		return NULL;
	}

	gCharacters[num] = new Character;
	player = gCharacters[num];
	
	player->LoadSprites(sprites);

	return player;
}
