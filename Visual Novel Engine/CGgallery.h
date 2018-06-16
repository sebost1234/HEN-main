#pragma once

#include "GeneralSettings.h"

class CGGallerySettings : public OptionsSubType
{
public:
	CGGallerySettings(Options*options) : OptionsSubType(options)
	{
		
	}

	virtual void enable(GuiNS::Gui*gui) override
	{
		
	}

	virtual void disable(GuiNS::Gui*gui)override
	{

	}

	virtual OptionsSubTypeEnum getSubtype()
	{
		return OptionsSubTypeEnum::Gallery_ST;
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from) {}
private:
};