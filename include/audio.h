#ifndef __AUDIO_H
#define __AUDIO_H

#include <SFML/Audio.hpp>

#define MAX_SONGS	10

extern sf::Music** gPlaylist;
extern sf::Sound gSound;

void AudioInitSystem();
void AudioLoadSongs(char **SongFiles);

void AudioLoop(int song);

#endif
