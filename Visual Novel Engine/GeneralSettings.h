#pragma once

#include "SaveGui.h"

class Options;

class OptionsSubType : public GuiNS::GuiElementObserver
{
public:
	OptionsSubType(Options*options):options(options)
	{

	}
	virtual ~OptionsSubType() {}
	virtual void enable(GuiNS::Gui*gui)
	{
	}

	virtual void disable(GuiNS::Gui*gui)
	{
	}
protected:
	Options*options;
};

class GeneralSettings : public OptionsSubType
{
public:
	GeneralSettings(Options*options) : OptionsSubType(options),
		volumeinfo(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Volume", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		volumebar(ResourceManager::getStyle(), sf::Vector2f(500, 50), GuiNS::GuiBar::Horizontal)
	{

		volumeinfo.setPosition(sf::Vector2f(300, 50));
		volumeinfo.setClickable(false);

		volumebar._changeMax(100);
		volumebar._changeState(int(sf::Listener::getGlobalVolume()));
		volumebar.setPosition(volumeinfo.getPosition() + sf::Vector2f(0, 10 + volumeinfo.getSize().y));
		volumebar.setObserver(this);

	}
	~GeneralSettings();

	virtual void enable(GuiNS::Gui*gui) override
	{
		gui->addElement(&volumebar);
		gui->addElement(&volumeinfo);
	}

	virtual void disable(GuiNS::Gui*gui)override
	{
		gui->eraseElement(&volumebar);
		gui->eraseElement(&volumeinfo);
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from);
private:
	GuiNS::GuiText volumeinfo;
	GuiNS::GuiBar volumebar;
};


class OptionsSaveSubType : public OptionsSubType
{
public:
	OptionsSaveSubType(Options*options) : OptionsSubType(options), savemanager(3, 3, this)
	{
		lastclicked = -1;
	}
	virtual void saveMangerEvent(int slot, bool del) = 0;

	void save();
	void load();
	void delet();

protected:
	void deletePopup(int slot);
	GuiSaveManager savemanager;
	int lastclicked;
};

class SaveSettings : public OptionsSaveSubType
{
public:
	SaveSettings(Options*options) : OptionsSaveSubType(options)
	{

	}

	virtual void enable(GuiNS::Gui*gui) override
	{
		savemanager.enable(gui, true);
	}

	virtual void disable(GuiNS::Gui*gui)override
	{
		savemanager.disable(gui);
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
	{

	}

	virtual void saveMangerEvent(int slot, bool del) override;
private:
};

class LoadSettings : public OptionsSaveSubType
{
public:
	LoadSettings(Options*options) : OptionsSaveSubType(options)
	{

	}

	virtual void enable(GuiNS::Gui*gui) override
	{
		savemanager.enable(gui, false);
	}

	virtual void disable(GuiNS::Gui*gui) override
	{
		savemanager.disable(gui);
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
	{

	}

	virtual void saveMangerEvent(int slot, bool del) override;
private:
};
