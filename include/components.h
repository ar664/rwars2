#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "include\player.h"

typedef enum
{
	COMPONENT_NONE,
	COMPONENT_PLAYER = 1 << 0,
	COMPONENT_ENTITY = 1 << 1
}ComponentMask;
/**
*@brief This is the parent component class. Instead of using Inheritence, Entities will contain Components
*		to define what type of Entity they are
*/

class Component
{
public:
	Component(void);
	~Component(void);

};
/**
*@brief This is the PlayerComponent which hosts all the methods and variables that define a player
*/

class PlayerComponent : public Component
{
private:
	int					mHealth;
	int					mState;
	PlayerState			mPrevState;
	PlayerState			mCurrState; //flag of all states
	
	sf::Uint32			mAtkPressTime; //Time atk btn was pressed
	sf::Uint32			mAtkHoldTime; // Amount of time atk btn was held
	//currently using timers for state changes
	//might change and use priority system
	sf::Uint32			mLastStateChange;
	int					mNextStateChange;
public:
	int					mID;

	/**
	 * @brief Handles movement, lane switch, jump, and attack inputs
	 * @param Event, player input to handle event
	 */
	void HandleInput();
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
	//void Touch(Entity *other);

	int GetState();

};

/**
* This is the Controls for a player
*		Later i have to make it so that its loaded from a file
*/
static const sf::Keyboard::Key KEY_ATTACK		=	sf::Keyboard::E;
static const sf::Keyboard::Key KEY_JUMP			=	sf::Keyboard::Space;
static const sf::Keyboard::Key KEY_MOVE_LEFT		=	sf::Keyboard::A;
static const sf::Keyboard::Key KEY_MOVE_RIGHT	=	sf::Keyboard::D;
static const sf::Keyboard::Key KEY_MOVE_UP		=	sf::Keyboard::W;
static const sf::Keyboard::Key KEY_MOVE_DOWN		=	sf::Keyboard::S;





#endif