#include "GeneralSettings.h"
#include "GameState.h"
#include "Engine.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <time.h>

GeneralSettings::~GeneralSettings()
{
	fatherstate->getEngine()->getSettings()->saveToFile();
}

void GeneralSettings::notifyEvent(GuiNS::MyEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::MyEvent::BarValueChanged&&from == &volumebar)
	{
		fatherstate->getEngine()->getSettings()->changeVolume(event.bar.state);
		SoundEngine::playSound("hover");
	}
}


SaveSettings::SaveSettings(State * _fatherstate) :
	OptionsSaveSubType(_fatherstate)
{
	currentstate = reinterpret_cast<GameState*>(_fatherstate)->getSave();
}

void SaveSettings::saveMangerEvent(int slot)
{
	currentstate.slot = slot;
	auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm timeinfo;

	localtime_s(&timeinfo, &in_time_t);

	std::wstringstream ss;
	ss << std::put_time(&timeinfo, L"%Y-%m-%d %X");
	currentstate.date = ss.str();


	sf::Vector2i size(355,200);
	auto&tmp = reinterpret_cast<GameState*>(fatherstate)->getCapturedScreenTexture();
	sf::Sprite tmp2(tmp);
	tmp2.setScale((float)size.x / tmp.getSize().x, (float)size.y / tmp.getSize().y);
	sf::RenderTexture tmp3;
	tmp3.create(size.x, size.y);
	tmp3.clear();
	tmp3.draw(tmp2);
	tmp3.display();
	tmp3.getTexture().copyToImage().saveToFile("Data\\Save\\" + std::to_string(slot) + ".png");
	OptionsSaveSubType::savemanager.addSaveData(currentstate);
}

void LoadSettings::saveMangerEvent(int slot)
{
	auto tmp = OptionsSaveSubType::savemanager.getSaveData(slot);
	if (tmp != nullptr)
		fatherstate->setNewState(new GameState(fatherstate->getEngine(), tmp));
	else;//TO DO: cannot load empty save data
}