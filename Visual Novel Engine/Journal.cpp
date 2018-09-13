#include "Journal.h"
#include "Options.h"
#include "VisualNovelControler.h"

Journal::Journal(Options * options) : OptionsSubType(options),
pagename(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(100, 70), "", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
journalleft(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(600, 845), "", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
journalright(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(600, 845), "", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
title(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(600, 70), "", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::NewLine),
entry(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(600, 675), "", 40, 5, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::NewLine),
pageinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(600, 70), "", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::NewLine),
image(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), sf::Vector2f(585, 1000)),
//hoverimage(ResourceManager::getStyle(StyleTypes::transparentbackground), sf::Vector2f(585, 1000)),
prevpage(GuiNS::GuiRectangle(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(40,40)), "Normal.png", "Hover.png", "Hover.png", "Data//Game//Journal//JournalArrowRight", 0, true),
nextpage(GuiNS::GuiRectangle(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(40, 40)), "Normal.png", "Hover.png", "Hover.png", "Data//Game//Journal//JournalArrowRight")
{
	pagename.setString("Journal");
	pagename.fitBackground(true, false);
	pagename.setPosition(sf::Vector2f(10, 100));
	pagename.setClickable(false);


	options->background.setTexture(ResourceManager::getBigTexture("Data//bgJournal.png"));

	journalleft.setClickable(false);
	journalleft.changeBackground()->setTexture(ResourceManager::getTexture("Data//Game//Journal//JournalPage.png"));
	journalleft.setPosition(sf::Vector2f(100, 200));
	journalright.setClickable(false);
	journalright.changeBackground()->setTexture(ResourceManager::getTexture("Data//Game//Journal//JournalPage.png"));
	journalright.setPosition(sf::Vector2f(1285, 110));


	{
		std::wfstream file;

		file.open("Data//Game//Journal//CharacterList.czpal");

		if (file.is_open())
			while (!file.eof())
			{
				std::vector<std::wstring> arguments;
				loadArguments(arguments, file);

				if (arguments.size() >= 3)
				{
					JournalCharacter tmp;

					tmp.name = arguments[0];
					tmp.picture = arguments[1];

					arguments.erase(arguments.begin(), arguments.begin()+2);

					tmp.entries = arguments;

					characters.push_back(tmp);
				}
			}

		file.close();
	}

	auto globalflags = VisualNovelControler::getGlobalFlags();

	for (int i = characters.size()-1; i >= 0; i--)
	{
		for (int o = characters[i].entries.size()-1; o >= 0; o--)
		{
			if (std::find(globalflags.begin(), globalflags.end(), characters[i].entries[o]) == globalflags.end())
				characters[i].entries.erase(characters[i].entries.begin() + o);
		}

		if (characters[i].entries.empty())
			characters.erase(characters.begin() + i);
	}

	{
		std::wfstream file;

		file.open("Data//Game//Journal//JournalData.czpal");

		if (file.is_open())
			while (!file.eof())
			{
				std::vector<std::wstring> arguments;
				loadArguments(arguments, file);

				if (arguments.size() == 3)
				{
					JournalEntry tmp;


					tmp.entryname = arguments[0];
					tmp.title = arguments[1];
					tmp.entry = arguments[2];

					entries.push_back(tmp);
				}
			}

		file.close();
	}

	float padding1 = 40;
	float padding2 = 10;

	{
		sf::Vector2f pos = journalleft.getPosition() + sf::Vector2f(padding1, padding1);
		if (characters.empty())
		{
			title.setString("Journal is empty!"); 
		}
		else
		{
			for (unsigned int i = 0; i < characters.size(); i++)
			{
				names.push_back(new GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(100, 70), "", 60, 5, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing));
				names.back()->setPosition(pos);
				pos.y += names.back()->getSize().y;
				names.back()->setString(characters[i].name);
				names.back()->fitBackground(true, false);

				names.back()->setObserver(this);
			}
		}
	}

	{
		image.setPosition(journalleft.getPosition() + sf::Vector2f(journalleft.getSize().x, 0));
		image.setClickable(false);

		//hoverimage.setPosition(sf::Vector2f(journalleft.getPosition().x, gamesize.y*0.5f));
		//hoverimage.setClickable(false);
	}

	{
		title.setPosition(journalright.getPosition());
		entry.setPosition(title.getPosition() + sf::Vector2f(0, title.getSize().y + padding2));
		pageinfo.setPosition(entry.getPosition() + sf::Vector2f(0, entry.getSize().y + padding2));

		title.setClickable(false);
		entry.setClickable(false);
		pageinfo.setClickable(false);

		prevpage.setPosition(journalright.getPosition() + sf::Vector2f(padding2, journalright.getSize().y - prevpage.getSize().y - padding2));
		prevpage.setObserver(this);
		nextpage.setPosition(journalright.getPosition() + sf::Vector2f(journalright.getSize().x - prevpage.getSize().x - padding2, journalright.getSize().y - prevpage.getSize().y - padding2));
		nextpage.setObserver(this);
	}


	//currenthover = -1;
	currentcharacter = -1;
	currentyentry = L"NONE";

	syncPageInfo(0, 0);
	//setCharacter(0);
}

void Journal::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::GuiElementEvent::Mouse)
	{
		for (unsigned int i = 0; i < names.size(); i++)
		{
			if (names[i] == from)
			{
				if (event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
				{
					//hoverimage.setPosition(sf::Vector2f(journalleft.getPosition().x, gamesize.y));
					//hoverimage.setStyle(ResourceManager::getStyle(StyleTypes::transparentbackground));
					//currenthover = -1;

					setCharacter(i);
					SoundEngine::playSound("kartka");
				}

				if (i != currentcharacter)
				{
					if (event.mouse.type == GuiNS::GuiElementEvent::Type::Hover)
					{
						names[i]->setPrefix(">");
						//hoverimage.setStyle(ResourceManager::getStyle(StyleTypes::blankwhite));
						//hoverimage.changeRectangle()->setTexture(ResourceManager::getTexture("Data//Game//Journal//" + sf::String(characters[i].picture).toAnsiString()));
						//currenthover = i;
						//timer = 0;
					}
					else if (event.mouse.type == GuiNS::GuiElementEvent::Type::Unhover)
					{
						names[i]->setPrefix("");

						//hoverimage.setPosition(sf::Vector2f(journalleft.getPosition().x, gamesize.y));
						//hoverimage.setStyle(ResourceManager::getStyle(StyleTypes::transparentbackground));
						//currenthover = -1;
					}
				}
				break;
			}
		}

		if (event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
		{
			if (from == &prevpage)
			{
				prevPage();
				SoundEngine::playSound("kartka");
			}
			else if (from == &nextpage)
			{
				nextPage();
				SoundEngine::playSound("kartka");
			}
		}
	}
}