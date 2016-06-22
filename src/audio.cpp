#include "audio.h"
#include <globals.h>

sf::Music **gPlaylist = NULL;
sf::Sound gSound;
int gCurrentSong = 0;
int gSongs = 0;

void AudioInitSystem()
{

}

void AudioLoadSongs(char **SongFiles)
{
	int i;
	void *temp_sound;
	gPlaylist = (sf::Music**) malloc(sizeof(sf::Music*)*(MAX_SONGS+1));
	memset(gPlaylist, 0, sizeof(sf::Music*)*(MAX_SONGS+1));

	if(!SongFiles)
	{
		return;
	}
	for(i = 0; SongFiles[i]; i++)
	{
		gPlaylist[i] = new sf::Music();
		gPlaylist[i]->openFromFile(SongFiles[i]);
	}
	gSongs = i;
	gPlaylist[i] = NULL;
}

void AudioLoop(int song)
{
	sf::Uint32 loop_next;
	if(!gPlaylist)
	{
		return;
	}
	
	if(song >= gSongs)
	{
		return;
	}
	gPlaylist[song]->play();
	loop_next = gClock.getElapsedTime().asMilliseconds() + gPlaylist[song]->getDuration().asMilliseconds();
	if(song+1 < gSongs)
	{
		TimedCallback(loop_next, AudioLoop, song+1);
	}else
	{
		TimedCallback(loop_next, AudioLoop, 0);
	}
}