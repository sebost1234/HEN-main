#include "GeneralSettings.h"
#include "GameState.h"
#include "Options.h"
#include "Engine.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <time.h>


GeneralSettings::~GeneralSettings()
{
	options->fatherstate->getEngine()->getSettings()->saveToFile();
}

void GeneralSettings::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::GuiElementEvent::BarValueChanged&&from == &volumebar)
	{
		options->fatherstate->getEngine()->getSettings()->changeVolume(event.bar.state);
		SoundEngine::playSound("hover");
	}
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
	sf::Vector2i size(355,200);
	sf::Sprite tmp2(tmp);
	tmp2.setScale((float)size.x / tmp.getSize().x, (float)size.y / tmp.getSize().y);
	sf::RenderTexture tmp3;
	tmp3.create(size.x, size.y);
	tmp3.clear();
	tmp3.draw(tmp2);
	tmp3.display();
	tmp3.getTexture().copyToImage().saveToFile("Data\\Save\\" + std::to_string(lastclicked) + ".png");
	OptionsSaveSubType::savemanager.addSaveData(currentstate);
}

void OptionsSaveSubType::load()
{
	if (lastclicked == -1)
		return;

	auto tmp = OptionsSaveSubType::savemanager.getSaveData(lastclicked);
	if (tmp != nullptr)
		options->fatherstate->setNewState(new GameState(options->fatherstate->getEngine(), tmp));
}

void OptionsSaveSubType::delet()
{
	if (lastclicked == -1)
		return;

	OptionsSaveSubType::savemanager.deleteSaveData(lastclicked);
}

void OptionsSaveSubType::deletePopup(int slot)
{
	auto tmp = OptionsSaveSubType::savemanager.getSaveData(slot);
	if (tmp != nullptr)
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
		auto tmp = OptionsSaveSubType::savemanager.getSaveData(slot);
		if (tmp != nullptr)
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
