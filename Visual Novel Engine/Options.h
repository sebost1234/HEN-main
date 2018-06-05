#pragma once

#include "State.h"
#include "GeneralSettings.h"

enum Optiontype
{
	Menu,
	InGame
};

struct Options : public GuiNS::GuiElementObserver, public sf::Drawable
{
public:
	Options(sf::RenderWindow*window, Optiontype type, State*fatherstate);
	~Options();
	bool processEvent(sf::Event event);

	void sync(float time);

	virtual void notifyEvent(GuiNS::MyEvent event, GuiNS::GuiElement*from) override;

	void setVisible(bool state);
	inline bool isVisible() const
	{
		return visible;
	}
	void toggle()
	{
		if (visible)
			setVisible(false);
		else
			setVisible(true);
	}

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void changeSubType(OptionsSubType*newsubtype)
	{
		if (subtype != nullptr)
		{
			subtype->disable(&gui);
			delete subtype;
		}
		subtype = newsubtype;
		subtype->enable(&gui);
	}

private:
	GuiNS::GuiRectangle background;

	GuiNS::GuiText optionsbutton;
	GuiNS::GuiText savebutton;
	GuiNS::GuiText loadbutton;
	GuiNS::GuiText mainmenubutton;//In game - go to main menu
	GuiNS::GuiText returnbutton;

	OptionsSubType*subtype;

	GuiNS::Gui gui;
	bool visible;
	sf::RenderWindow*window;
	Optiontype type;

	State*fatherstate;
};