#include "Music.h"
#include "Options.h"

Music::Music(Options * options) : OptionsSubType(options),
pagename(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(100, 70), "", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
currentsonginfo(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(250, 70), "", 45, 1, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::NewLine),
columns(2), rows(4)
{
	pagename.setString("Music");
	pagename.fitBackground(true, false);
	pagename.setPosition(sf::Vector2f(10, 100));
	pagename.setClickable(false);

	options->background.changeRectangle()->setTexture(ResourceManager::getTexture("Data//bgMusic.png"));


	std::fstream file;
	file.open("Data\\Music\\songlist.czpal");
	if (file.is_open())
		while (!file.eof())
		{
			Song song;
			std::getline(file, song.path, '\n');
			std::getline(file, song.name, '\n');
			std::getline(file, song.artist, '\n');

			songlist.push_back(song);
		}
	else std::cout << "songlist.txt is missing" << std::endl;
	file.close();



	int padding = 20;

	sf::Vector2f pos(300.0f + padding, 90.0f + padding);

	float widthavilable = 1380 - padding - pos.x - (columns - 1)*padding;
	sf::Vector2f size = sf::Vector2f(widthavilable / columns, 40);



	for (unsigned int x = 0; x < columns; x++)
		for (unsigned int y = 0; y < rows; y++)
		{
			unsigned int i = x*rows + y;

			if (i < songlist.size())
			{
				songs.push_back(Songdisplay(pos, size, x, y, padding));

				songs.back().background.setString(songlist[i].name);

				songs.back().background.setObserver(this);
			}
			else break;
		}

	pos.x = 1390.0f + padding;

	currentsonginfo.setPosition(pos);

	currentsonginfo.setSize(sf::Vector2f(1920 - padding - pos.x, 800));

	syncInfo();
}

void Music::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::GuiElementEvent::Pressed && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
	{
		
			for (unsigned int i = 0; i < songs.size(); i++)
				if (&songs[i].background == from)
				{
					if (i < songlist.size())
					{
						SoundEngine::changeMusic(songlist[i].path, true, true);
						syncInfo();
					}

				}
		SoundEngine::playSound("click");
	}
}
