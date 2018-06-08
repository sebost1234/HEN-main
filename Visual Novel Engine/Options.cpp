#include "Options.h"
#include "MainMenuState.h"

Options::Options(Optiontype type, State*fatherstate) :
	background(ResourceManager::getStyle(), gamesize),
	optionsbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Options", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	savebutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Save Game", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	loadbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Load Game", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	mainmenubutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Main Menu", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	returnbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Return", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
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


	localgui.addElement(&background);

	localgui.addElement(&optionsbutton);
	localgui.addElement(&loadbutton);

	switch (type)
	{
	case Menu:
		savebutton.setClickable(false);
		break;
	case InGame:
		localgui.addElement(&savebutton);
		localgui.addElement(&mainmenubutton);
		break;
	default:
		break;
	}

	localgui.addElement(&returnbutton);

	changeSubType(new GeneralSettings(fatherstate));

}

Options::~Options()
{
	if (subtype != nullptr)
		delete subtype;
}

void Options::notifyEvent(GuiNS::MyEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::MyEvent::Pressed && event.mouse.type == GuiNS::MyEvent::Type::Released)
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
			to_delete = true;
		else return;
		SoundEngine::playSound("click");
	}
}