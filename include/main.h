#ifndef __MAIN_H
#define __MAIN_H
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