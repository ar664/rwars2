#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "entity.h"
/**
 *	Different player states depending on player input and interaction with different players & entities
 *	can be used as a flag for multiple states
 */
typedef enum PlayerState
{
	P_State_Neutral = 0,
	P_State_Attack = 2,
	P_State_Jump = 4,
	P_State_Running = 8,
	P_State_Death = 16,

	P_State_Charge_Atk = 0x20,
	P_State_Hurt = 0x10,
	P_State_Lane_Switch = 0x40,
	P_State_Fall = 0x80

};

/**
 * The Player class that handles in game inputs and state changes
 *
 *	States:
 *		mHealth				-	Amount of life this character has left
 *		mState				-	flag of all States applicable to this character
 *		mCurrState			-	Current state of player that is treated with highest priority
 *		mLastStateChange	-	The last sf time State was changed
 *		mNextStateChange	-	The next sf time State returns to Neutral and can be changed
 */

class Character : public Entity
{
private:
	int					mHealth;
	int					mState;
	PlayerState			mCurrState; //flag of all states
	
	sf::Uint32			mAtkPressTime; //Time atk btn was pressed
	sf::Uint32			mAtkHoldTime; // Amount of time atk btn was held
	//currently using timers for state changes
	//might change and use priority system
	sf::Uint32			mLastStateChange;
	int					mNextStateChange;
public:
	Character();

	/**
	 * @brief Handles movement, lane switch, jump, and attack inputs
	 * @param Event, player input to handle event
	 */
	void HandleInput(sf::Event Event);
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

	int GetState();

};

extern Character **Characters;

Character * PlayerLoad(int num, char **sprites);

#define KEY_ATTACK		sf::Keyboard::E
#define KEY_JUMP		sf::Keyboard::Space
#define KEY_MOVE_LEFT	sf::Keyboard::A
#define KEY_MOVE_RIGHT	sf::Keyboard::D
#define KEY_MOVE_UP		sf::Keyboard::W
#define	KEY_MOVE_DOWN	sf::Keyboard::S

#endif