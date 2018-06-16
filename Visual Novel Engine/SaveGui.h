#pragma once

#include <fstream>

#include "State.h"
#include "Save.h"
#include "Gui.h"
#include "ResourceManager.h"
#include "Defines.h"

struct GuiSaveData
{
	GuiSaveData(sf::Vector2f offset, sf::Vector2f size, int x, int y, int padding, float innerpadding = 5) :
		desc(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 200), "", 40, 5, 5, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::NewLine),
		button(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 70), "", 50, 5, 5, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::NewLine),
		picture(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f((size.y - innerpadding * 2)*(16.0f / 9.0f), size.y-innerpadding*2.0f)),
		background(ResourceManager::getStyle(), size)
	{
		sf::Vector2f pos = offset + sf::Vector2f((size.x + padding) * x, (size.y + padding)*y);
		background.setPosition(pos);
		picture.setPosition(pos + sf::Vector2f(innerpadding,innerpadding));
		desc.setPosition(pos + sf::Vector2f(picture.getSize().x,innerpadding));
		desc.setSize(sf::Vector2f(size.x-picture.getSize().x-innerpadding*2.0f, picture.getSize().y));
		button.setPosition(sf::Vector2f(pos.x+size.x-button.getSize().x-innerpadding, pos.y+size.y-button.getSize().y - innerpadding));

		desc.setClickable(false);
		background.setClickable(false);
		picture.setClickable(false);

		button.changeBackground()->setOutlineThickness(2);
		button.changeBackground()->setOutlineColor(sf::Color::White);

		clear();
		loaded = false;
	}


	void clear()
	{
		picture.changeRectangle()->setTexture(NULL);
		desc.setString("");
		loaded = false;
	}

	void setSave(SaveData tmp)
	{
		texture.loadFromFile("Data\\Save\\" + std::to_string(tmp.slot) + ".png");
		picture.changeRectangle()->setTexture(&texture);
		desc.setString(tmp.date + L'\n' + tmp.desc);
		loaded = true;
	}


	void enable(GuiNS::Gui*gui, bool save, bool del = false)
	{
		if (del&&loaded)
		{
			button.setClickable(true);
			button.setString("Delete");
		}
		else if (loaded&&!save)
		{
			button.setClickable(true);
			button.setString("Load");
		}
		else if (save)
		{
			button.setClickable(true);
			button.setString("Save");
		}
		else 
		{
			button.setClickable(false);
			button.setString("Empty");
		}

		gui->addElement(&background);
		gui->addElement(&picture);
		gui->addElement(&desc);
		gui->addElement(&button);

	}
	void disable(GuiNS::Gui*gui)
	{
		gui->eraseElement(&background);
		gui->eraseElement(&picture);
		gui->eraseElement(&desc);
		gui->eraseElement(&button);
	}

	
	sf::Texture texture;
	GuiNS::GuiRectangle background;
	GuiNS::GuiRectangle picture;
	GuiNS::GuiText button;
	GuiNS::GuiText desc;
	bool loaded;
};

class OptionsSaveSubType;


class GuiSaveManager : public GuiNS::GuiElementObserver
{
public:
	GuiSaveManager(OptionsSaveSubType*fatherstate, unsigned int rows = 3, unsigned int columns = 2, sf::Vector2f pos = sf::Vector2f(10, 170), float width = 1800) :
		deletebutton(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(250, 70), "Delete", 50, 1, 10, GuiNS::GuiText::FormatVer::Ver_Down, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		currentpage(0),
		rows(rows),
		columns(columns),
		fatherstate(fatherstate)
	{
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

		//////////////////////POSITIONING GUI/////////////////////

		deletebutton.setPosition(sf::Vector2f(gamesize.x*0.5f - deletebutton.getSize().x*0.5f, gamesize.y - deletebutton.getSize().y - 10.0f));
		deletebutton.setObserver(this);


		{
			sf::Vector2f pagespos(810.0f + pos.x, pos.y - 95.0f);
			sf::Vector2f size(40.0f, 40.0f);
			float padding = 5.0f;


			int nrofpages = 20;
			for (int i = 0; i < nrofpages; i++)
			{
				pages.push_back(new GuiNS::GuiText(ResourceManager::getStyle(), *ResourceManager::getFont(), size, 
					std::to_string(i+1), 30, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing));
				pages[i]->setPosition(pagespos + sf::Vector2f((size.x + padding) * i, 0));
				pages[i]->setObserver(this);
			}

			pages.push_back(new GuiNS::GuiText(ResourceManager::getStyle(), *ResourceManager::getFont(), size,
				L'Q', 30, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing));
			pages.back()->setPosition(pagespos + sf::Vector2f((size.x + padding) * (nrofpages) + padding*4.0f, 0));
			pages.back()->setObserver(this);

			pages.push_back(new GuiNS::GuiText(ResourceManager::getStyle(), *ResourceManager::getFont(), size,
				L'A', 30, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing));
			pages.back()->setPosition(pagespos + sf::Vector2f((size.x + padding) * (nrofpages+1) + padding*4.0f, 0));
			pages.back()->setObserver(this);
		

			
		}



		{
			int padding = 20;

			float widthavilable = width - (columns - 1)*padding;
			sf::Vector2f size = sf::Vector2f(widthavilable / columns, 230);

			for (unsigned int y = 0; y < rows; y++)
				for (unsigned int x = 0; x < columns; x++)
				{
					savesgui.push_back(GuiSaveData(pos, size, x, y, padding));
					savesgui.back().desc.setObserver(this);
					savesgui.back().button.setObserver(this);
				}
			changePage(1);
		}

		gui = nullptr;
	}

	~GuiSaveManager()
	{
		saveDataToFile();
		for (unsigned int i = 0; i < pages.size(); i++)
			delete pages[i];
		pages.clear();
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from);

	void changePage(int newpage, bool del = false)
	{
		if (newpage < 1)
			newpage = 1;
		else if (newpage > 99)
			newpage = 99;

		if (currentpage-1 >= 0 && currentpage-1 < int(pages.size()))
		{
			pages[currentpage - 1]->setClickable(true);
			pages[currentpage - 1]->move(sf::Vector2f(0.0f, 5.0f));
		}
		if (newpage-1 >= 0 && newpage-1 < int(pages.size()))
		{
			pages[newpage - 1]->setClickable(false);
			pages[newpage - 1]->move(sf::Vector2f(0.0f, -5.0f));
		}

		for (unsigned int i = 0; i < savesgui.size(); i++)
			savesgui[i].clear();

		for (unsigned int i = 0; i < savedata.size(); i++)
			if (savedata[i].slot >= (newpage - 1)*rows*columns&&savedata[i].slot < (newpage)*rows*columns)
			{
				savesgui[savedata[i].slot % (rows*columns)].setSave(savedata[i]);
			}


		if (gui != nullptr)
		{
			for (unsigned int i = 0; i < savesgui.size(); i++)
			{
				savesgui[i].disable(gui);
				savesgui[i].enable(gui, save, del);
			}
		}

		currentpage = newpage;
	}

	void enable(GuiNS::Gui*_gui, bool _save)
	{
		gui = _gui;
		if (gui == nullptr)
			return;

		for (unsigned int i = 0; i < savesgui.size(); i++)
			savesgui[i].enable(gui, _save);
		save = _save;

		gui->addElement(&deletebutton);
		for (unsigned int i = 0; i < pages.size(); i++)
			gui->addElement(pages[i]);
	}

	void disable(GuiNS::Gui*_gui)
	{
		gui = _gui;
		if (gui == nullptr)
			return;


		for (unsigned int i = 0; i < savesgui.size(); i++)
			savesgui[i].disable(gui);

		gui->eraseElement(&deletebutton);
		for (unsigned int i = 0; i < pages.size(); i++)
			gui->eraseElement(pages[i]);
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

	void deleteSaveData(int slot)
	{
		for (int i = savedata.size()-1; i>=0; i--)
		{
			if (savedata[i].slot == slot)
			{
				savedata.erase(savedata.begin() + i);
				saveDataToFile();
				changePage(currentpage);
				return;
			}
		}
	}



	SaveData getSaveData(int slot)
	{
		for (unsigned int i = 0; i < savedata.size(); i++)
			if (slot == savedata[i].slot)
				return savedata[i];
		return SaveData();
	}

	bool hasSaveData(int slot)
	{
		for (unsigned int i = 0; i < savedata.size(); i++)
			if (slot == savedata[i].slot)
				return true;
		return false;
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
	const unsigned int rows;
	const unsigned int columns;
	std::vector <GuiSaveData> savesgui;

	std::vector <SaveData> savedata;


	std::vector <GuiNS::GuiText*> pages;

	int currentpage;
	bool save;
	OptionsSaveSubType*fatherstate;
	GuiNS::Gui*gui;

	GuiNS::GuiText deletebutton;
};