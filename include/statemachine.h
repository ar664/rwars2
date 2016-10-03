#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <queue>  
#include <typeinfo>
#include "fault.h"

class EventData
{
public:
	//virtual ~EventData() {}
	//XALLOCATOR
};

typedef EventData NoEventData;

class StateMachine;
class SpriteMachine;

/// @brief Abstract state base class that all states inherit from.
class StateBase
{
public:
	/// Called by the state machine engine to execute a state action. If a guard condition
	/// exists and it evaluates to false, the state action will not execute. 
	/// @param[in] sm - A state machine instance. 
	/// @param[in] data - The event data. 
	virtual void InvokeStateAction(StateMachine* sm, const EventData* data) const = 0;
};

/// @brief StateAction takes three template arguments: A state machine class,
/// a state function event data type (derived from EventData) and a state machine 
/// member function pointer.  
template <class SM, class Data, void (SM::*Func)(const Data*)>
class StateAction : public StateBase
{
public:
	/// @see StateBase::InvokeStateAction
	virtual void InvokeStateAction(StateMachine* sm, const EventData* data) const 
	{
		// Downcast the state machine and event data to the correct derived type
		SM* derivedSM = static_cast<SM*>(sm);
		
		// If this check fails, there is a mismatch between the STATE_DECLARE 
		// event data type and the data type being sent to the state function. 
		// For instance, given the following state defintion:
		//    STATE_DECLARE(MyStateMachine, MyStateFunction, MyEventData)
		// The following internal event transition is valid:
		//    InternalEvent(ST_MY_STATE_FUNCTION, new MyEventData());
		// This next internal event is not valid and causes the assert to fail:
		//    InternalEvent(ST_MY_STATE_FUNCTION, new OtherEventData());
		const Data* derivedData = reinterpret_cast<const Data*>(data);
		ASSERT_TRUE(derivedData != NULL);

		// Call the state function
		(derivedSM->*Func)(derivedData);
	}
};

/// @brief Abstract guard base class that all guards classes inherit from.
class GuardBase
{
public:
	/// Called by the state machine engine to execute a guard condition action. If guard
	/// condition evaluates to TRUE the state action is executed. If FALSE, no state transition
	/// is performed.
	/// @param[in] sm - A state machine instance. 
	/// @param[in] data - The event data. 
	/// @return Returns TRUE if no guard condition or the guard condition evaluates to TRUE.
	virtual bool InvokeGuardCondition(StateMachine* sm, const EventData* data) const = 0;
};

/// @brief GuardCondition takes three template arguments: A state machine class,
/// a state function event data type (derived from EventData) and a state machine 
/// member function pointer. 
template <class SM, class Data, bool (SM::*Func)(const Data*)>
class GuardCondition : public GuardBase
{
public:
	virtual bool InvokeGuardCondition(StateMachine* sm, const EventData* data) const 
	{
		SM* derivedSM = static_cast<SM*>(sm);		
		const Data* derivedData = reinterpret_cast<const Data*>(data);
		ASSERT_TRUE(derivedData != NULL);

		// Call the guard function
		return (derivedSM->*Func)(derivedData);
	}
};

/// @brief Abstract entry base class that all entry classes inherit from.
class EntryBase
{
public:
	/// Called by the state machine engine to execute a state entry action. Called when
	/// entering a state. 
	/// @param[in] sm - A state machine instance. 
	/// @param[in] data - The event data.
	virtual void InvokeEntryAction(StateMachine* sm, const EventData* data) const = 0;
};

/// @brief EntryAction takes three template arguments: A state machine class,
/// a state function event data type (derived from EventData) and a state machine 
/// member function pointer.  
template <class SM, class Data, void (SM::*Func)(const Data*)>
class EntryAction : public EntryBase
{
public:
	virtual void InvokeEntryAction(StateMachine* sm, const EventData* data) const
	{
		SM* derivedSM = static_cast<SM*>(sm);
		const Data* derivedData = reinterpret_cast<const Data*>(data);
		ASSERT_TRUE(derivedData != NULL);

		// Call the entry function
		(derivedSM->*Func)(derivedData);
	}
};

/// @brief Abstract exit base class that all exit classes inherit from.
class ExitBase
{
public:
	/// Called by the state machine engine to execute a state exit action. Called when
	/// leaving a state. 
	/// @param[in] sm - A state machine instance. 
	virtual void InvokeExitAction(StateMachine* sm) const = 0;
};

/// @brief ExitAction takes two template arguments: A state machine class and
/// a state machine member function pointer.   
template <class SM, void (SM::*Func)(void)>
class ExitAction : public ExitBase
{
public:
	virtual void InvokeExitAction(StateMachine* sm) const
	{
		SM* derivedSM = static_cast<SM*>(sm);

		// Call the exit function
		(derivedSM->*Func)();
	}
};

/// @brief A structure to hold a single row within the state map. 
struct StateMapRow
{
	const StateBase* const State;
};

/// @brief A structure to hold a single row within the extended state map. 
struct StateMapRowEx
{
	const StateBase* const State;
	const GuardBase* const Guard;
	const EntryBase* const Entry;
	const ExitBase* const Exit;
};

/// @brief StateMachine implements a software-based state machine. 
class StateMachine 
{
public:
	enum { EVENT_IGNORED = 0xFE, CANNOT_HAPPEN };

	///	Constructor.
	///	@param[in] maxStates - the maximum number of state machine states.
	StateMachine(unsigned char maxStates, unsigned char initialState = 0);

	virtual ~StateMachine() {}

	/// Gets the current state machine state.
	/// @return Current state machine state.
	unsigned char GetCurrentState() { return m_currentState; }
	
protected:
	/// External state machine event.
	/// @param[in] newState - the state machine state to transition to.
	/// @param[in] pData - the event data sent to the state.
	void ExternalEvent(unsigned char newState, const EventData* pData = NULL);

	/// Internal state machine event. These events are generated while executing
	///	within a state machine state.
	/// @param[in] newState - the state machine state to transition to.
	/// @param[in] pData - the event data sent to the state.
	void InternalEvent(unsigned char newState, const EventData* pData = NULL);
	
private:
	/// The maximum number of state machine states.
	const unsigned char MAX_STATES;

	/// The current state machine state.
	unsigned char m_currentState;

	/// The new state the state machine has yet to transition to. 
	unsigned char m_newState;

	/// Set to TRUE when an event is generated.
	bool m_eventGenerated;

	/// The state event data pointer.
	const EventData* m_pEventData;

	/// Gets the state map as defined in the derived class. The BEGIN_STATE_MAP,
	/// STATE_MAP_ENTRY and END_STATE_MAP macros are used to assist in creating the
	/// map. A state machine only needs to return a state map using either GetStateMap()  
	/// or GetStateMapEx() but not both. 
	/// @return An array of StateMapRow pointers with the array size MAX_STATES or
	/// NULL if the state machine uses the GetStateMapEx(). 
	virtual const StateMapRow* GetStateMap() = 0;

	/// Gets the extended state map as defined in the derived class. The BEGIN_STATE_MAP_EX,
	/// STATE_MAP_ENTRY_EX, STATE_MAP_ENTRY_ALL_EX, and END_STATE_MAP_EX macros are used to 
	/// assist in creating the map. A state machine only needs to return a state map using 
	/// either GetStateMap() or GetStateMapEx() but not both. 
	/// @return An array of StateMapRowEx pointers with the array size MAX_STATES or 
	/// NULL if the state machine uses the GetStateMap().
	virtual const StateMapRowEx* GetStateMapEx() = 0;

	/// Set a new current state.
	/// @param[in] newState - the new state.
	void SetCurrentState(unsigned char newState) { m_currentState = newState; }

	/// State machine engine that executes the external event and, optionally, all 
	/// internal events generated during state execution.
	void StateEngine(void); 	
	void StateEngine(const StateMapRow* const pStateMap);
	void StateEngine(const StateMapRowEx* const pStateMapEx);
};

#define STATE_DECLARE(stateMachine, stateName, eventData) \
	void ST_##stateName(const eventData*); \
	StateAction<stateMachine, eventData, &stateMachine::ST_##stateName> stateName;
	
#define STATE_DEFINE(stateMachine, stateName, eventData) \
	void stateMachine::ST_##stateName(const eventData* data)
		
#define GUARD_DECLARE(stateMachine, guardName, eventData) \
	bool GD_##guardName(const eventData*); \
	GuardCondition<stateMachine, eventData, &stateMachine::GD_##guardName> guardName;
	
#define GUARD_DEFINE(stateMachine, guardName, eventData) \
	bool stateMachine::GD_##guardName(const eventData* data)

#define ENTRY_DECLARE(stateMachine, entryName, eventData) \
	void EN_##entryName(const eventData*); \
	EntryAction<stateMachine, eventData, &stateMachine::EN_##entryName> entryName;
	
#define ENTRY_DEFINE(stateMachine, entryName, eventData) \
	void stateMachine::EN_##entryName(const eventData* data)

#define EXIT_DECLARE(stateMachine, exitName) \
	void EX_##exitName(void); \
	ExitAction<stateMachine, &stateMachine::EX_##exitName> exitName;
	
#define EXIT_DEFINE(stateMachine, exitName) \
	void stateMachine::EX_##exitName(void)

#define BEGIN_TRANSITION_MAP \
    const unsigned char TRANSITIONS[] = {\

#define TRANSITION_MAP_ENTRY(entry)\
    entry,

#define END_TRANSITION_MAP(data) \
    };\
    ExternalEvent(TRANSITIONS[GetCurrentState()], data); \
	C_ASSERT((sizeof(TRANSITIONS)/sizeof(unsigned char)) == ST_MAX_STATES); 
	
#define BEGIN_STATE_MAP \
	private:\
	virtual const StateMapRowEx* GetStateMapEx() { return NULL; }\
	virtual const StateMapRow* GetStateMap() {\
		static const StateMapRow STATE_MAP[] = { 

#define STATE_MAP_ENTRY(stateName)\
	stateName,

#define END_STATE_MAP \
    }; \
	C_ASSERT((sizeof(STATE_MAP)/sizeof(StateMapRow)) == ST_MAX_STATES); \
	return &STATE_MAP[0]; }

#define BEGIN_STATE_MAP_EX \
	private:\
	virtual const StateMapRow* GetStateMap() { return NULL; }\
	virtual const StateMapRowEx* GetStateMapEx() {\
		static const StateMapRowEx STATE_MAP[] = { 

#define STATE_MAP_ENTRY_EX(stateName)\
	{ stateName, 0, 0, 0 },

#define STATE_MAP_ENTRY_ALL_EX(stateName, guardName, entryName, exitName)\
	{ stateName, guardName, entryName, exitName },

#define END_STATE_MAP_EX \
    }; \
	C_ASSERT((sizeof(STATE_MAP)/sizeof(StateMapRowEx)) == ST_MAX_STATES); \
   return &STATE_MAP[0]; }
class MovementData : public EventData
{
public:
	int mID;
	int mTouchingGround;
	sf::Keyboard::Key key;
	MovementData();
	int mJumped;
};
class CharacterStateMachine : public StateMachine
{
public:
    CharacterStateMachine();
	int					mID;
    // External events taken by this state machine
	void IdleF(MovementData* data);
	void MoveF(MovementData* data);
	void JumpF(MovementData* data);

private:

    // State enumeration order must match the order of state method entries
    // in the state map.
    enum States
    {
        ST_IDLE,
        ST_MOVE,
        ST_JUMP,
        ST_MAX_STATES
    };
	STATE_DECLARE(CharacterStateMachine,	Idle, MovementData);
	STATE_DECLARE(CharacterStateMachine,	Move, MovementData);
	STATE_DECLARE(CharacterStateMachine,	Jump, MovementData);
	//ENTRY_DECLARE(CharacterStateMachine, SwitchSprite, MovementData)
	//EXIT_DECLARE(CharacterStateMachine,	ExitJump);

    // State map to define state object order. Each state map entry defines a
    // state object.
    //BEGIN_STATE_MAP
    //    STATE_MAP_ENTRY(&Idle)
    //    STATE_MAP_ENTRY(&Move)
    //    STATE_MAP_ENTRY(&Jump)
    //END_STATE_MAP    

	BEGIN_STATE_MAP_EX
		STATE_MAP_ENTRY_EX(&Idle)
		STATE_MAP_ENTRY_EX(&Move)
		STATE_MAP_ENTRY_EX(&Jump)
	END_STATE_MAP_EX
};

class SpriteMachine : public StateMachine
{
public:
	SpriteMachine(int id);
	int			mID;
	// External events taken by this state machine
	void IdleS();
	void MoveS();
	void JumpS();

private:

    // State enumeration order must match the order of state method entries
    // in the state map.
    enum States
    {
        ST_IDLE,
        ST_MOVE,
        ST_JUMP,
        ST_MAX_STATES
    };
	STATE_DECLARE(SpriteMachine,	Idle, NoEventData);
	STATE_DECLARE(SpriteMachine,	Move, NoEventData);
	STATE_DECLARE(SpriteMachine,	Jump, NoEventData);

	BEGIN_STATE_MAP_EX
		STATE_MAP_ENTRY_EX(&Idle)
		STATE_MAP_ENTRY_EX(&Move)
		STATE_MAP_ENTRY_EX(&Jump)
	END_STATE_MAP_EX

	

};

#endif
