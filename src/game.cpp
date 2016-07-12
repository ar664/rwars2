#include "game.h"

int Game::state_;

int Game::update()
{
    switch (state_)
    {
        case STATE_PLAYING:
            // Code for playing state

            break;

        case STATE_QUITING:
            // Code for quitting state

            break;
    }

}

int Game::getState()
{
    return state_;
}

void Game::setState(int newState)
{
    state_ = newState;
}

