#include "Options.h"
#include "MainMenuState.h"

Options::Options(sf::RenderWindow*window, Optiontype type, State*fatherstate) :
	background(ResourceManager::getStyle(), gamesize),
	optionsbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Options", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	savebutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Save Game", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	loadbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Load Game", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	mainmenubutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Main Menu", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	returnbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Return", 30, 1, 30, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Center, Gui::GuiText::Nothing),
	window(window),
	type(type),
	fatherstate(fatherstate),
	subtype(nullptr)
{
	background.setClickable(false);

	optionsbutton.setPosition(sf::Vector2f(50, 50));
	optionsbutton.setObserver(this);

	loadbutton.setPosition(optionsbutton.getPosition() + sf::Vector2f(0, 10 + optionsbutton.getSize().y));
	loadbutton.setObserver(this);

	savebutton.setPosition(loadbutton.getPosition() + sf::Vector2f(0, 10 + loadbutton.getSize().y));
	savebutton.setObserver(this);

	mainmenubutton.setPosition(savebutton.getPosition() + sf::Vector2f(0, 10 + savebutton.getSize().y));
	mainmenubutton.setObserver(this);

	returnbutton.setPosition(mainmenubutton.getPosition() + sf::Vector2f(0, (10 + mainmenubutton.getSize().y)*2));
	returnbutton.setObserver(this);


	gui.addElement(&background);

	gui.addElement(&optionsbutton);
	gui.addElement(&loadbutton);

	switch (type)
	{
	case Menu:
		savebutton.setClickable(false);
		break;
	case InGame:
		gui.addElement(&savebutton);
		gui.addElement(&mainmenubutton);
		break;
	default:
		break;
	}

	gui.addElement(&returnbutton);

	changeSubType(new GeneralSettings(fatherstate));

	visible = false;
}

Options::~Options()
{
	if (subtype != nullptr)
		delete subtype;
}

bool Options::processEvent(sf::Event event)
{
	if (gui.processEvent(event, window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
	{
		if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Escape)
		{
			setVisible(false);
			return false;
		}
		return true;
	}
	return false;
}

void Options::sync(float time)
{
	gui.sync(window->mapPixelToCoords(sf::Mouse::getPosition(*window)), time);
}

void Options::notifyEvent(Gui::MyEvent event, Gui::GuiElement * from)
{
	if (event.type == Gui::MyEvent::Pressed && event.mouse.type == Gui::MyEvent::Type::Released)
	{
		if (from == &optionsbutton)
			changeSubType(new GeneralSettings(fatherstate));
		if (from == &savebutton)
			changeSubType(new SaveSettings(fatherstate));
		else if (from == &loadbutton)
			changeSubType(new LoadSettings(fatherstate));
		else if (from == &mainmenubutton)
			fatherstate->setNewState(new MainMenuState(fatherstate->getEngine()));
		else if (from == &returnbutton)
			setVisible(false); 
		else return;
		SoundEngine::playSound("click");
	}
}

void Options::setVisible(bool state)
{
	visible = state;
	if(!visible)
		changeSubType(new GeneralSettings(fatherstate));
}

void Options::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(gui, states);
	if (subtype != nullptr)
		target.draw(*subtype, states);
}
