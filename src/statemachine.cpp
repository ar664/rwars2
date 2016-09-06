#include <assert.h>
#include "include\globals.h"
#include "include\statemachine.h"
 

using namespace std;

//----------------------------------------------------------------------------
// StateMachine
//----------------------------------------------------------------------------
StateMachine::StateMachine(unsigned char maxStates, unsigned char initialState) :
	MAX_STATES(maxStates),
	m_currentState(initialState),
	m_newState(false),
	m_eventGenerated(false),
	m_pEventData(NULL)
{
	ASSERT_TRUE(MAX_STATES < EVENT_IGNORED);
}  

//----------------------------------------------------------------------------
// ExternalEvent
//----------------------------------------------------------------------------
void StateMachine::ExternalEvent(unsigned char newState, const EventData* pData)
{
	// If we are supposed to ignore this event
	if (newState == EVENT_IGNORED)
	{

	}
	else
	{
		// TODO - capture software lock here for thread-safety if necessary

		// Generate the event
		InternalEvent(newState, pData);

		// Execute the state engine. This function call will only return
		// when all state machine events are processed.
		StateEngine();

		// TODO - release software lock here 
	}
}

//----------------------------------------------------------------------------
// InternalEvent
//----------------------------------------------------------------------------
void StateMachine::InternalEvent(unsigned char newState, const EventData* pData)
{
	if (pData == NULL)
		pData = new NoEventData();

	m_pEventData = pData;
	m_eventGenerated = true;
	m_newState = newState;
}

//----------------------------------------------------------------------------
// StateEngine
//----------------------------------------------------------------------------
void StateMachine::StateEngine(void)
{
	const StateMapRow* pStateMap = GetStateMap();
	if (pStateMap != NULL)
		StateEngine(pStateMap);
	else
	{
		const StateMapRowEx* pStateMapEx = GetStateMapEx();
		if (pStateMapEx != NULL)
			StateEngine(pStateMapEx);
		else
			ASSERT();
	}
}

//----------------------------------------------------------------------------
// StateEngine
//----------------------------------------------------------------------------
void StateMachine::StateEngine(const StateMapRow* const pStateMap)
{	

	// While events are being generated keep executing states
	while (m_eventGenerated)
	{
		// Error check that the new state is valid before proceeding
		ASSERT_TRUE(m_newState < MAX_STATES);

		// Get the pointer from the state map
		const StateBase* state = pStateMap[m_newState].State;


		// Event used up, reset the flag
		m_eventGenerated = false;

		// Switch to the new current state
		SetCurrentState(m_newState);

		// Execute the state action passing in event data
		ASSERT_TRUE(state != NULL);
		state->InvokeStateAction(this, m_pEventData);
		m_pEventData = NULL;
		// If event data was used, then delete it
	}
}

//----------------------------------------------------------------------------
// StateEngine
//----------------------------------------------------------------------------
void StateMachine::StateEngine(const StateMapRowEx* const pStateMapEx)
{

	// While events are being generated keep executing states
	while (m_eventGenerated)
	{
		// Error check that the new state is valid before proceeding
		ASSERT_TRUE(m_newState < MAX_STATES);

		// Get the pointers from the state map
		const StateBase* state = pStateMapEx[m_newState].State;
		const GuardBase* guard = pStateMapEx[m_newState].Guard;
		const EntryBase* entry = pStateMapEx[m_newState].Entry;
		const ExitBase* exit = pStateMapEx[m_currentState].Exit;

		// Event used up, reset the flag
		m_eventGenerated = false;

		// Execute the guard condition
		bool guardResult = true;
		if (guard != NULL)
			guardResult = guard->InvokeGuardCondition(this, m_pEventData);

		// If the guard condition succeeds
		if (guardResult == true)
		{
			// Transitioning to a new state?
			if (m_newState != m_currentState)
			{
				// Execute the state exit action on current state before switching to new state
				if (exit != NULL)
					exit->InvokeExitAction(this);

				// Execute the state entry action on the new state
				if (entry != NULL)
					entry->InvokeEntryAction(this, m_pEventData);

				// Ensure exit/entry actions didn't call InternalEvent by accident 
				ASSERT_TRUE(m_eventGenerated == false);
			}

			// Switch to the new current state
			SetCurrentState(m_newState);

			// Execute the state action passing in event data
			ASSERT_TRUE(state != NULL);
			state->InvokeStateAction(this,m_pEventData);
		}
			m_pEventData = NULL;
	}
}

MovementData::MovementData()
{
	memset(this,0,sizeof(MovementData));
}
MovementMachine::MovementMachine() : StateMachine(ST_MAX_STATES)
{
}
 
void MovementMachine::MoveF(MovementData* n)
{
    BEGIN_TRANSITION_MAP                      // - Current State -
        TRANSITION_MAP_ENTRY (ST_MOVE)		  // ST_Idle
        TRANSITION_MAP_ENTRY (ST_MOVE)		  // ST_Move
        TRANSITION_MAP_ENTRY (ST_MOVE)        // ST_Jump
    END_TRANSITION_MAP(n)

}
void MovementMachine::IdleF(MovementData* n)
{
    BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)	    // ST_Idle
        TRANSITION_MAP_ENTRY (ST_IDLE)				// ST_Move
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)        // ST_Jump
    END_TRANSITION_MAP(n)

}
void MovementMachine::JumpF(MovementData* n)
{
    BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (ST_JUMP)				// ST_Idle
        TRANSITION_MAP_ENTRY (ST_JUMP)			 	// ST_Move
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)	    // ST_Jump
    END_TRANSITION_MAP(n)

}

STATE_DEFINE(MovementMachine,Idle,MovementData)
{
	if(data->mGrounded == 1)
	{
		mSpriteMachine->IdleS();
	}
	printf("I Am Idle\n");
}
STATE_DEFINE(MovementMachine,Move,MovementData)
{
	printf("I Am Moving\n");
	mSpriteMachine->MoveS();
	switch(data->key)
	{
	case KEY_MOVE_RIGHT:
		gEntities[data->mID].mBody->GetBody()->SetLinearVelocity(b2Vec2(2,
			gEntities[data->mID].mBody->GetBody()->GetLinearVelocity().y));
		if(gEntities[data->mID].mIsFlipped != 1)
		{
			gEntities[data->mID].mIsFlipped = 1;
			FlipFixtures(gEntities[data->mID].mBody->GetBody()->GetFixtureList());
		}
		break;
	case KEY_MOVE_LEFT:
		gEntities[data->mID].mBody->GetBody()->SetLinearVelocity(b2Vec2(-2,
			gEntities[data->mID].mBody->GetBody()->GetLinearVelocity().y));
		if(gEntities[data->mID].mIsFlipped != 0)
		{
			gEntities[data->mID].mIsFlipped = 0;
			FlipFixtures(gEntities[data->mID].mBody->GetBody()->GetFixtureList());
		}
		break;
	default:
		break;
	}
}
STATE_DEFINE(MovementMachine,Jump,MovementData)
{
		mSpriteMachine->JumpS();
		printf("I Am Jumping\n");
}
/**
*This is the Beginning of where i start to define the functions
*	for the spriteMachine
*
*
*/

SpriteMachine::SpriteMachine(int id) : StateMachine(ST_MAX_STATES)
{
	mID = id;
	InternalEvent(ST_IDLE,NULL);
}

void SpriteMachine::IdleS()
{
	BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (ST_IDLE)				// ST_Idle
        TRANSITION_MAP_ENTRY (ST_IDLE)			 	// ST_Move
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)	    // ST_Jump
    END_TRANSITION_MAP(NULL)

}
void SpriteMachine::MoveS()
{
	BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (ST_MOVE)				// ST_Idle
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)	    // ST_Move
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)	    // ST_Jump
    END_TRANSITION_MAP(NULL)

}
void SpriteMachine::JumpS()
{
BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (ST_JUMP)				// ST_Idle
        TRANSITION_MAP_ENTRY (ST_JUMP)			 	// ST_Move
        TRANSITION_MAP_ENTRY (ST_JUMP)			    // ST_Jump
    END_TRANSITION_MAP(NULL)

}


STATE_DEFINE(SpriteMachine,Idle,NoEventData)
{
	gEntities[mID].SetSprite(0);
	printf("Switch to Idle Sprite\n");
}
STATE_DEFINE(SpriteMachine,Move,NoEventData)
{
	gEntities[mID].SetSprite(1);
	printf("Switch to Move Sprite\n");
}
STATE_DEFINE(SpriteMachine,Jump,NoEventData)
{
	gEntities[mID].SetSprite(2);
	printf("Switch to Jump Sprite\n");
}




//Old Code just in case

/*
StateMachine::StateMachine(int maxStates) :
    mMaxStates(maxStates),
    mCurrentState(0),
    mEventGenerated(false),
    mEventData(nullptr)
{

}    
EventData::~EventData()
{

}

MovementData::MovementData()
{
	memset(this,0,sizeof(MovementData));
}
 
// generates an external event. called once per external event 
// to start the state machine executing
void StateMachine::ExternalEvent(unsigned char newState, 
                                 EventData* pData)
{
	if (pData == NULL)
	{
		printf("nulldata");
	}
    // if we are supposed to ignore this event
    if (newState == EVENT_IGNORE) {
       
    }
    else {
        // generate the event and execute the state engine
        InternalEvent(newState, pData); 
        StateEngine();                  
    }
}
 
// generates an internal event. called from within a state 
// function to transition to a new state
void StateMachine::InternalEvent(unsigned char newState, 
                                 EventData* pData)
{
    mEventData = pData;
    mEventGenerated = true;
	if(mPrevStates.size() < 6)
	{
		mPrevStates.push(mCurrentState);
	}
	else
	{
		mPrevStates.pop();
		mPrevStates.push(mCurrentState);
	}
    mCurrentState = newState;
}
 
// the state engine executes the state machine states
void StateMachine::StateEngine(void)
{
 
    // TBD - lock semaphore here
    // while events are being generated keep executing states
    while (mEventGenerated) {         
        mEventGenerated = false;  // event used up, reset flag
       assert(mCurrentState < mMaxStates);
 
        // execute the state passing in event data, if any
        const StateStruct* pStateMap = GetStateMap();
        (this->*pStateMap[mCurrentState].pStateFunc)(mEventData);
        mEventData = NULL;       // event data used up, reset ptr 
	}

    // TBD - unlock semaphore here
}

void Movement::Move(MovementData* n)
{
    BEGIN_TRANSITION_MAP                      // - Current State -
        TRANSITION_MAP_ENTRY (ST_MOVE)		  // ST_Idle
        TRANSITION_MAP_ENTRY (ST_MOVE)		 // ST_Move
        TRANSITION_MAP_ENTRY (ST_MOVE)        // ST_Jump
    END_TRANSITION_MAP(n)

}
void Movement::Idle(MovementData* n)
{
    BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORE)		  // ST_Idle
        TRANSITION_MAP_ENTRY (ST_IDLE)				// ST_Move
        TRANSITION_MAP_ENTRY (EVENT_IGNORE)        // ST_Jump
    END_TRANSITION_MAP(n)

}
void Movement::Jump(MovementData* n)
{
    BEGIN_TRANSITION_MAP                      // - Current State -
        TRANSITION_MAP_ENTRY (ST_JUMP)		  // ST_Idle
        TRANSITION_MAP_ENTRY (ST_JUMP)		  // ST_Move
        TRANSITION_MAP_ENTRY (EVENT_IGNORE)	  // ST_Jump
    END_TRANSITION_MAP(n)

}
void Movement::ST_Idle(MovementData* n)
{
	//Do Nothing
	printf("I Am Idle\n");
}

void Movement::ST_Move(MovementData* n)
{
	printf("I Am Moving\n");
}

void Movement::ST_Jump(MovementData* n)
{
	switch(n->mJump)
	{
	case 0:
		printf("I Am Jumping\n");
		break;
	case 1:
		printf("I Cannot Jump Anymore\n");
		break;
	default:
		break;
	}
}
*/