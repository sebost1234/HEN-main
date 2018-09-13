 #pragma once

#include <fstream>
#include <cstdio>

#include "State.h"
#include "Save.h"
#include "Gui.h"
#include "ResourceManager.h"
#include "Defines.h"

struct GuiSaveData
{
	GuiSaveData(sf::Vector2f offset, sf::Vector2f size, int x, int y, int padding, float innerpadding = 10) :
		desc(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 200), "", 45, 5, 5, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::NewLine),
		picture(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f( float(int((size.y - innerpadding * 2.0)*(16.0f / 9.0f))), size.y-innerpadding*2.0f)),
		pictureborder(GuiNS::GuiRectangle(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(float(int((size.y - innerpadding * 2.0)*(16.0f / 9.0f))), size.y - innerpadding*2.0f)),
			"SaveImageBorder.png", "SaveImageBorder.png", "SaveImageBorder.png", "Data\\"),
		button(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), "Copy", 45, 5, 5, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
			"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5),
		background(ResourceManager::getStyle(StyleTypes::blankwhite), size)
	{
		sf::Vector2f pos = offset + sf::Vector2f((size.x + padding) * x, (size.y + padding)*y);
		pos.x = float(int(pos.x));
		pos.y = float(int(pos.y));

		background.setPosition(pos);
		picture.setPosition(pos + sf::Vector2f(innerpadding,innerpadding));
		pictureborder.setPosition(picture.getPosition());
		desc.setPosition(pos + sf::Vector2f(picture.getSize().x+innerpadding*2.0f,innerpadding));
		desc.setSize(sf::Vector2f(size.x-picture.getSize().x-innerpadding*2.0f, picture.getSize().y));
		button.setPosition(sf::Vector2f(pos.x+size.x-button.getSize().x-innerpadding, pos.y+size.y-button.getSize().y - innerpadding));

		desc.setClickable(false);
		background.setClickable(false);
		picture.setClickable(false);
		pictureborder.setClickable(false);

		background.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\SaveBackground.png"));

		clear();
		loaded = false;
	}


	void clear()
	{
		picture.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\EmptySavePicture.png"));
		desc.setString("");
		loaded = false;
	}

	void setSave(SaveData tmp)
	{
		picture.changeRectangle()->setTexture(ResourceManager::getSaveTexture(tmp.slot));
		desc.setString(tmp.date + L"\n" + tmp.desc);
		loaded = true;
	}


	void sync(bool save, bool del, int copy)
	{
		if (copy == -1)
		{
			if (loaded)
			{
				button.setClickable(true);
				button.setString("Copy");
			}
			else
			{
				button.setClickable(false);
				button.setString("Empty");
			}
		}
		else if (copy >= 0)
		{
			button.setClickable(true);
			button.setString("Copy");
		}
		else if (del&&loaded)
		{
			button.setClickable(true);
			button.setString("Delete");
		}
		else if (loaded && !save && !del)
		{
			button.setClickable(true);
			button.setString("Load");
		}
		else if (save && !del)
		{
			button.setClickable(true);
			button.setString("Save");
		}
		else
		{
			button.setClickable(false);
			button.setString("Empty");
		}
	}

	void enable(GuiNS::Gui*gui)
	{
		gui->addElement(&background);
		gui->addElement(&picture);
		gui->addElement(&pictureborder);
		gui->addElement(&desc);
		gui->addElement(&button);
	}
	void disable(GuiNS::Gui*gui)
	{
		gui->eraseElement(&background);
		gui->eraseElement(&picture);
		gui->eraseElement(&pictureborder);
		gui->eraseElement(&desc);
		gui->eraseElement(&button);
	}

	GuiNS::GuiRectangle background;
	GuiNS::GuiRectangle picture;
	GuiNS::GuiRectangleSprite pictureborder;
	GuiNS::GuiTextSprite button;
	GuiNS::GuiText desc;
	bool loaded;
};

class OptionsSaveSubType;


class GuiSaveManager : public GuiNS::GuiElementObserver
{
public:
	GuiSaveManager(OptionsSaveSubType*fatherstate, bool save, unsigned int rows = 3, unsigned int columns = 2, sf::Vector2f pos = sf::Vector2f(20, 170), float width = 1880) :
		description(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(230, 70), "", 50, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		deletebutton(GuiNS::GuiRectangle(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(100, 100)), "DeleteButtonNormal.png", "DeleteButtonHover.png", "DeleteButtonHover.png", "Data\\"),
		copybutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), "Copy", 45, 5, 5, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
			"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5),
		currentpage(0),
		rows(rows),
		columns(columns),
		fatherstate(fatherstate),
		gui(gui),
		save(save),
		delet(false),
		copyslot(-2)
	{
		////////////////////////LOADING SAVE DATA////////////////////
		std::string path = "Data\\Save\\save.czpal";
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

		{
			sf::Vector2f pagespos(810.0f + pos.x - 20.0f + 85.0f, pos.y - 60.0f);
			sf::Vector2f size(40.0f, 40.0f);
			float padding = 5.0f;


			const int nrofpages = 20;
			for (int i = 0; i < nrofpages; i++)
			{
				pages.push_back(new GuiNS::GuiTextSprite(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), size,
					std::to_string(i+1), 30, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
					"PageNumberNormal.png", "PageNumberHover.png", "PageNumberHover.png", "Data//"
					));
				pages[i]->setPosition(pagespos + sf::Vector2f((size.x + padding) * i, 0));
				pages[i]->setObserver(this);
			}

			pages.push_back(new GuiNS::GuiTextSprite(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), size,
				L'Q', 30, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
				"PageNumberNormal.png", "PageNumberHover.png", "PageNumberHover.png", "Data//"
				));
			pages.back()->setPosition(pagespos + sf::Vector2f((size.x + padding) * (nrofpages) + padding*4.0f, 0));
			pages.back()->setObserver(this);

			pages.push_back(new GuiNS::GuiTextSprite(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), size,
				L'A', 30, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
				"PageNumberNormal.png", "PageNumberHover.png", "PageNumberHover.png", "Data//"
				));
			pages.back()->setPosition(pagespos + sf::Vector2f((size.x + padding) * (nrofpages+1) + padding*4.0f, 0));
			pages.back()->setObserver(this);
		

			
		}



		{
			int padding = 20;

			float widthavilable = width - (columns - 1)*padding;
			sf::Vector2f size = sf::Vector2f(widthavilable / columns, 240);

			for (unsigned int y = 0; y < rows; y++)
				for (unsigned int x = 0; x < columns; x++)
				{
					savesgui.push_back(GuiSaveData(pos, size, x, y, padding));
					savesgui.back().desc.setObserver(this);
					savesgui.back().button.setObserver(this);
				}
			
			deletebutton.setPosition(sf::Vector2f(pos.x + padding + size.x + padding, gamesize.y - deletebutton.getSize().y - padding));
			deletebutton.setObserver(this);

			copybutton.setPosition(sf::Vector2f(deletebutton.getPosition().x + deletebutton.getSize().x + padding, gamesize.y - copybutton.getSize().y - padding*2));
			copybutton.setObserver(this);

			description.setSize(sf::Vector2f(size.x, 100.0f));
			description.setPosition(sf::Vector2f(pos.x, gamesize.y - description.getSize().y - padding));
			description.setObserver(this);

			changePage(1);
		}
	}

	~GuiSaveManager()
	{
		saveDataToFile();
		for (unsigned int i = 0; i < pages.size(); i++)
			delete pages[i];
		pages.clear();
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from);

	void changePage(int newpage)
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

		for (unsigned int i = 0; i < savesgui.size(); i++)
		{
			savesgui[i].sync(save, delet, copyslot);
		}
		

		currentpage = newpage;
	}

	void enable(GuiNS::Gui*_gui)
	{
		gui = _gui;
		if (gui == nullptr)
			return;

		for (unsigned int i = 0; i < savesgui.size(); i++)
			savesgui[i].enable(gui);

		gui->addElement(&deletebutton);
		for (unsigned int i = 0; i < pages.size(); i++)
			gui->addElement(pages[i]);

		gui->addElement(&description);
		gui->addElement(&copybutton);
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

		gui->eraseElement(&description);
		gui->eraseElement(&copybutton);
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
				ResourceManager::deleteSaveTexture(slot);
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
		std::string path = "Data\\Save\\save.czpal";

		std::wfstream file;

		std::locale  defaultLocale("");
		file.imbue(defaultLocale);

		file.open(path, std::ios::trunc | std::ios::out);
		file << savedata.size() << L'\n';
		for (unsigned int i = 0; i < savedata.size(); i++)
			file << savedata[i];
		file.close();
	}

	void setDefaultDescription(std::wstring string)
	{
		defaultdescription = string;
		description.setString(defaultdescription);
	}
private:
	const unsigned int rows;
	const unsigned int columns;
	std::vector <GuiSaveData> savesgui;

	std::vector <SaveData> savedata;


	std::vector <GuiNS::GuiTextSprite*> pages;

	int currentpage;
	bool save;
	int copyslot;
	bool delet;
	OptionsSaveSubType*fatherstate;
	GuiNS::Gui*gui;

	GuiNS::GuiRectangleSprite deletebutton;
	GuiNS::GuiText description;
	GuiNS::GuiTextSprite copybutton;
	std::wstring defaultdescription;
};