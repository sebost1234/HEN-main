#pragma once

#include <string>
#include "OptionsSubtype.h"




struct JournalCharacter
{
	std::wstring name;
	std::wstring picture;
	std::vector<std::wstring> entries;
};

struct JournalEntry
{
	std::wstring entryname;
	std::wstring title;
	std::wstring entry;
};

class Journal: public OptionsSubType
{
public:
	Journal(Options*options);
	~Journal()
	{
		for (auto&tmp : names)
			delete tmp;
	}

	virtual void enable(GuiNS::Gui*gui) override
	{
		gui->addElement(&pagename);
		gui->addElement(&journalleft);
		gui->addElement(&journalright);
		gui->addElement(&title);
		gui->addElement(&entry);
		gui->addElement(&pageinfo);
		gui->addElement(&image);
		//gui->addElement(&hoverimage);

		gui->addElement(&prevpage);
		gui->addElement(&nextpage);

		for (auto&tmp : names)
			gui->addElement(tmp);
	}

	virtual void disable(GuiNS::Gui*gui)override
	{
		gui->eraseElement(&pagename);
		gui->eraseElement(&journalleft);
		gui->eraseElement(&journalright);
		gui->eraseElement(&title);
		gui->eraseElement(&entry);
		gui->eraseElement(&pageinfo);
		gui->eraseElement(&image);
		//gui->eraseElement(&hoverimage);

		gui->eraseElement(&prevpage);
		gui->eraseElement(&nextpage);

		for (auto&tmp : names)
			gui->eraseElement(tmp);
	}

	virtual OptionsSubTypeEnum getSubtype()
	{
		return OptionsSubTypeEnum::Journal_ST;
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from);

	virtual void sync(float time) override
	{
		timer += time;
	}
private:
	std::wstring getLine(std::wfstream&file)
	{
		std::wstring line;
		std::getline(file, line, L'\n');

		if (line.size() > 3)
			if (line.substr(0, 3) == L"\xEF\xBB\xBF")
				line = line.substr(3, line.size() - 3);

		return line;
	}

	void loadArguments(std::vector<std::wstring>&arguments, std::wfstream&file)
	{
		std::wstring inbracket = L"";
		bool bracketstarted = false;
		int last = 0;

		std::wstring line = getLine(file);

		for (unsigned int i = 0; i < line.size(); i++)
		{
			if (line[i] == '"')
			{
				if (bracketstarted)
				{
					inbracket = line.substr(last + 1, i - last - 1);
					if (i == line.size() - 1)
						arguments.push_back(inbracket);
					bracketstarted = false;
				}
				else bracketstarted = true;
			}
			else if (line[i] == ';' && !bracketstarted)
			{
				if (inbracket != L"")
					arguments.push_back(inbracket);
				else
					arguments.push_back(line.substr(last, i - last));

				inbracket = L"";
				last = i + 1;
			}
			else if (i == line.size() - 1)
			{
				if (!bracketstarted)
					arguments.push_back(line.substr(last, line.size() - last));
				else
				{
					line += L'\n' + getLine(file);
				}
			}
		}
	}

	void setCharacter(int nr)
	{
		if (nr != currentcharacter)
		{
			if (currentcharacter != -1)
			{
				names[currentcharacter]->changeTextApperance()->setStyle(sf::Text::Regular);
				names[currentcharacter]->setClickable(true);
			}

			currentcharacter = nr;

			image.changeRectangle()->setTexture(ResourceManager::getTexture("Data//Game//Journal//" + sf::String(characters[nr].picture).toAnsiString()), true);
			sf::Vector2f size = sf::Vector2f((float)image.changeRectangle()->getTextureRect().width, (float)image.changeRectangle()->getTextureRect().height);
			image.changeRectangle()->setSize(size);
			image.changeRectangle()->setOrigin(size*0.5f);
			image.setPosition(sf::Vector2f(1000, 400) + sf::Vector2f(sin(timer*1220.0f+100.0f),sin(timer*1400.0f+50.0f)));

			image.changeRectangle()->setRotation(sin(timer*1000.0f + 1.0f) * 10);



			setEntry(characters[nr].entries.front());
			syncPageInfo(1, characters[nr].entries.size());


			image.setStyle(ResourceManager::getStyle(StyleTypes::blankwhite));

			names[nr]->changeTextApperance()->setStyle(sf::Text::Underlined);
			names[nr]->setClickable(false);
		}
	}
	
	void nextPage()
	{
		if (currentcharacter != -1)
		{
			unsigned int i = 0;
			for (; i < characters[currentcharacter].entries.size(); i++)
			{
				if (characters[currentcharacter].entries[i] == currentyentry)
					break;
			}
			if (i != characters[currentcharacter].entries.size() - 1)
			{
				setEntry(characters[currentcharacter].entries[i + 1]);
				syncPageInfo(i + 2, characters[currentcharacter].entries.size());
			}
		}
	}

	void prevPage()
	{
		if (currentcharacter != -1)
		{
			int i = characters[currentcharacter].entries.size()-1;
			for (; i>=0; i--)
			{
				if (characters[currentcharacter].entries[i] == currentyentry)
					break;
			}
			if (i != 0)
			{
				setEntry(characters[currentcharacter].entries[i - 1]);
				syncPageInfo(i , characters[currentcharacter].entries.size());
			}
		}
	}

	void syncPageInfo(int page, int pagenr)
	{
		if (currentcharacter != -1)
			pageinfo.setString(std::to_wstring(page) + L"\\" + std::to_wstring(pagenr));
		else
			pageinfo.setString(L"");

		if (page == 1 || currentcharacter == -1)
		{
			prevpage.setStyle(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext));
			prevpage.setClickable(false);
		}
		else
		{
			prevpage.setStyle(ResourceManager::getStyle(StyleTypes::blankwhite));
			prevpage.setClickable(true);
		}

		if (page == pagenr || currentcharacter == -1)
		{
			nextpage.setStyle(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext));
			nextpage.setClickable(false);
		}
		else
		{
			nextpage.setStyle(ResourceManager::getStyle(StyleTypes::blankwhite));
			nextpage.setClickable(true);
		}
	}

	bool setEntry(std::wstring name)
	{
		for (unsigned int i = 0; i < entries.size(); i++)
		{
			if (entries[i].entryname == name)
			{
				title.setString(sf::String::fromUtf8(entries[i].title.begin(), entries[i].title.end()));
				entry.setString(sf::String::fromUtf8(entries[i].entry.begin(), entries[i].entry.end()));
				currentyentry = entries[i].entryname;
				return true;
			}
		}
		return false;
	}

	GuiNS::GuiText pagename;

	std::vector<GuiNS::GuiText*>  names;

	GuiNS::GuiText journalleft;

	GuiNS::GuiText journalright;
	GuiNS::GuiText title;
	GuiNS::GuiText entry;
	GuiNS::GuiText pageinfo;

	GuiNS::GuiRectangleSprite prevpage;
	GuiNS::GuiRectangleSprite nextpage;

	float timer;
	GuiNS::GuiRectangle image;

	std::vector<JournalCharacter> characters;
	std::vector<JournalEntry> entries;

	int currentcharacter;
	std::wstring currentyentry;
};