#pragma once


#include "OptionsSubtype.h"



struct Song
{
	Song()
	{
		path = "";
		name = "NONE";
		artist = "";
	}
	std::string path;
	std::string name;
	std::string artist;
};

struct Songdisplay
{
	Songdisplay(sf::Vector2f offset, sf::Vector2f size, int x, int y, int padding) :
		background(ResourceManager::getStyle(StyleTypes::normal), *ResourceManager::getFont(), size, "...", 30, 1, 5, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center)
	{
		sf::Vector2f pos = offset + sf::Vector2f((size.x + padding) * x, (size.y + padding)*y);
		pos.x = float(int(pos.x));
		pos.y = float(int(pos.y));

		background.setPosition(pos);
		//background.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\EmptySavePicture.png"), true);
	}

	void load(Song song)
	{
		background.setClickable(true);
	}

	void clear()
	{
		background.setClickable(false);
	}

	void enable(GuiNS::Gui*gui)
	{
		gui->addElement(&background);
	}

	void disable(GuiNS::Gui*gui)
	{
		gui->eraseElement(&background);
	}

	GuiNS::GuiText background;
};

class Music : public OptionsSubType
{
public:
	Music(Options*options);

	virtual void enable(GuiNS::Gui*gui) override
	{
		for (unsigned int i = 0; i < songs.size(); i++)
			songs[i].enable(gui);
		gui->addElement(&pagename);
		gui->addElement(&currentsonginfo);
	}

	virtual void disable(GuiNS::Gui*gui)override
	{
		for (unsigned int i = 0; i < songs.size(); i++)
			songs[i].disable(gui);
		gui->eraseElement(&pagename);
		gui->eraseElement(&currentsonginfo);
	}

	virtual OptionsSubTypeEnum getSubtype()
	{
		return OptionsSubTypeEnum::Music_ST;
	}

	Song getCurrentSong()
	{
		std::string tmp = SoundEngine::getCurrentMusic();
		for (unsigned int i = 0; i < songlist.size(); i++)
		{
			if (songlist[i].path == tmp)
				return songlist[i];
		}
		return Song();
	}

	void syncInfo()
	{
		Song tmp = getCurrentSong();
		if (getCurrentSong().name == "NONE")
			currentsonginfo.setString("No music is currently playing");
		else
		{
			currentsonginfo.setString("Name: " + tmp.name + "\n\n" + "Artist: " + tmp.artist);
		}
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from) override;
private:
	const unsigned int rows;
	const unsigned int columns;


	GuiNS::GuiText currentsonginfo;
	std::vector<Song> songlist;
	std::vector<Songdisplay> songs;

	GuiNS::GuiText pagename;
};
