#include "SoundEngine.h"

std::map<std::string, Audio>  SoundEngine::audio;

sf::Music*SoundEngine::music = nullptr;