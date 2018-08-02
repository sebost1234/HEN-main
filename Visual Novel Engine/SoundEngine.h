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
	void play(int volume)
	{
		sound.push_back(sf::Sound());
		sound.back().setBuffer(buffer);
		sound.back().play();
		sound.back().setVolume(float(volume));
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

class Settings;


class SoundEngine
{
public:
	enum Type
	{
		sfx,
		system
	};

	static void playSound(std::string sound, Type type = Type::system)
	{
		if (audio.count(sound) == 0)
		{
			audio[sound].init("Data\\Sound\\" + sound + ".wav");
		}
		if(type == Type::system)
			audio[sound].play(systemvolume);
		else
			audio[sound].play(sfxvolume);
	}

	static void changeMusic(std::string sound, bool loop = false, bool startnew = false)
	{
		if (sound != "NONE")
		{
			if (sound == currentmusic)
			{
				if (startnew)
				{
					music->play();
				}
				music->setLoop(loop);
				music->setVolume(float(bgvolume));
			}
			else
			{
				if (music == nullptr)
					music = new sf::Music();
				music->openFromFile("Data\\Music\\" + sound);

				music->setLoop(loop);
				music->play();
				music->setVolume(float(bgvolume));
				currentmusic = sound;
			}
		}
		else
		{
			if (music != nullptr)
			{
				delete music;
				music = nullptr;
			}
			currentmusic = "NONE";
		}
	}

	static std::string getCurrentMusic()
	{
		return currentmusic;
	}

	static void sync_audio()
	{
		for (auto&tmp : audio)
			tmp.second.sync();
		if(music != nullptr)
			if (music->getStatus() == sf::Sound::Stopped)
			{
				delete music;
				music = nullptr;
				currentmusic = "NONE";
			}
	}

	static void clear_audio()
	{
		audio.clear();
		if (music != nullptr)
			delete music;
		music = nullptr;
	}
private:
	static std::map<std::string, Audio> audio;


	static std::string currentmusic;
	static sf::Music*music;

	static int bgvolume;
	static int sfxvolume;
	static int systemvolume;

	friend class Settings;
};


