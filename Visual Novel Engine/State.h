#pragma once

#include <SFML\Graphics.hpp>
#include <algorithm>


#include "Defines.h"
#include "Gui.h"
#include "ResourceManager.h"


class Engine;

class State
{
public:
	State(Engine*engine) :engine(engine)
	{
		newstate = nullptr;
	}
	virtual ~State()
	{

	}

	virtual bool processEvent(sf::Event event) = 0;
	virtual void sync(float time) = 0;
	virtual void draw() = 0;

	void setNewState(State*state)
	{
		if (newstate != nullptr)
			delete newstate;
		newstate = state;
	}
	sf::RenderWindow*getWindow();
	Engine*getEngine()
	{
		return engine;
	}
private:
	friend class Engine;
	Engine*engine;
	State*newstate;
};
