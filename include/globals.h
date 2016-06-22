#ifndef _GLOBALS_H
#define _GLOBALS_H
/**
 * @brief Contains all the globals variables!
*/
#define CALLBACKS_MAX		1000

#define ASSET_FILE			"assets.txt"
#define ASSETS_CHARACTERS	1
#define ASSETS_PLATFORMS	0
#define ASSETS_BACKGROUNDS	0
#define ASSETS_SONGS		1

#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600
extern int numSprites;


//Whenever ssing an external type include the file before you use it to reduce errors.
#include <SFML\Graphics.hpp>
extern sf::RenderWindow gRenderWindow;
extern sf::Event gEvent;
extern sf::Clock gClock;

/**
 * The animation order for characters.
 *
 */
enum AnimationCharacter
{
	ANIMATION_IDLE,
	ANIMAITON_ATTACK,
	ANIMATION_DOODLE
};

/**
 * The sound order for characters.
 *
 */
enum SoundCharacter
{
	SOUND_IDLE,
	SOUND_ATTACK,
	SOUND_DOODLE
};

typedef void (*_cb_func)(int) ;

typedef struct _time_callback
{
	sf::Uint32 time;
	_cb_func callback;
	int argument;
};

extern _time_callback *gCallBacks;

/**
 * @breif	Counts the memory of the array of memory that HAS BEEN NULL TERMINATED.
 *
 * @param	void *,		The memory to count
 * @param	size_type,	The size of individual pieces of memory. Use Sizeof.
 *
 * @author	Anthony Rios
 */
int MemoryCount(void *, int size_type);

/**
 * @breif Initialize the callback system by allocating the gCallBacks
 */
void CallbackInitSystem();

/**
 * @breif Run
 */
void CallbackRunSystem();

/**
 * @brief Add Callbacks that are time based on gClock
 * 
 * @param	time	The time the you want to have the callback 'called'
 * @param	func	The function to call
 * @param	arg		An additional argument to call the function with.
 *
 *
 * @note	The gCallBacks will be an ordered array list, so inserts will take time accordingly.
 * @note	If someone can optimize this that would be great.
 * @author	Anthony Rios
 */
void TimedCallback(sf::Uint32 time, _cb_func func, int arg );

#endif
