#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

#include "Defines.h"
#include "Warnings.h"
#include "SoundEngine.h"
#include "Gui.h"
#include "ResourceManager.h"
#include "State.h"

#include "StartState.h"
#include "MainMenuState.h"
#include "GameState.h"

struct Settings
{
	Settings()
	{
		std::fstream file;
		file.open("Data\\Save\\settings", std::ios::in);
		file >> volume;
		sf::Listener::setGlobalVolume(float(volume));
	}
	void changeVolume(int newvolume)
	{
		sf::Listener::setGlobalVolume(float(newvolume));
		volume = newvolume;
		saveToFile();
	}
	void saveToFile()
	{
		std::fstream file;
		file.open("Data\\Save\\settings", std::ios::out | std::ios::trunc);
		file << volume;
	}
private:
	int volume;
};

class Engine
{
public:
	Engine(sf::RenderWindow*window) :
		window(window)
	{
		fullscreen = false;
		state = new MainMenuState(this);
		resize();
	}
	~Engine()
	{
		if(state!=nullptr)
			delete state;
	}

	void start()
	{
		sf::Clock clock;
		while (window->isOpen())
		{
			sf::Event event;
			while (window->pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					window->close();
					break;
				case sf::Event::Resized:
					resize();
					break;
				default:
					if(state->processEvent(event))
					if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::F)
					{
						window->close();
						if(fullscreen)
							window->create(sf::VideoMode(800, 600), "Test", sf::Style::Default);
						else
							window->create(sf::VideoMode::getDesktopMode(), "Test", sf::Style::Fullscreen);
						resize();
						fullscreen = !fullscreen;
					}
					else if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::F12)
					{
						sf::Texture tmp;
						tmp.create(window->getSize().x, window->getSize().y);
						tmp.update(*window);
						sf::Image tmp2 = tmp.copyToImage();
						tmp2.saveToFile("screenshot" + std::to_string(time(nullptr)) + ".png");
					}

					break;
				}
			}
			float frametime = clock.restart().asSeconds();
			state->sync(frametime);
			State*tmp = state->newstate;
			if (tmp != nullptr)
			{
				delete state;
				state = tmp;
			}

			static float tmp2 = 0;
			tmp2 += frametime;
			if (tmp2 > 5)
			{
				std::cout << "FPS:" << 1 / frametime << std::endl;			
				std::cout << "MS:" << frametime*1000 << std::endl;
				tmp2 = 0;
			}

			window->clear();
			state->draw();
			window->display();

			SoundEngine::sync_audio();
		}
	}
	sf::RenderWindow*getWindow()
	{
		return window;
	}
	Settings*getSettings()
	{
		return &settings;
	}
private:

	void resize()
	{
		//Resizing window if its smaller than min size
		sf::Vector2u size = window->getSize();
		size.x = std::max(size.x, MINWIDTH);
		size.y = std::max(size.y, MINHEIGHT);
		window->setSize(size);
		//Changing the view
		sf::View view = getLetterboxView(sf::View(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(float(gamesize.x), float(gamesize.y)))), size.x, size.y);

		window->setView(view);
	}

	sf::View getLetterboxView(sf::View view, unsigned int windowWidth, unsigned int windowHeight) 
	{

		// Compares the aspect ratio of the window to the aspect ratio of the view,
		// and sets the view's viewport accordingly in order to archieve a letterbox effect.
		// A new view (with a new viewport set) is returned.

		float windowRatio = windowWidth / (float)windowHeight;
		float viewRatio = view.getSize().x / (float)view.getSize().y;
		float sizeX = 1;
		float sizeY = 1;
		float posX = 0;
		float posY = 0;

		bool horizontalSpacing = true;
		if (windowRatio < viewRatio)
			horizontalSpacing = false;

		// If horizontalSpacing is true, the black bars will appear on the left and right side.
		// Otherwise, the black bars will appear on the top and bottom.

		if (horizontalSpacing) {
			sizeX = viewRatio / windowRatio;
			posX = (1 - sizeX) / 2.f;
		}

		else {
			sizeY = windowRatio / viewRatio;
			posY = (1 - sizeY) / 2.f;
		}

		view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

		return view;
	}

	sf::RenderWindow*window;
	bool fullscreen;

	Settings settings;

	State*state;
};