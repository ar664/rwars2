#include "audio.h"

sf::Music **gPlaylist = NULL;

void AudioLoadSongs(char **SongFiles)
{
	int i;
	void *temp_sound;
	gPlaylist = (sf::Music**) malloc(sizeof(sf::Music*)*(MAX_SONGS+1));
	memset(gPlaylist, 0, sizeof(sf::Sound*)*(MAX_SONGS+1));

	if(!SongFiles)
	{
		return;
	}
	for(i = 0; SongFiles[i]; i++)
	{
		gPlaylist[i] = new sf::Music;
		gPlaylist[i]->openFromFile(SongFiles[i]);
	}
	gPlaylist[i] = NULL;
}