#ifndef _GLOBALS_H
#define _GLOBALS_H
/**
 * @brief Contains all the globals variables!
*/

#define ASSET_FILE			"assets.txt"
#define ASSETS_CHARACTERS	1
#define ASSETS_PLATFORMS	0
#define ASSETS_BACKGROUNDS	0
#define ASSETS_SONGS		1

#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		600
extern int numSprites;
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

/**
 * @breif	Counts the memory of the array of memory that HAS BEEN NULL TERMINATED.
 *
 * @param	void *,		The memory to count
 * @param	size_type,	The size of individual pieces of memory. Use Sizeof.
 *
 * @author	Anthony Rios
 */
int MemoryCount(void *, int size_type);

#endif
