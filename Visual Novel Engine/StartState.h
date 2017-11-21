#pragma once

#include "State.h"

class StartState : public State
{
public:
	StartState(Engine*engine, int nroflogos = 3):State(engine), nroflogos(nroflogos)
	{
		timer = 0;
		currentnr = 0;
		nextLogo();
	}

	virtual bool processEvent(sf::Event event)
	{
		return false;
	}
	virtual void sync(float time) override;
	virtual void draw() override
	{
		getWindow()->draw(logo);
	}

private:

	void nextLogo();

	sf::Sprite logo;

	float timer;
	int currentnr;
	int nroflogos;
};