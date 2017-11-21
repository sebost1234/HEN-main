#pragma once

#include <fstream>

#include "State.h"
#include "Save.h"
#include "Gui.h"
#include "ResourceManager.h"
#include "Defines.h"

struct GuiSaveData
{
	GuiSaveData(sf::Vector2f offset, sf::Vector2f size, int x, int y, int paddingbetween, int paddingoutside) :
		desc(ResourceManager::getStyle(), *ResourceManager::getFont(), size, "", 25, 5, 5, Gui::GuiText::FormatVer::Ver_Top, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
		time(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(size.x ,size.y*0.25f), "", 25, 5, 5, Gui::GuiText::FormatVer::Ver_Top, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing)
	{
		desc.setPosition(offset + sf::Vector2f((size.x + paddingoutside) * x, (size.y + paddingoutside)*y));
		time.setPosition(offset + sf::Vector2f((size.x + paddingoutside) * x, (size.y + paddingoutside)*y) + sf::Vector2f(0.0f, size.y*0.75f));
		clear();
	}


	void clear()
	{
		desc.changeBackground()->setTexture(NULL);
		desc.setString("EMPTY");
		time.setString("");
	}

	void setSave(SaveData tmp)
	{
		texture.loadFromFile("Data\\Save\\" + std::to_string(tmp.slot) + ".png");
		desc.changeBackground()->setTexture(&texture);
		desc.setString(tmp.desc);
		time.setString(tmp.date);
	}

	void enable(Gui::Gui*gui)
	{
		gui->addElement(&time);
		gui->addElement(&desc);
	}
	void disable(Gui::Gui*gui)
	{
		gui->eraseElement(&desc);
		gui->eraseElement(&time);
	}


	sf::Texture texture;
	Gui::GuiText desc;
	Gui::GuiText time;
};

class OptionsSaveSubType;


class GuiSaveManager : public Gui::GuiElementObserver
{
public:
	GuiSaveManager(unsigned int rows, unsigned int columns, OptionsSaveSubType*fatherstate) :
		prevpagebutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Prev", 30, 5, 5, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Left, Gui::GuiText::Nothing),
		nextpagebutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Next", 30, 5, 5, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Left, Gui::GuiText::Nothing),
		pagenr(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "", 30, 5, 5, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Left, Gui::GuiText::Nothing),
		currentpage(0),
		rows(rows),
		columns(columns),
		fatherstate(fatherstate)
	{
		prevpagebutton.setObserver(this);
		nextpagebutton.setObserver(this);

		prevpagebutton.setPosition(sf::Vector2f(300, 50));
		nextpagebutton.setPosition(sf::Vector2f(gamesize.x - nextpagebutton.getSize().x - 100, 50));

		pagenr.setPrefix("Page Nr.");
		pagenr.setString("1");
		pagenr.setClickable(false);
		pagenr.setPosition(prevpagebutton.getPosition()*0.5f + nextpagebutton.getPosition()*0.5f - sf::Vector2f(pagenr.getSize().x*0.5f - nextpagebutton.getSize().x*0.5f, 0));

		////////////////////////LOADING SAVE DATA////////////////////
		std::string path = "Data\\Save\\save";
		std::wfstream file;
		file.open(path, std::ios::in);
		if (!file.is_open())
			std::throw_with_nested(std::runtime_error("ERROR 01: Failed to load game file: " + path));

		std::wstring tmp;
		std::getline(file, tmp, L'\n');
		int nr = stoi(tmp);
		for (int i = 0; i < nr; i++)
		{
			SaveData save;
			file >> save;
			savedata.push_back(save);
		}

		//////////////////////////////////////////////////////////////

		sf::Vector2f offset = sf::Vector2f(prevpagebutton.getPosition() + sf::Vector2f(0, prevpagebutton.getSize().y + 10));
		sf::Vector2f size = sf::Vector2f((gamesize.x - offset.x - 100 - 10 * (columns - 1)) / (3), 200);

		for (unsigned int y = 0; y < rows; y++)
			for (unsigned int x = 0; x < columns; x++)
			{
				savesgui.push_back(GuiSaveData(offset, size, x, y, 5, 10));
				savesgui.back().desc.setObserver(this);
			}
		changePage(1);
	}

	~GuiSaveManager()
	{
		saveDataToFile();
	}

	void notifyEvent(Gui::MyEvent event, Gui::GuiElement * from);

	void changePage(int newpage)
	{
		if (newpage < 1)
			newpage = 1;
		else if (newpage > 99)
			newpage = 99;

		pagenr.setString(std::to_string(newpage));

		for (unsigned int i = 0; i < savesgui.size(); i++)
			savesgui[i].clear();

		for (unsigned int i = 0; i < savedata.size(); i++)
			if (savedata[i].slot >= (newpage - 1)*rows*columns&&savedata[i].slot < (newpage)*rows*columns)
				savesgui[savedata[i].slot % (rows*columns)].setSave(savedata[i]);


		currentpage = newpage;
	}

	void enable(Gui::Gui*gui)
	{
		gui->addElement(&prevpagebutton);
		gui->addElement(&nextpagebutton);
		gui->addElement(&pagenr);

		for (unsigned int i = 0; i < savesgui.size(); i++)
			savesgui[i].enable(gui);
	}

	void disable(Gui::Gui*gui)
	{
		gui->eraseElement(&prevpagebutton);
		gui->eraseElement(&nextpagebutton);
		gui->eraseElement(&pagenr);

		for (unsigned int i = 0; i < savesgui.size(); i++)
			savesgui[i].disable(gui);
	}

	void addSaveData(SaveData data)
	{
		for (unsigned int i = 0; i<savedata.size(); i++)
		{
			if (savedata[i].slot == data.slot)
			{
				savedata[i] = data;
				saveDataToFile();
				changePage(currentpage);
				return;
			}
		}
		savedata.push_back(data);
		saveDataToFile();
		changePage(currentpage);
	}

	SaveData*getSaveData(int slot)
	{
		for (unsigned int i = 0; i < savedata.size(); i++)
			if(slot == savedata[i].slot)
			return new SaveData(savedata[i]);
		return nullptr;
	}

	void saveDataToFile()
	{
		std::string path = "Data\\Save\\save";
		std::wfstream file;
		file.open(path, std::ios::trunc | std::ios::out);
		file << savedata.size() << L'\n';
		for (unsigned int i = 0; i < savedata.size(); i++)
			file << savedata[i];
		file.close();
	}
private:
	Gui::GuiText prevpagebutton;
	Gui::GuiText nextpagebutton;

	Gui::GuiText pagenr;

	const unsigned int rows;
	const unsigned int columns;
	std::vector <GuiSaveData> savesgui;

	std::vector <SaveData> savedata;

	int currentpage;
	OptionsSaveSubType*fatherstate;
};