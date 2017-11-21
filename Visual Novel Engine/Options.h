#pragma once

#include "State.h"
#include "GeneralSettings.h"

enum Optiontype
{
	Menu,
	InGame
};

struct Options : public Gui::GuiElementObserver, public sf::Drawable
{
public:
	Options(sf::RenderWindow*window, Optiontype type, State*fatherstate);
	~Options();
	bool processEvent(sf::Event event);

	void sync(float time);

	virtual void notifyEvent(Gui::MyEvent event, Gui::GuiElement*from) override;

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
	Gui::GuiRectangle background;

	Gui::GuiText optionsbutton;
	Gui::GuiText savebutton;
	Gui::GuiText loadbutton;
	Gui::GuiText mainmenubutton;//In game - go to main menu
	Gui::GuiText returnbutton;

	OptionsSubType*subtype;

	Gui::Gui gui;
	bool visible;
	sf::RenderWindow*window;
	Optiontype type;

	State*fatherstate;
};