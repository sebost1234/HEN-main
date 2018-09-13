#include "SaveSettings.h"
#include "GameState.h"
#include "Options.h"
#include "Engine.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <time.h>



OptionsSaveSubType::OptionsSaveSubType(Options * options, bool save) : OptionsSubType(options), savemanager(this, save),
pagename(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(100, 70), "", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing)
{
	pagename.setPosition(sf::Vector2f(10, 100));
	pagename.setClickable(false);
	lastclicked = -1;
	lastclicked2 = -1;
	options->background.setTexture(ResourceManager::getBigTexture("Data//bgSave.png"));
}



void OptionsSaveSubType::save()
{
	if (lastclicked == -1)
		return;

	auto gamestate = reinterpret_cast<GameState*>(options->fatherstate);
	SaveData currentstate = gamestate->getSave();
	currentstate.slot = lastclicked;
	auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm timeinfo;

	localtime_s(&timeinfo, &in_time_t);

	std::wstringstream ss;
	ss << std::put_time(&timeinfo, L"%Y-%m-%d %X");
	currentstate.date = ss.str();

	auto&tmp = gamestate->getCapturedScreenTexture();
	sf::Vector2i size(426,240);
	sf::Sprite tmp2(tmp);
	tmp2.setScale((float)size.x / tmp.getSize().x, (float)size.y / tmp.getSize().y);
	sf::RenderTexture tmp3;
	tmp3.create(size.x, size.y);
	tmp3.clear();
	tmp3.draw(tmp2);
	tmp3.display();
	ResourceManager::deleteSaveTexture(lastclicked);
	tmp3.getTexture().copyToImage().saveToFile("Data\\Save\\" + std::to_string(lastclicked) + ".png");
	OptionsSaveSubType::savemanager.addSaveData(currentstate);
}

void OptionsSaveSubType::load()
{
	if (lastclicked == -1)
		return;

	if (OptionsSaveSubType::savemanager.hasSaveData(lastclicked))
		options->fatherstate->setNewState(new GameState(options->fatherstate->getEngine(), OptionsSaveSubType::savemanager.getSaveData(lastclicked)));
}

void OptionsSaveSubType::delet()
{
	if (lastclicked == -1)
		return;

	OptionsSaveSubType::savemanager.deleteSaveData(lastclicked);
}

void OptionsSaveSubType::copy()
{
	if (lastclicked == -1 || lastclicked2 == -1)
		return;

	if (OptionsSaveSubType::savemanager.hasSaveData(lastclicked))
	{
		SaveData tmp = OptionsSaveSubType::savemanager.getSaveData(lastclicked);
		tmp.slot = lastclicked2;

		sf::Image image;
		image.loadFromFile("Data\\Save\\" + std::to_string(lastclicked) + ".png");
		image.saveToFile("Data\\Save\\" + std::to_string(lastclicked2) + ".png");

		savemanager.addSaveData(tmp);
	}
}

void OptionsSaveSubType::saveMangerCopyEvent(int from, int to)
{
	lastclicked = from;
	lastclicked2 = to;
	if (OptionsSaveSubType::savemanager.hasSaveData(lastclicked2))
	{
		options->choicepopup.changeText("Are you sure you want to copy the save on a slot nr." + std::to_string(from + 1) + " to a slot nr." + std::to_string(to + 1) + " (You will override a save that's currently on that slot)?");
		options->localgui.setPopup(&options->choicepopup);
		options->choicepopup.changeId("copy");
	}
	else OptionsSaveSubType::copy();
}


void OptionsSaveSubType::deletePopup(int slot)
{
	if (OptionsSaveSubType::savemanager.hasSaveData(slot))
	{
		lastclicked = slot;
		std::string tmp;
		tmp += "Are you sure you want to delete a save a slot nr." + std::to_string(slot+1) + "?";
		tmp += "(You can't undo this)";
		options->choicepopup.changeText(tmp);
		options->choicepopup.changeId("delete");
		options->localgui.setPopup(&options->choicepopup);
	}
}

void SaveSettings::saveMangerEvent(int slot, bool del)
{
	if (!del)
	{
		lastclicked = slot;
		options->choicepopup.changeText("Are you sure you want to save the game on a slot nr." + std::to_string(slot+1) + "?");
		options->localgui.setPopup(&options->choicepopup);
		options->choicepopup.changeId("save");
	}
	else deletePopup(slot);
}

void LoadSettings::saveMangerEvent(int slot, bool del)
{
	if (!del)
	{
		if (OptionsSaveSubType::savemanager.hasSaveData(slot))
		{
			lastclicked = slot;
			std::string tmp;
			tmp += "Are you sure you want to load on a slot nr." + std::to_string(slot+1) + "?";
			if (options->type == Optiontype::InGame)
				tmp += "(Any unsaved progress will be lost)";
			options->choicepopup.changeText(tmp);
			options->choicepopup.changeId("load");
			options->localgui.setPopup(&options->choicepopup);
		}
	}
	else deletePopup(slot);
}
