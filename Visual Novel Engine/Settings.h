#pragma once
#include <fstream>

#include "SoundEngine.h"


class Settings
{
public:
	Settings()
	{
		std::fstream file;
		file.open("Data\\Save\\settings.czpal", std::ios::in);
		int mastervolume;
		file >> mastervolume;
		setMasterVolume(mastervolume);
		file >> SoundEngine::bgvolume;
		setBgVolume(SoundEngine::bgvolume);
		file >> SoundEngine::sfxvolume;
		setSfxVolume(SoundEngine::sfxvolume);
		file >> SoundEngine::systemvolume;
		setSystemVolume(SoundEngine::systemvolume);

		file >> skip;
		file >> fullscreen;
		file >> textspeed;
	}

	void saveToFile()
	{
		std::fstream file;
		file.open("Data\\Save\\settings.czpal", std::ios::out | std::ios::trunc);
		file << getMasterVolume() << "\n";
		file << SoundEngine::bgvolume << "\n";
		file << SoundEngine::sfxvolume << "\n";
		file << SoundEngine::systemvolume << "\n";

		file << skip << "\n";
		file << fullscreen << "\n";
		file << textspeed << "\n";
	}

	void restoreDefaults()
	{
		setSkipping(false);
		setTextspeed(25);

		setMasterVolume(50);
		setBgVolume(50);
		setSfxVolume(50);
		setSystemVolume(50);
	}

	void setFullscreen(bool set)
	{
		fullscreen = set;
	}
	void setSkipping(bool set)
	{
		skip = set;
	}
	bool isFullscreen()
	{
		return fullscreen;
	}
	bool isSkipping()
	{
		return skip;
	}

	void setTextspeed(int set)
	{
		textspeed = set;
	}
	int getTextspeed()
	{
		return textspeed;
	}

	void setMasterVolume(int newvolume)
	{
		sf::Listener::setGlobalVolume(float(newvolume));
	}
	void setBgVolume(int newvolume)
	{
		if (SoundEngine::music != nullptr)
			SoundEngine::music->setVolume(float(newvolume));
		SoundEngine::bgvolume = newvolume;
	}
	void setSfxVolume(int newvolume)
	{
		SoundEngine::sfxvolume = newvolume;
	}
	void setSystemVolume(int newvolume)
	{
		SoundEngine::systemvolume = newvolume;
	}

	int getMasterVolume()
	{
		return int(sf::Listener::getGlobalVolume());
	}
	int getBgVolume()
	{
		return SoundEngine::bgvolume;
	}
	int getSfxVolume()
	{
		return SoundEngine::sfxvolume;
	}
	int getSystemVolume()
	{
		return SoundEngine::systemvolume;
	}
private:
	//Sound (inside SoundEngine.h)
	//int bgvolume;
	//int sfxvolume;
	//int systemvolume;
	//Game
	bool skip;
	bool fullscreen;
	int textspeed;
};