#include "StartState.h"
#include "MainMenuState.h"
#include "Engine.h"

void StartState::sync(float time)
{
	timer += time;
	int value = int(abs(sin(timer) * 255));
	logo.setColor(sf::Color(value, value, value));
	if (timer > 3.14)
	{
		nextLogo();
		timer = 0;
	}
}

void StartState::nextLogo()
{
	currentnr++;
	if (currentnr > nroflogos)
	{
		setNewState(new MainMenuState(getEngine()));
		return;
	}
	logo.setTexture(*ResourceManager::getTexture("data\\Logo" + std::to_string(currentnr) + ".png"));
	logo.setOrigin(logo.getGlobalBounds().width / 2, logo.getGlobalBounds().height / 2);
	logo.setPosition(gamesize.x / 2.0f, gamesize.y / 2.0f);
}
