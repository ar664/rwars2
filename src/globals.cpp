#include <SFML/Graphics.hpp>
#include "globals.h"
#include <string.h>
sf::Event gEvent;
_time_callback *gCallBacks = NULL;
int gCallStackSize = 0;

int MemoryCount(void *src, int size_type)
{
	int i, offset;
	int *source = (int*)src;
	if(source == NULL)
		return 0;
	i = 0; offset = size_type/sizeof(int);
	while( *(source) ) 
	{
		source += offset;
		i++;
	}
	return i;
}

void CallbackInitSystem()
{
	if(gCallBacks)
	{
		return;
	}
	gCallBacks = (_time_callback*) malloc(sizeof(_time_callback)*CALLBACKS_MAX);
	memset(gCallBacks, 0, sizeof(_time_callback)*CALLBACKS_MAX);

}

void CallbackRunSystem()
{
	int i, calls;
	sf::Uint32 time;
	calls = 0;
	time = gClock.getElapsedTime().asMilliseconds();

	//Do the calls, ordered list so if you reached something that doesn't need to be called break.
	for(i = 0; i < gCallStackSize; i++)
	{
		if(time < gCallBacks[i].time)
		{
			break;
		}
		calls++;
		gCallBacks[i].callback(gCallBacks[i].argument);
		memset(&gCallBacks[i], 0, sizeof(_time_callback));
	}

	//Reorder list & update callstacksize
	if(calls)
	{
		gCallStackSize -= calls;
		if(gCallStackSize)
		{
			memmove( gCallBacks, &gCallBacks[i], sizeof(_time_callback)*gCallStackSize);
		}
	}
}

//Internal function
void InsertCallback(_time_callback callback)
{
	int i;
	for(i = 0; i < CALLBACKS_MAX; i++)
	{
		if(gCallBacks[i].time == 0)
		{
			memcpy(&gCallBacks[i], &callback, sizeof(_time_callback));
			gCallStackSize++;
			break;
		}
		if(gCallBacks[i].time > callback.time)
		{
			
			memmove(&gCallBacks[i+1], &gCallBacks[i], sizeof(_time_callback)*(i - gCallStackSize));
			memcpy(&gCallBacks[i], &callback, sizeof(_time_callback));
			gCallStackSize++;
			break;
		}
	}
}

void TimedCallback(sf::Uint32 time, _cb_func func, int arg)
{
	_time_callback cb;
	if(!gCallBacks || gCallStackSize >= CALLBACKS_MAX)
	{
		return;
	}
	cb.argument = arg;
	cb.callback = func;
	cb.time = time;
	InsertCallback(cb);
}
