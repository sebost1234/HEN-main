#pragma once

#include "State.h"
#include "Options.h"

#include "Warnings.h"
#include "VisualNovelControler.h"
#include "FXengine.h"
#include "Save.h"

#include "Scene.h"


class GameState : public State, public Gui::GuiElementObserver
{
public:
	GameState(Engine*engine, SaveData*data = nullptr);

	virtual bool processEvent(sf::Event event);
	virtual void sync(float time) override;
	virtual void draw() override;

	virtual void notifyEvent(Gui::MyEvent event, Gui::GuiElement * from) override;
	SaveData getSave()
	{
		SaveData data = vnc.createSaveBase();
		scene.save(data);
		fxengine.save(data);
		return data;
	}

	sf::Image getCapturedScreen()
	{
		return fxengine.getTexture()->getTexture().copyToImage();
	}
	sf::Texture getCapturedScreenTexture()
	{
		return fxengine.getTexture()->getTexture();
	}
private:
	
	bool process(VisualNovelEvent event);

	//Gui
	bool hidegui;
	Gui::Gui gui;

	Gui::GuiTextSlowShow tekst;
	Gui::GuiText name;

	std::vector <Gui::GuiText*> choices;

	Options options;

	//Game
	Scene scene;

	VisualNovelControler vnc;
	bool processing;
	float timer;

	//FX
	FXengine fxengine;
};