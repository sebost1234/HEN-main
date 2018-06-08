#pragma once

#include "Popup.h"
#include "State.h"
#include "GeneralSettings.h"

enum Optiontype
{
	Menu,
	InGame
};

struct Options : public GuiNS::GuiElementObserver, public GuiNS::Popup
{
public:
	Options(Optiontype type, State*fatherstate);
	~Options();

	virtual void notifyEvent(GuiNS::MyEvent event, GuiNS::GuiElement*from) override;


	bool sync(sf::Vector2f mousepos, float time) override //1 - delete 0 - don't
	{
		localgui.sync(mousepos, time);
		return to_delete;
	}

	bool processEvent(sf::Event event, sf::Vector2f mousepos) override
	{
		if (localgui.processEvent(event, mousepos))
		{
			if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Escape)
			{
				to_delete = true;
				return false;
			}
			return true;
		}
		return false;
	}

	void changeSubType(OptionsSubType*newsubtype)
	{
		if (subtype != nullptr)
		{
			subtype->disable(&localgui);
			delete subtype;
		}
		subtype = newsubtype;
		subtype->enable(&localgui);
	}

private:
	GuiNS::GuiRectangle background;

	GuiNS::GuiText optionsbutton;
	GuiNS::GuiText savebutton;
	GuiNS::GuiText loadbutton;
	GuiNS::GuiText mainmenubutton;//In game - go to main menu
	GuiNS::GuiText returnbutton;

	OptionsSubType*subtype;

	Optiontype type;

	State*fatherstate;
};