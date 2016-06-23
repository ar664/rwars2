#ifndef __MAIN_H
#define __MAIN_H

/**
 * @brief Loads all the assests from the assets.txt file. Calls exit if critical failure.
 *
 * @note	Assets are loaded by order as of this version
 * 
 *	Order:
 *		Sprites: Characters->Platforms->Backgrounds
 *		Sounds:	Characters->Songs->Other
 *
 *	If you load x characters' sprites into the game, it is assumed theres x characters' sounds.
 *	Other sounds belongs to platform right now.
 */
void LoadAssets();

/**
 * @brief Intializes all systems
 */
void Init_All();
/**
 * @brief Game Loop
 */
void Loop();
/**
 * @brief Handles events
 * @param an sf::Event
 */
void HandleEvent(sf::Event Event);

#endif