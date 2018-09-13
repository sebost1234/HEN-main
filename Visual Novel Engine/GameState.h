#pragma once

#include "State.h"
#include "Options.h"

#include "Warnings.h"
#include "VisualNovelControler.h"
#include "FXengine.h"
#include "Save.h"

#include "Scene.h"


class GameState : public State, public GuiNS::GuiElementObserver, public GuiNS::GuiObserver
{
public:
	GameState(Engine*engine, SaveData data = SaveData());
	~GameState();

	virtual bool processEvent(sf::Event event);
	virtual void sync(float time) override;
	virtual void draw() override;

	// Inherited via GuiElementObserver
	virtual void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from) override;
	// Inherited via GuiObserver
	virtual void notifyEvent(GuiNS::GuiEvent event, GuiNS::Gui * from) override;

	SaveData getSave()
	{
		SaveData data = vnc.createSaveBase();
		data.toLoad.push_back(VisualNovelEvent(VisualNovelEvent::PlayMusic, { sf::String(currentmusic) }));
		data.toLoad.push_back(VisualNovelEvent(VisualNovelEvent::Say, { name.getString().toWideString(), tekst.getString().toWideString(), L"NS" }));
		scene.save(data);
		return data;
	}

	sf::Image getCapturedScreen()
	{
		return sf::Image();
	}
	sf::Texture getCapturedScreenTexture()
	{
		return sf::Texture();
	}

	void syncTextSpeed();
private:
	
	bool process(VisualNovelEvent event);

	//Gui
	bool hidegui;
	GuiNS::Gui gui;

	GuiNS::GuiTextSlowShow tekst;
	GuiNS::GuiText name;
	sf::Sprite textbackground;



	std::vector <GuiNS::GuiText*> choices;

	Options options;

	//Game
	Scene scene;

	std::string currentmusic;

	VisualNovelControler vnc;
	bool processing;
	float timer;
};