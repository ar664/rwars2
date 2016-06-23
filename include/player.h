#ifndef __PLAYER_H
#define __PLAYER_H

#include "entity.h"

class Character : public Entity
{
	int			mHealth;
	int			mState;

public:
	void		Think();

};

extern Character **Characters;

void PlayerLoad(int num, char **sprites);



#endif