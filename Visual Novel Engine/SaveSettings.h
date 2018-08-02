#pragma once

#include "SaveGui.h"
#include "OptionsSubtype.h"

class OptionsSaveSubType : public OptionsSubType
{
public:
	OptionsSaveSubType(Options*options, bool save);
	virtual void saveMangerEvent(int slot, bool del) = 0;
	virtual void saveMangerCopyEvent(int from, int to);

	void save();
	void load();
	void delet();
	void copy();

protected:
	void deletePopup(int slot);
	GuiSaveManager savemanager;
	int lastclicked;
	int lastclicked2;
	GuiNS::GuiText pagename;
};

class SaveSettings : public OptionsSaveSubType
{
public:
SaveSettings(Options * options) : OptionsSaveSubType(options, true)
	{
		pagename.setString("Save");
		pagename.fitBackground(true, false);
		savemanager.setDefaultDescription(L"Select a slot to save on.");
	}

	virtual void enable(GuiNS::Gui*gui) override
	{
		savemanager.enable(gui);
		gui->addElement(&pagename);
	}

	virtual void disable(GuiNS::Gui*gui)override
	{
		savemanager.disable(gui);
		gui->eraseElement(&pagename);
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
	{

	}

	virtual OptionsSubTypeEnum getSubtype()
	{
		return OptionsSubTypeEnum::Save_ST;
	}

	virtual void saveMangerEvent(int slot, bool del) override;
private:
};

class LoadSettings : public OptionsSaveSubType
{
public:
	LoadSettings(Options*options) : OptionsSaveSubType(options, false)
	{
		pagename.setString("Load");
		pagename.fitBackground(true, false);
		savemanager.setDefaultDescription(L"Select a slot to load from.");
	}

	virtual void enable(GuiNS::Gui*gui) override
	{
		savemanager.enable(gui);
		gui->addElement(&pagename);
	}

	virtual void disable(GuiNS::Gui*gui) override
	{
		savemanager.disable(gui);
		gui->eraseElement(&pagename);
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
	{

	}

	virtual OptionsSubTypeEnum getSubtype()
	{
		return OptionsSubTypeEnum::Load_ST;
	}

	virtual void saveMangerEvent(int slot, bool del) override;
private:
};
