#include "MainMenuState.h"
#include "Engine.h"
#include "Options.h"

MainMenuState::MainMenuState(Engine*engine) :
	startbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), L"Start", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	loadbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Load Game", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	optionsbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Options", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	exitbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Exit", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	options(engine->getWindow(), Optiontype::Menu, this),
	State(engine)
{

	background.setSize(gamesize);
	background.setFillColor(ResourceManager::getStyle()->backcolor);


	startbutton.setPosition(gamesize*0.5f - startbutton.getSize()*0.5f);
	startbutton.setObserver(this);

	loadbutton.setPosition(startbutton.getPosition() + sf::Vector2f(0, 10 + startbutton.getSize().y));
	loadbutton.setObserver(this);

	optionsbutton.setPosition(loadbutton.getPosition() + sf::Vector2f(0, 10 + loadbutton.getSize().y));
	optionsbutton.setObserver(this);

	exitbutton.setPosition(optionsbutton.getPosition() + sf::Vector2f(0, 10 + optionsbutton.getSize().y));
	exitbutton.setObserver(this);

	gui.addElement(&startbutton);
	gui.addElement(&loadbutton);
	gui.addElement(&optionsbutton);
	gui.addElement(&exitbutton);
}

bool MainMenuState::processEvent(sf::Event event)
{
	if (options.isVisible())
		return options.processEvent(event);
	else
	if (gui.processEvent(event, getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*getWindow()))))
	{
		if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Escape)
		{
			options.toggle();
			return false;
		}
		return true;
	}
	return false;
}

void MainMenuState::sync(float time)
{
	if (options.isVisible())
		options.sync(time);
	else
		gui.sync(getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*getWindow())), time);
}

void MainMenuState::draw()
{
	if (options.isVisible())
		getWindow()->draw(options);
	else
	{
		getWindow()->draw(background);
		getWindow()->draw(gui);
	}
}

void MainMenuState::notifyEvent(Gui::MyEvent event, Gui::GuiElement*from)
{
	if (event.type == Gui::MyEvent::Pressed && event.mouse.type == Gui::MyEvent::Type::Released)
	{
		if (from == &startbutton)
			setNewState(new GameState(getEngine(), nullptr));
		else if (from == &loadbutton)
		{
			options.setVisible(true);
			options.changeSubType(new LoadSettings(this));
		}
		else if (from == &optionsbutton)
			options.setVisible(true);
		else if (from == &exitbutton)
			getWindow()->close();
		else return;
		SoundEngine::playSound("click");
	}
}
