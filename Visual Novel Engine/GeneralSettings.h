#pragma once

#include "SaveGui.h"

class OptionsSubType : public GuiNS::GuiElementObserver
{
public:
	virtual ~OptionsSubType() {}
	virtual void enable(GuiNS::Gui*gui)
	{
	}

	virtual void disable(GuiNS::Gui*gui)
	{
	}
};

class GeneralSettings : public OptionsSubType
{
public:
	GeneralSettings(State*fatherstate) :fatherstate(fatherstate),
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

	void notifyEvent(GuiNS::MyEvent event, GuiNS::GuiElement * from);
private:
	GuiNS::GuiText volumeinfo;
	GuiNS::GuiBar volumebar;
	State*fatherstate;
};


class OptionsSaveSubType : public OptionsSubType
{
public:
	OptionsSaveSubType(State*fatherstate):fatherstate(fatherstate), savemanager(3, 3, this)
	{
	}
	virtual void saveMangerEvent(int slot) = 0;
protected:
	GuiSaveManager savemanager;
	State*fatherstate;
};

class SaveSettings : public OptionsSaveSubType
{
public:
	SaveSettings(State*_fatherstate);

	virtual void enable(GuiNS::Gui*gui) override
	{
		savemanager.enable(gui);
	}

	virtual void disable(GuiNS::Gui*gui)override
	{
		savemanager.disable(gui);
	}

	void notifyEvent(GuiNS::MyEvent event, GuiNS::GuiElement * from)
	{

	}

	virtual void saveMangerEvent(int slot) override;

private:
	SaveData currentstate;
};

class LoadSettings : public OptionsSaveSubType
{
public:
	LoadSettings(State*_fatherstate) :
		OptionsSaveSubType(_fatherstate)
	{

	}

	virtual void enable(GuiNS::Gui*gui) override
	{
		savemanager.enable(gui);
	}

	virtual void disable(GuiNS::Gui*gui) override
	{
		savemanager.disable(gui);
	}

	void notifyEvent(GuiNS::MyEvent event, GuiNS::GuiElement * from)
	{

	}

	virtual void saveMangerEvent(int slot) override;

private:
};
