#include "player.h"
#include "globals.h"

Character **Characters = (Character**) malloc(sizeof(Character*)*ASSETS_CHARACTERS);

void Character::Think()
{
	//Yeah I'm not doing this right now.
}

void PlayerLoad(int num, char** sprites)
{
	Character *player;
	if(num > ASSETS_CHARACTERS || !Characters)
	{
		return;
	}

	Characters[num] = new Character;
	player = Characters[num];
	
	player->LoadSprites(sprites);
}
