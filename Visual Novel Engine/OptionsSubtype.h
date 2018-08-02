#pragma once

#include "Gui.h"

class Options;

enum OptionsSubTypeEnum
{
	Save_ST,
	Load_ST,
	Options_ST,
	Gallery_ST,
	Journal_ST,
	Music_ST
};


class OptionsSubType : public GuiNS::GuiElementObserver
{
public:
	OptionsSubType(Options*options) :options(options)
	{

	}
	virtual ~OptionsSubType() {}
	virtual void enable(GuiNS::Gui*gui)
	{
	}

	virtual void disable(GuiNS::Gui*gui)
	{
	}

	virtual OptionsSubTypeEnum getSubtype() = 0;
protected:
	Options*options;
};

#include "GeneralSettings.h"
#include "SaveSettings.h"
#include "CGgallery.h"
#include "Journal.h"
#include "Music.h"