#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "entity.h"

/**
 *	Different player states depending on player input and interaction with different players & entities
 *	can be used as a flag for multiple states
 */
typedef enum PlayerState
{
	Neutral = 0,
	Fall = 2,
	Attack = 4,
	Hurt = 8,
	Lane_Switch = 16,
	Jump = 32,
	Running = 64,
};

/**
 * The Player class that handles in game inputs and state changes
 *
 *	States:
 *		mState				-	The current state Player is in
 *		mLastStateChange	-	The last sf time State was changed
 *		mNextStateChange	-	The next sf time State returns to Neutral and can be changed
 */


class Character : public Entity
{
private:
	int					mHealth;
	int					mState;

	sf::Uint32			mLastStateChange;
	sf::Uint32			mNextStateChange;
public:
	Character();

	/**
	 * @brief Handles movement, lane switch, jump, and attack inputs
	 * @param Event, player input to handle event
	 */
	void Input(sf::Event Event);
	/**
	 * @brief Changes player state if enough time has passed since last state change
	 * @param state, state player must change to
	 */
	void ChangeState(PlayerState state);
	/**
	 * @brief Handles updating player status and state
	 */
	void Update();

	/**
	  *@brief Handles updating player actions / interaction with world
	*/
	void Think();

	/**
	 * @brief Handles Collision Events with other entities
	 * @param other, entity that touched this player
	 */
	void Touch(Entity *other);

};

extern Character **Characters;

void PlayerLoad(int num, char **sprites);

#endif