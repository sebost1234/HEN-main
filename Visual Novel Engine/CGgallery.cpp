#include "CGgallery.h"
#include "Options.h"
#include "Music.h"

CGGallery::CGGallery(Options * options) : OptionsSubType(options),
pagename(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(100, 70), "", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
chapter1info(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Chapter 1:", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
chapter1page1(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(40, 40),
	"1", 30, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	"GalleryCombNormal.png", "GalleryCombHover.png", "GalleryCombHover.png", "Data\\"),
	chapter1page2(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(40, 40),
		"2", 30, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"GalleryCombNormal.png", "GalleryCombHover.png", "GalleryCombHover.png", "Data\\"),
	columns(4), rows(3),
	fullscreen(ResourceManager::getStyle(StyleTypes::blankwhite), gamesize)
{
	fullscreen.setClickable(true);
	fullscreen.setObserver(this);

	options->background.changeRectangle()->setTexture(ResourceManager::getTexture("Data//bgGallery.png"));

	pagename.setString("Gallery");
	pagename.fitBackground(true, false);
	pagename.setPosition(sf::Vector2f(10, 100));
	pagename.setClickable(false);

	{
		float padding = 10;
		sf::Vector2f pos = sf::Vector2f(300 * 0.5f - pagename.getSize().x*0.5f, 200);
		chapter1info.setPosition(pos);
		chapter1info.setClickable(false);
		chapter1page1.setPosition(chapter1info.getPosition() + sf::Vector2f(padding, chapter1info.getSize().y));
		chapter1page1.setObserver(this);
		chapter1page2.setPosition(chapter1page1.getPosition() + sf::Vector2f(chapter1page1.getSize().x + padding, 0));
		chapter1page2.setObserver(this);
	}

	{
		int padding = 20;

		sf::Vector2f pos(300.0f + padding, 90.0f + padding);

		float widthavilable = 1920 - padding - pos.x - (columns - 1)*padding;
		sf::Vector2f size = sf::Vector2f(widthavilable / columns, ((widthavilable / columns)*9.0f) / 16.0f);

		for (unsigned int y = 0; y < rows; y++)
			for (unsigned int x = 0; x < columns; x++)
			{
				cgs.push_back(CGdisplay(pos, size, x, y, padding));
				cgs.back().background.setObserver(this);
			}

		std::fstream file;
		file.open("Data\\Game\\cglist.czpal");
		if (file.is_open())
			while (!file.eof())
			{
				std::string path;
				std::getline(file, path, '\n');
				cglist.push_back(path);
			}
		file.close();
		file.open("Data\\Save\\seencgs.czpal");
		if (file.is_open())
			while (!file.eof())
			{
				std::string path;
				std::getline(file, path, '\n');
				seencglist.push_back(path);
			}

		file.close();

		currentpage = 1;
		sync(1);
	}
}

void CGGallery::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::GuiElementEvent::Pressed && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
	{
		if (from == &chapter1page1)
			sync(1);
		else if (from == &chapter1page2)
			sync(2);
		else if (from == &fullscreen)
		{
			auto tmp = pagename.getGui();
			if (tmp != nullptr)
				tmp->eraseElement(&fullscreen);
		}
		else
		{
			for (unsigned int i = 0; i < cgs.size(); i++)
				if (&cgs[i].background == from)
				{
					unsigned int nr = rows*columns*(currentpage-1) + i;
					if (nr < cglist.size())
					{
						auto tmp = pagename.getGui();
						if (tmp != nullptr)
						{
							fullscreen.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\Game\\" + cglist[nr]));
							tmp->addElement(&fullscreen);
						}
					}
				}

		}


		SoundEngine::playSound("click");
	}
}
