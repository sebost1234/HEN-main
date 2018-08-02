#include "SoundEngine.h"

std::map<std::string, Audio>  SoundEngine::audio;

std::string SoundEngine::currentmusic = "NONE";
sf::Music*SoundEngine::music = nullptr;

int SoundEngine::bgvolume;
int SoundEngine::sfxvolume;
int SoundEngine::systemvolume;