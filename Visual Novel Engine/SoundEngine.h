#pragma once

#include <SFML\Audio.hpp>
#include <string>
#include <deque>
#include <vector>
#include <map>

class Audio
{
public:
	bool init(std::string src)
	{
		return buffer.loadFromFile(src);
	}
	void play()
	{
		sound.push_back(sf::Sound());
		sound.back().setBuffer(buffer);
		sound.back().play();
	}
	sf::Sound getLast()
	{
		return sound.back();
	}
	void sync()
	{
		for (unsigned int i = 0; i < sound.size(); i++)
			if (sound[i].getStatus() == sf::Sound::Stopped)
			{
				sound.erase(sound.begin() + i);
				i--;
			}
	}
private:
	sf::SoundBuffer buffer;
	std::deque<sf::Sound> sound;
};

class SoundEngine
{
public:
	static void playSound(std::string sound)
	{
		if (audio.count(sound) == 0)
		{
			audio[sound].init("Data\\Sound\\" + sound + ".wav");
		}
		audio[sound].play();
	}

	static void changeMusic(std::string sound, bool loop = false)
	{
		if (music != nullptr)
		music = new sf::Music();
		music->openFromFile("Data\\Music\\" + sound + ".wav");
		
		music->setLoop(loop);
		music->play();	
		music->setVolume(100);
	}

	static void sync_audio()
	{
		for (auto&tmp : audio)
			tmp.second.sync();
	}

	static void clear_audio()
	{
		audio.clear();
		if (music != nullptr)
			delete music;
	}
private:
	static std::map<std::string, Audio> audio;

	static sf::Music*music;
};


