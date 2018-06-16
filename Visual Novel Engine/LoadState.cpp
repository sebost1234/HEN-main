#include "LoadState.h"
#include "MainMenuState.h"
#include "Engine.h"

LoadState::LoadState(Engine * engine, int nroflogos) :State(engine), nroflogos(nroflogos)
{
	//Loading
	loadedbar.setTexture(*ResourceManager::getTexture("Data\\LoadBarFull.png"));
	notloadedbar.setTexture(*ResourceManager::getTexture("Data\\LoadBarEmpty.png"));

	std::fstream file;
	file.open("Data\\TextureList.txt");
	if (file.is_open())
		while (!file.eof())
		{
			std::string path;
			std::getline(file, path, '\n');
			to_load.push_back(path);
		}
	loaded = 0;

	//Logos
	timer = 0;
	currentnr = 0;
	nextLogo();

	getEngine()->setCursorVisible(false);
}

bool LoadState::processEvent(sf::Event event)
{
	if (event.type == event.KeyPressed || event.type == event.MouseButtonPressed)
	{
		if (to_load.empty())
		{
			setNewState(new MainMenuState(getEngine()));
		}
	}
	return false;
}

void LoadState::sync(float time)
{
	//Loading
	if (!to_load.empty())
	{
		ResourceManager::getTexture(to_load.front());
		to_load.erase(to_load.begin());
		sf::Vector2u size = loadedbar.getTexture()->getSize();

		float progress = ((float)loaded) / (to_load.size() + loaded);
		progress *= size.x;

		int intprogress = int(progress);


		sf::Vector2f pos = sf::Vector2f((gamesize.x - size.x)*0.5f, gamesize.y*0.85f);

		loadedbar.setTextureRect(sf::IntRect(0,0, intprogress,size.y));
		notloadedbar.setTextureRect(sf::IntRect(intprogress, 0, size.x - intprogress, size.y));
		loadedbar.setPosition(pos);
		notloadedbar.setPosition(sf::Vector2f(float(intprogress), 0.0f) + pos);
		loaded++;
		if (to_load.empty())
		{
			getEngine()->setCursorVisible(true);
			loadedbar.setTexture(*ResourceManager::getTexture("Data\\LoadBarDone.png"));
		}
	}
	//Logos
	timer += time*0.5f;
	int value = int(abs(sin(timer) * 255));
	logo.setColor(sf::Color(value, value, value));
	if (timer > 3.14)
	{
		nextLogo();
		timer = 0;
	}
}

void LoadState::nextLogo()
{
	currentnr++;
	if (currentnr > nroflogos)
	{
		currentnr = 1;
	}
	logo.setTexture(*ResourceManager::getTexture("data\\Logo" + std::to_string(currentnr) + ".png"));
	logo.setOrigin(logo.getGlobalBounds().width / 2, logo.getGlobalBounds().height / 2);
	logo.setPosition(gamesize.x / 2.0f, gamesize.y / 2.0f);
}

