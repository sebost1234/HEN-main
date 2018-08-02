#pragma once

#include "OptionsSubtype.h"

class GeneralSettings : public OptionsSubType
{
public:
	GeneralSettings(Options*options);
	~GeneralSettings();

	virtual void enable(GuiNS::Gui*gui) override
	{
		gui->addElement(&pagename);
		gui->addElement(&volumeinfo);
		gui->addElement(&windowmodeinfo);

		gui->addElement(&checkboxfullscreen);
		gui->addElement(&checkboxwindowed);

		gui->addElement(&checkboxfullscreeninfo);
		gui->addElement(&checkboxwindowedinfo);

		gui->addElement(&mastervolumeinfo);
		gui->addElement(&mastervolumebar);

		gui->addElement(&bgmvolumeinfo);
		gui->addElement(&bgmvolumebar);

		gui->addElement(&sfxvolumeinfo);
		gui->addElement(&sfxvolumebar);

		gui->addElement(&systemvolumeinfo);
		gui->addElement(&systemvolumebar);


		gui->addElement(&gameconfiginfo);

		gui->addElement(&textspeedinfo);
		gui->addElement(&textspeedbar);

		gui->addElement(&checkboxskipinfo);
		gui->addElement(&checkboxskip);

		gui->addElement(&defaultsbutton);
	}

	virtual void disable(GuiNS::Gui*gui)override
	{
		gui->eraseElement(&pagename);
		gui->eraseElement(&volumeinfo);
		gui->eraseElement(&windowmodeinfo);

		gui->eraseElement(&checkboxfullscreen);
		gui->eraseElement(&checkboxwindowed);

		gui->eraseElement(&checkboxfullscreeninfo);
		gui->eraseElement(&checkboxwindowedinfo);

		gui->eraseElement(&mastervolumeinfo);
		gui->eraseElement(&mastervolumebar);

		gui->eraseElement(&bgmvolumeinfo);
		gui->eraseElement(&bgmvolumebar);

		gui->eraseElement(&sfxvolumeinfo);
		gui->eraseElement(&sfxvolumebar);

		gui->eraseElement(&systemvolumeinfo);
		gui->eraseElement(&systemvolumebar);


		gui->eraseElement(&gameconfiginfo);

		gui->eraseElement(&textspeedinfo);
		gui->eraseElement(&textspeedbar);

		gui->eraseElement(&checkboxskipinfo);
		gui->eraseElement(&checkboxskip);

		gui->eraseElement(&defaultsbutton);
	}

	virtual OptionsSubTypeEnum getSubtype()
	{
		return OptionsSubTypeEnum::Options_ST;
	}

	void sync()
	{
		syncFullscreen();
		syncSkipping();
		syncBars();
	}
	
	void syncFullscreen();
	void syncSkipping();
	void syncBars();

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from);
private:

	GuiNS::GuiText windowmodeinfo;

	GuiNS::GuiRectangle checkboxfullscreen;
	GuiNS::GuiText checkboxfullscreeninfo;

	GuiNS::GuiRectangle checkboxwindowed;
	GuiNS::GuiText checkboxwindowedinfo;


	GuiNS::GuiText volumeinfo;

	GuiNS::GuiText mastervolumeinfo;
	GuiNS::GuiBarSprite mastervolumebar;

	GuiNS::GuiText bgmvolumeinfo;
	GuiNS::GuiBarSprite bgmvolumebar;

	GuiNS::GuiText sfxvolumeinfo;
	GuiNS::GuiBarSprite sfxvolumebar;

	GuiNS::GuiText systemvolumeinfo;
	GuiNS::GuiBarSprite systemvolumebar;

	GuiNS::GuiText pagename;



	GuiNS::GuiText gameconfiginfo;

	GuiNS::GuiText textspeedinfo;
	GuiNS::GuiBarSprite textspeedbar;

	GuiNS::GuiRectangle checkboxskip;
	GuiNS::GuiText checkboxskipinfo;

	GuiNS::GuiTextSprite defaultsbutton;
};
