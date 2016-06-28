#include <SFML/Graphics.hpp>

#include "player.h"
#include "globals.h"

Player::Player()
{
	this->mState				= PlayerState::Neutral;
	this->mLastStateChange		= 0;
	this->mNextStateChange		= 0;
}

void Player::Input(sf::Event Event)
{
	if(Event.type == sf::Event::EventType::KeyPressed)
	{
		switch(Event.key.code)
		{
		case sf::Keyboard::Space :
			ChangeState(Jump);
			break;
		case sf::Keyboard::W :
			ChangeState(Lane_Switch);
			break;
		case sf::Keyboard::S :
			ChangeState(Lane_Switch);
			break;
		case sf::Keyboard::A :
			//move left
			break;
		case sf::Keyboard::D :
			//move right
			break;
		case sf::Keyboard::Q :
			ChangeState(Attack);
			break;
		case sf::Keyboard::E :
			ChangeState(Attack);
			break;
		default:
			break;
		}
	}
}

void Player::ChangeState(PlayerState state)
{
	int delta = 0;

	delta = gClock.getElapsedTime().asMilliseconds() - mLastStateChange;
	mNextStateChange -= delta;
	
	if(mNextStateChange <= 0)
		mState = Neutral;
	//can only change states when we are in neutral state
	if(mState != Neutral)	return;

	mState = state;
	mLastStateChange = gClock.getElapsedTime().asMilliseconds();
	mNextStateChange = 500;//hard coded 500 miliseconds for now
}
//incomplete. Created to set up state changes
void Player::Update()
{
	if(this->GetVelocity().y < 0)
	{
		this->ChangeState(Fall);
	}
}
//needs to be completed
//only changes states so far
void Player::Touch(Entity *other)
{
	//needs a team check so ally doesn't hurt 
	this->ChangeState(Hurt);
}