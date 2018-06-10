#pragma once

#include "State.h"
#include "GameState.h"
#include "Options.h"

class MainMenuState : public State, GuiNS::GuiElementObserver
{
public:
	MainMenuState(Engine*engine);

	virtual bool processEvent(sf::Event event);
	virtual void sync(float time) override;
	virtual void draw() override;


	virtual void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement*from) override;
private:
	GuiNS::Gui gui;

	sf::RectangleShape background;
	GuiNS::GuiText startbutton;
	GuiNS::GuiText loadbutton;
	GuiNS::GuiText optionsbutton;
	GuiNS::GuiText exitbutton;

	GuiNS::GuiText testbutton;

	Options options;
}; 