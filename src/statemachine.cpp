#include <assert.h>
#include "globals.h"
#include "statemachine.h"

#define KEY_MOVE_RIGHT 1
#define KEY_MOVE_LEFT 2

using namespace std;


StateMachine::StateMachine(unsigned char maxStates, unsigned char initialState) :
	MAX_STATES(maxStates),
	m_currentState(initialState),
	m_newState(false),
	m_eventGenerated(false),
	m_pEventData(NULL)
{
	ASSERT_TRUE(MAX_STATES < EVENT_IGNORED);
}  


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


void StateMachine::InternalEvent(unsigned char newState, const EventData* pData)
{
	if (pData == NULL)
		pData = new NoEventData();

	m_pEventData = pData;
	m_eventGenerated = true;
	m_newState = newState;
}


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
CharacterStateMachine::CharacterStateMachine() : StateMachine(ST_MAX_STATES)
{
}
 
void CharacterStateMachine::MoveF(MovementData* n)
{
    BEGIN_TRANSITION_MAP                      // - Current State -
        TRANSITION_MAP_ENTRY (ST_MOVE)		  // ST_Idle
        TRANSITION_MAP_ENTRY (ST_MOVE)		  // ST_Move
        TRANSITION_MAP_ENTRY (ST_MOVE)        // ST_Jump
    END_TRANSITION_MAP(n)

}
void CharacterStateMachine::IdleF(MovementData* n)
{
    BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)	    // ST_Idle
        TRANSITION_MAP_ENTRY (ST_IDLE)				// ST_Move
        TRANSITION_MAP_ENTRY (ST_IDLE)				// ST_Jump
    END_TRANSITION_MAP(n)

}
void CharacterStateMachine::JumpF(MovementData* n)
{
    BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (ST_JUMP)				// ST_Idle
        TRANSITION_MAP_ENTRY (ST_JUMP)			 	// ST_Move
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)	    // ST_Jump
    END_TRANSITION_MAP(n)

}

STATE_DEFINE(CharacterStateMachine,Idle,MovementData)
{
	if(data->mTouchingGround == 1)
	{
		gEntities[data->mID].mSpriteMachine->IdleS();
	}
	//printf("I Am Idle\n");
}
STATE_DEFINE(CharacterStateMachine,Move,MovementData)
{
	//printf("I Am Moving\n");
	if(data->mTouchingGround != 0)
	{
		//printf("%d\n",data->mTouchingGround);
		gEntities[data->mID].mSpriteMachine->MoveS();
	}
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
STATE_DEFINE(CharacterStateMachine,Jump,MovementData)
{
		gEntities[data->mID].mSpriteMachine->JumpS();
		//printf("I Am Jumping\n");
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
        TRANSITION_MAP_ENTRY (ST_IDLE)				// ST_Jump
    END_TRANSITION_MAP(NULL)

}
void SpriteMachine::MoveS()
{
	BEGIN_TRANSITION_MAP							// - Current State -
        TRANSITION_MAP_ENTRY (ST_MOVE)				// ST_Idle
        TRANSITION_MAP_ENTRY (EVENT_IGNORED)	    // ST_Move
        TRANSITION_MAP_ENTRY (ST_MOVE)				  // ST_Jump
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
	gEntities[mID].SetBodyFixtures(gEntities[mID].mCurrentSprite->mHurtBoxData);
	//printf("Switch to Idle Sprite\n");
}
STATE_DEFINE(SpriteMachine,Move,NoEventData)
{
	gEntities[mID].SetSprite(1);
	gEntities[mID].SetBodyFixtures(gEntities[mID].mCurrentSprite->mHurtBoxData);
	//printf("Switch to Move Sprite\n");
}
STATE_DEFINE(SpriteMachine,Jump,NoEventData)
{
	gEntities[mID].SetSprite(2);
	gEntities[mID].SetBodyFixtures(gEntities[mID].mCurrentSprite->mHurtBoxData);
	//printf("Switch to Jump Sprite\n");
}



