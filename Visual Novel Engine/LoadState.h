#pragma once

#include "State.h"
#include <fstream>

class LoadState : public State
{
public:
	LoadState(Engine*engine, int nroflogos = 2);

	virtual bool processEvent(sf::Event event);
	virtual void sync(float time) override;
	virtual void draw() override
	{
		getWindow()->draw(loadedbar);
		getWindow()->draw(notloadedbar);
		getWindow()->draw(logo);
	}

private:
	//Loading
	int loaded;
	std::vector<std::string> to_load;
	sf::Sprite loadedbar;
	sf::Sprite notloadedbar;

	//Logos
	void nextLogo();

	thor::BigSprite logo;

	float timer;
	int currentnr;
	int nroflogos;
};