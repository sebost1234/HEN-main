#pragma once

#include "State.h"
#include "GameState.h"
#include "Options.h"

class MainMenuState : public State, Gui::GuiElementObserver
{
public:
	MainMenuState(Engine*engine);

	virtual bool processEvent(sf::Event event);
	virtual void sync(float time) override;
	virtual void draw() override;


	virtual void notifyEvent(Gui::MyEvent event, Gui::GuiElement*from) override;
private:
	Gui::Gui gui;

	sf::RectangleShape background;
	Gui::GuiText startbutton;
	Gui::GuiText loadbutton;
	Gui::GuiText optionsbutton;
	Gui::GuiText exitbutton;

	Options options;
}; 