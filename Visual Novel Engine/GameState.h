#pragma once

#include "State.h"
#include "Options.h"

#include "Warnings.h"
#include "VisualNovelControler.h"
#include "FXengine.h"
#include "Save.h"

#include "Scene.h"


class GameState : public State, public GuiNS::GuiElementObserver
{
public:
	GameState(Engine*engine, SaveData*data = nullptr);

	virtual bool processEvent(sf::Event event);
	virtual void sync(float time) override;
	virtual void draw() override;

	virtual void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from) override;
	SaveData getSave()
	{
		SaveData data = vnc.createSaveBase();
		data.toLoad.push_back(L"say;" + name.getString() + L";" + tekst.getString() + ";NS");
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
	GuiNS::Gui gui;

	GuiNS::GuiTextSlowShow tekst;
	GuiNS::GuiText name;

	std::vector <GuiNS::GuiText*> choices;

	Options options;

	//Game
	Scene scene;

	VisualNovelControler vnc;
	bool processing;
	float timer;

	//FX
	FXengine fxengine;
};