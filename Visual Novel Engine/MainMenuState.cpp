#include "MainMenuState.h"
#include "Engine.h"
#include "Options.h"

MainMenuState::MainMenuState(Engine*engine) :
	startbutton(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Start", 40, 1, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	loadbutton(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Load", 40, 1, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	optionsbutton(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Options", 40, 1, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	gallerybutton(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Gallery", 40, 1, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	journalbutton(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Journal", 40, 1, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	exitbutton(ResourceManager::getStyle(StyleTypes::transparentbackground), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Exit", 40, 1, 10, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	options(Optiontype::Menu, this),
	State(engine)
{

	background.setSize(gamesize);
	background.setTexture(ResourceManager::getTexture("Data//bgMenu.png"));

	startbutton.fitBackground(true, false);
	loadbutton.fitBackground(true, false);
	optionsbutton.fitBackground(true, false);
	gallerybutton.fitBackground(true, false);
	journalbutton.fitBackground(true, false);
	exitbutton.fitBackground(true, false);

	float width = startbutton.getSize().x + loadbutton.getSize().x + optionsbutton.getSize().x + gallerybutton.getSize().x + journalbutton.getSize().x + exitbutton.getSize().x;
	float padding = 20;
	width += 5*padding;


	startbutton.setPosition(sf::Vector2f(sf::Vector2f(gamesize*0.5f - sf::Vector2f(width,50)*0.5f).x, gamesize.y-startbutton.getSize().y-200));
	startbutton.setObserver(this);

	loadbutton.setPosition(startbutton.getPosition() + sf::Vector2f(padding + startbutton.getSize().x, 0));
	loadbutton.setObserver(this);

	gallerybutton.setPosition(loadbutton.getPosition() + sf::Vector2f(padding + loadbutton.getSize().x, 0));
	gallerybutton.setObserver(this);

	journalbutton.setPosition(gallerybutton.getPosition() + sf::Vector2f(padding + gallerybutton.getSize().x, 0));
	journalbutton.setObserver(this);

	optionsbutton.setPosition(journalbutton.getPosition() + sf::Vector2f(padding + journalbutton.getSize().x, 0));
	optionsbutton.setObserver(this);

	exitbutton.setPosition(optionsbutton.getPosition() + sf::Vector2f(padding + optionsbutton.getSize().x, 0));
	exitbutton.setObserver(this);


	gui.addElement(&startbutton);
	gui.addElement(&loadbutton);
	gui.addElement(&optionsbutton);
	gui.addElement(&gallerybutton);
	gui.addElement(&journalbutton);
	gui.addElement(&exitbutton);

	gui.setObserver(this);

	SoundEngine::changeMusic("menu.ogg", true);
}

bool MainMenuState::processEvent(sf::Event event)
{
	if (gui.processEvent(event, getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*getWindow()))))
	{
		if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Escape)
		{
			SoundEngine::changeMusic("options.ogg", true);
			gui.setPopup(&options);
			return false;
		}
		return true;
	}
	return false;
}

void MainMenuState::sync(float time)
{
	gui.sync(getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*getWindow())), time);
}

void MainMenuState::draw()
{
	getWindow()->draw(background);
	getWindow()->draw(gui);
}

void MainMenuState::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement*from)
{
	if (event.type == GuiNS::GuiElementEvent::Pressed && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
	{
		if (from == &startbutton)
			setNewState(new GameState(getEngine()));
		else if (from == &loadbutton)
		{
			SoundEngine::changeMusic("options.ogg", true);
			gui.setPopup(&options);
			options.changeSubType(OptionsSubTypeEnum::Load_ST);
		}
		else if (from == &optionsbutton)
		{
			SoundEngine::changeMusic("options.ogg", true);
			gui.setPopup(&options);
			options.changeSubType(OptionsSubTypeEnum::Options_ST);
		}
		else if (from == &gallerybutton)
		{
			SoundEngine::changeMusic("options.ogg", true);
			gui.setPopup(&options);
			options.changeSubType(OptionsSubTypeEnum::Gallery_ST);
		}
		else if (from == &journalbutton)
		{
			SoundEngine::changeMusic("options.ogg", true);
			gui.setPopup(&options);
			options.changeSubType(OptionsSubTypeEnum::Journal_ST);
		}
		else if (from == &exitbutton)
			getWindow()->close();
		else return;
		SoundEngine::playSound("click");
	}
}

void MainMenuState::notifyEvent(GuiNS::GuiEvent event, GuiNS::Gui * from)
{
	switch (event.type)
	{
	case GuiNS::GuiEvent::PopupDeleted:
	{
		GuiNS::Popup*tmp = from->getCurrentPopup();
		if (tmp == &options)
		{
			SoundEngine::changeMusic("menu.ogg", true);
		}
	}
	break;
	default:
		break;
	}
}
