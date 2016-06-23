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
		if(gPlaylist[i]->openFromFile(SongFiles[i]))
		{
			printf("Song #%i : %s loaded successfully \n", i, SongFiles[i]);
		} else
		{
			printf("Song #%i : %s could not be loaded. \n", i, SongFiles[i]);
		}
	}
	gSongs = i;
	gPlaylist[i] = NULL;
}

void AudioLoop(int song)
{
	sf::Uint32 loop_next, song_time;
	if(!gPlaylist)
	{
		return;
	}
	
	if(song >= gSongs)
	{
		return;
	}

	//Update the status of the songs
	if(gPlaylist[gCurrentSong]->getStatus() == sf::Music::Playing)
	{
		printf("Stopped Playing Song #%i\n", gCurrentSong);
		gPlaylist[gCurrentSong]->stop();
	}
	printf("Started Playing Song #%i \n", song);
	gPlaylist[song]->play();

	//Get Song Play Time
	song_time = gPlaylist[song]->getDuration().asMilliseconds();
	loop_next = gClock.getElapsedTime().asMilliseconds() + song_time;

	//Add callback when song finishes
	gCurrentSong = song;
	if(song+1 < gSongs)
	{
		TimedCallback(loop_next, AudioLoop, song+1);
	}else
	{
		TimedCallback(loop_next, AudioLoop, 0);
	}
}