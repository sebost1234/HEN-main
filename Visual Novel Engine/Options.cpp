#include "Options.h"
#include "MainMenuState.h"

Options::Options(Optiontype type, State*fatherstate) :
	background(ResourceManager::getStyle(StyleTypes::blankwhite), gamesize),
	optionsbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Options", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	savebutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Save Game", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	loadbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Load Game", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	mainmenubutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Main Menu", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	returnbutton(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Return", 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
	choicepopup("none", ResourceManager::getStyle(), ""),
	type(type),
	fatherstate(fatherstate),
	subtype(nullptr),
	Popup("options")
{
	background.setClickable(false);
	background.changeRectangle()->setTexture(ResourceManager::getTexture("Data//OptionsTexture.png"));

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

	localgui.setObserver(this);

	changeSubType(new GeneralSettings(this));

}

Options::~Options()
{
	if (subtype != nullptr)
		delete subtype;
}

void Options::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::GuiElementEvent::Pressed && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
	{
		if (from == &optionsbutton)
			changeSubType(new GeneralSettings(this));
		if (from == &savebutton)
			changeSubType(new SaveSettings(this));
		else if (from == &loadbutton)
			changeSubType(new LoadSettings(this));
		else if (from == &mainmenubutton)
		{
			choicepopup.changeId("gotomainmenu");
			choicepopup.changeText("Do you really want to go back to mainmenu? (All unsaved progress will be lost)");
			localgui.setPopup(&choicepopup);
		}
		else if (from == &returnbutton)
			to_delete = true;
		else return;
		SoundEngine::playSound("click");
	}
}

void Options::notifyEvent(GuiNS::GuiEvent event, GuiNS::Gui * from)
{
	if (event.type == GuiNS::GuiEvent::PopupDeleted)
	{
		Popup*tmp = from->getCurrentPopup();

		if (tmp != nullptr)
		{
			if (tmp->getId() == "gotomainmenu")
			{
				if (choicepopup.getClicked() == GuiNS::ChoicePopup::Clicked::Button1)
					fatherstate->setNewState(new MainMenuState(fatherstate->getEngine()));
			}
			else if (tmp->getId() == "save")
			{
				if (choicepopup.getClicked() == GuiNS::ChoicePopup::Clicked::Button1)
				{
					reinterpret_cast<OptionsSaveSubType*>(subtype)->save();
				}
			}
			else if (tmp->getId() == "load")
			{
				if (choicepopup.getClicked() == GuiNS::ChoicePopup::Clicked::Button1)
				{
					reinterpret_cast<OptionsSaveSubType*>(subtype)->load();
				}
			}
			else if (tmp->getId() == "delete")
			{
				if (choicepopup.getClicked() == GuiNS::ChoicePopup::Clicked::Button1)
				{
					reinterpret_cast<OptionsSaveSubType*>(subtype)->delet();
				}
			}
		}

	}
}
