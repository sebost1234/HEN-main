#pragma once

#include "State.h"
#include "GameState.h"
#include "Options.h"

#include <Thor\Graphics.hpp>

class MainMenuState : public State, GuiNS::GuiElementObserver, GuiNS::GuiObserver
{
public:
	MainMenuState(Engine*engine);

	virtual bool processEvent(sf::Event event);
	virtual void sync(float time) override;
	virtual void draw() override;


	virtual void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement*from) override;
private:
	GuiNS::Gui gui;

	thor::BigSprite background;

	GuiNS::GuiText startbutton;
	GuiNS::GuiText loadbutton;
	GuiNS::GuiText optionsbutton;
	GuiNS::GuiText gallerybutton;
	GuiNS::GuiText journalbutton;
	GuiNS::GuiText exitbutton;

	Options options;

	// Inherited via GuiObserver
	virtual void notifyEvent(GuiNS::GuiEvent event, GuiNS::Gui * from) override;
};