#pragma once

#include "Popup.h"
#include "State.h"
#include "OptionsSubtype.h"

enum Optiontype
{
	Menu,
	InGame
};

class Options : public GuiNS::GuiElementObserver, public GuiNS::GuiObserver, public GuiNS::Popup
{
public:
	Options(Optiontype type, State*fatherstate);
	~Options();

	virtual void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement*from) override;


	bool sync(sf::Vector2f mousepos, float time) override //1 - delete 0 - don't
	{
		localgui.sync(mousepos, time);
		return to_delete;
	}

	bool processEvent(sf::Event event, sf::Vector2f mousepos) override
	{
		if (localgui.processEvent(event, mousepos))
		{
			if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Escape)
			{
				if (localgui.hasPopup())
					localgui.setPopup(nullptr);
				else
					to_delete = true;
				return false;
			}
			return true;
		}
		return false;
	}

	virtual void notifyEvent(GuiNS::GuiEvent event, GuiNS::Gui*from);

	void changeSubType(OptionsSubTypeEnum newtype)
	{
		if (subtype != nullptr)
			if (newtype == subtype->getSubtype())
				return;

		SoundEngine::changeMusic("options.ogg", true);
		localgui.eraseElement(&musicandgallerybutton);
		switch (newtype)
		{
		case Save_ST:
			changeSubType(new SaveSettings(this));
			break;
		case Load_ST:
			changeSubType(new LoadSettings(this));
			if (type == Optiontype::Menu)
				localgui.eraseElement(&saveandloadbutton);
			break;
		case Options_ST:
			changeSubType(new GeneralSettings(this));
			break;
		case Gallery_ST:
			changeSubType(new CGGallery(this));
			localgui.addElement(&musicandgallerybutton);
			musicandgallerybutton.setString("Music");
			break;
		case Journal_ST:
			changeSubType(new Journal(this));
			break;
		case Music_ST:
			changeSubType(new Music(this));
			localgui.addElement(&musicandgallerybutton);
			musicandgallerybutton.setString("Gallery");
			break;
		default:
			break;
		}
	}

private:

	void changeSubType(OptionsSubType*newsubtype)
	{
		localgui.addElement(&saveandloadbutton);
		if (subtype != nullptr)
		{
			subtype->disable(&localgui);
			delete subtype;
		}
		subtype = newsubtype;
		subtype->enable(&localgui);
	}

	GuiNS::GuiRectangle background;

	GuiNS::GuiTextSprite optionsbutton;
	GuiNS::GuiTextSprite gallerybutton;
	GuiNS::GuiTextSprite journalbutton;
	GuiNS::GuiTextSprite saveandloadbutton;
	GuiNS::GuiTextSprite mainmenubutton;
	GuiNS::GuiTextSprite returnbutton;

	GuiNS::GuiTextSprite musicandgallerybutton;

	OptionsSubType*subtype;

	Optiontype type;

	State*fatherstate;

	GuiNS::ChoicePopup choicepopup;

	friend class GeneralSettings;
	friend class OptionsSaveSubType;
	friend class SaveSettings;
	friend class LoadSettings;
	friend class CGGallery;
	friend class Journal;
	friend class Music;
};