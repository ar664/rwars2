#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "include\player.h"
//
//typedef enum
//{
//	COMPONENT_NONE,
//	COMPONENT_PLAYER = 1 << 0,
//	COMPONENT_RIGIDBODY = 1 << 1
//}ComponentMask;
//
//
//class Component
//{
//public:
//	Component(void);
//	~Component(void);
//
//};
//
//class PlayerComponent : public Component
//{
//private:
//	int					mHealth;
//	int					mState;
//	PlayerState			mCurrState; //flag of all states
//	
//	sf::Uint32			mAtkPressTime; //Time atk btn was pressed
//	sf::Uint32			mAtkHoldTime; // Amount of time atk btn was held
//	//currently using timers for state changes
//	//might change and use priority system
//	sf::Uint32			mLastStateChange;
//	int					mNextStateChange;
//public:
//
//	/**
//	 * @brief Handles movement, lane switch, jump, and attack inputs
//	 * @param Event, player input to handle event
//	 */
//	void HandleInput(sf::Event Event);
//	/**
//	 * @brief Changes player state if enough time has passed since last state change
//	 * @param state, state player must change to
//	 */
//	void ChangeState(PlayerState state);
//	/**
//	 * @brief Handles updating player status and state
//	 */
//	void Update();
//
//	/**
//	  *@brief Handles updating player actions / interaction with world
//	*/
//	void Think();
//
//	/**
//	 * @brief Handles Collision Events with other entities
//	 * @param other, entity that touched this player
//	 */
//	void Touch(Entity *other);
//
//	int GetState();
//};
//
//
//



#endif