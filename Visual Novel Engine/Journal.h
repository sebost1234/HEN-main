#pragma once


#include "OptionsSubtype.h"

class Journal: public OptionsSubType
{
public:
	Journal(Options*options);

	virtual void enable(GuiNS::Gui*gui) override
	{

	}

	virtual void disable(GuiNS::Gui*gui)override
	{

	}

	virtual OptionsSubTypeEnum getSubtype()
	{
		return OptionsSubTypeEnum::Journal_ST;
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from) {}
private:
};