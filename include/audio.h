#ifndef __AUDIO_H
#define __AUDIO_H

#include <SFML\Audio.hpp>

#define MAX_SONGS	10

extern sf::Music** gPlaylist;

void AudioLoadSongs(char **SongFiles);

#endif