#include "Options.h"
#include "MainMenuState.h"
#include "Engine.h"

Options::Options(Optiontype type, State*fatherstate) :
	background(ResourceManager::getStyle(StyleTypes::blankwhite), gamesize),
	optionsbutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), "Options", 50, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5),
	gallerybutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), "Gallery", 50, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5),
	journalbutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), "Journal", 50, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5),
	saveandloadbutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), "Load", 50, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5),
	mainmenubutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(250, 70), "Main Menu", 45, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"BigButtonNormal.png", "BigButtonHover.png", "BigButtonHover.png", "Data\\", 5),
	returnbutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(250, 70), "Back", 45, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"BigButtonNormal.png", "BigButtonHover.png", "BigButtonHover.png", "Data\\", 5),
	musicandgallerybutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(250, 70), "Music", 45, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"BigButtonNormal.png", "BigButtonHover.png", "BigButtonHover.png", "Data\\", 5),
	choicepopup("none", ResourceManager::getStyle(), ""),
	type(type),
	fatherstate(fatherstate),
	subtype(nullptr),
	Popup("options")
{
	background.setClickable(false);

	float padding = 10;

	optionsbutton.setPosition(sf::Vector2f(gamesize.x - optionsbutton.getSize().x - padding, 10)); 
	optionsbutton.setObserver(this);

	gallerybutton.setPosition(optionsbutton.getPosition() + sf::Vector2f(-gallerybutton.getSize().x-padding, 0));
	gallerybutton.setObserver(this);

	gallerybutton.setPosition(optionsbutton.getPosition() + sf::Vector2f(-gallerybutton.getSize().x - padding, 0));
	gallerybutton.setObserver(this);


	returnbutton.setPosition(sf::Vector2f(gamesize.x - returnbutton.getSize().x - padding*2, gamesize.y  - returnbutton.getSize().y - padding*4));
	returnbutton.setObserver(this);

	mainmenubutton.setPosition(returnbutton.getPosition() + sf::Vector2f(-returnbutton.getSize().x - padding, 0));
	mainmenubutton.setObserver(this);

	journalbutton.setPosition(gallerybutton.getPosition() + sf::Vector2f(-journalbutton.getSize().x - padding, 0));
	journalbutton.setObserver(this);


	localgui.addElement(&background);

	localgui.addElement(&journalbutton);
	localgui.addElement(&optionsbutton);
	localgui.addElement(&gallerybutton);

	musicandgallerybutton.setPosition(sf::Vector2f(25, gamesize.y - musicandgallerybutton.getSize().y - 10 * 4));
	musicandgallerybutton.setObserver(this);

	switch (type)
	{
	case Menu:

		saveandloadbutton.setPosition(journalbutton.getPosition() + sf::Vector2f(-saveandloadbutton.getSize().x - padding, 0));
		saveandloadbutton.setObserver(this);

		changeSubType(OptionsSubTypeEnum::Load_ST);
		break;
	case InGame:

		saveandloadbutton.setPosition(journalbutton.getPosition() + sf::Vector2f(-saveandloadbutton.getSize().x - padding, 0));
		saveandloadbutton.setObserver(this);

		localgui.addElement(&mainmenubutton);
		localgui.addElement(&saveandloadbutton);
		changeSubType(OptionsSubTypeEnum::Save_ST);
		break;
	default:

		changeSubType(OptionsSubTypeEnum::Options_ST);
		break;
	}

	localgui.addElement(&returnbutton);

	localgui.setObserver(this);
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
		{
			changeSubType(OptionsSubTypeEnum::Options_ST);
		}
		else if (from == &saveandloadbutton)
		{
			if (type == Optiontype::InGame)
			{
				if (subtype->getSubtype() == OptionsSubTypeEnum::Save_ST)
				{
					saveandloadbutton.setString(L"Save");
					changeSubType(new LoadSettings(this));
				}
				else
				{
					saveandloadbutton.setString(L"Load");
					changeSubType(new SaveSettings(this));
				}
			}
			else if (subtype->getSubtype() != OptionsSubTypeEnum::Load_ST)
			{
				changeSubType(OptionsSubTypeEnum::Load_ST);
			}
		}
		else if (from == &gallerybutton)
		{
			changeSubType(OptionsSubTypeEnum::Gallery_ST);
		}
		else if (from == &journalbutton)
		{
			changeSubType(OptionsSubTypeEnum::Journal_ST);
		}
		else if (from == &musicandgallerybutton)
		{
			if(subtype->getSubtype() == OptionsSubTypeEnum::Music_ST)
				changeSubType(OptionsSubTypeEnum::Gallery_ST);
			else
				changeSubType(OptionsSubTypeEnum::Music_ST);
		}
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
			else if (tmp->getId() == "copy")
			{
				if (choicepopup.getClicked() == GuiNS::ChoicePopup::Clicked::Button1)
				{
					reinterpret_cast<OptionsSaveSubType*>(subtype)->copy();
				}
			}
			else if (tmp->getId() == "defaults")
			{
				if (choicepopup.getClicked() == GuiNS::ChoicePopup::Clicked::Button1)
				{
					fatherstate->getEngine()->getSettings()->restoreDefaults();
					reinterpret_cast<GeneralSettings*>(subtype)->sync();
				}
			}
		}

	}
}
