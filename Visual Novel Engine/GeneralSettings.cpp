#include "GeneralSettings.h"
#include "GameState.h"
#include "Options.h"
#include "Engine.h"


GeneralSettings::GeneralSettings(Options * options) : OptionsSubType(options),
	pagename(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(100, 70), "Options", 60, 5, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	windowmodeinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Window Mode:", 50, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	checkboxfullscreen(GuiNS::GuiRectangle(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(20, 20))),
	checkboxfullscreeninfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 20), "Fullscreen", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	checkboxwindowed(GuiNS::GuiRectangle(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(20, 20))),
	checkboxwindowedinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 20), "Windowed", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	volumeinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Volume:", 50, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	mastervolumeinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Master Volume:", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	mastervolumebar(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(400, 40), sf::Vector2f(20, 40), "OptionsBarNormal.png", "OptionsBarHeld.png", "OptionsBarHeld.png", "OptionsBarBackground.png", "Data\\"),
	bgmvolumeinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "BGM Volume:", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	bgmvolumebar(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(400, 40), sf::Vector2f(20, 40), "OptionsBarNormal.png", "OptionsBarHeld.png", "OptionsBarHeld.png", "OptionsBarBackground.png", "Data\\"),
	sfxvolumeinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "SFX Volume:", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	sfxvolumebar(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(400, 40), sf::Vector2f(20, 40), "OptionsBarNormal.png", "OptionsBarHeld.png", "OptionsBarHeld.png", "OptionsBarBackground.png", "Data\\"),
	systemvolumeinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "System Volume:", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	systemvolumebar(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(400, 40), sf::Vector2f(20, 40), "OptionsBarNormal.png", "OptionsBarHeld.png", "OptionsBarHeld.png", "OptionsBarBackground.png", "Data\\"),
	gameconfiginfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Game Config:", 50, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	textspeedinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Text Speed:", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	textspeedbar(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(400, 40), sf::Vector2f(20, 40), "OptionsBarNormal.png", "OptionsBarHeld.png", "OptionsBarHeld.png", "OptionsBarBackground.png", "Data\\"),
	checkboxskip(GuiNS::GuiRectangle(ResourceManager::getStyle(StyleTypes::blankwhite), sf::Vector2f(40, 40))),
	checkboxskipinfo(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(200, 50), "Skip previously unseen text.", 30, 1, 0, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Left, GuiNS::GuiText::Nothing),
	defaultsbutton(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), "Default", 40, 1, 10, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
		"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5)
{
	options->background.setTexture(ResourceManager::getBigTexture("Data//bgOptions.png"));

	pagename.setPosition(sf::Vector2f(10, 100));
	pagename.setClickable(false);


	windowmodeinfo.setPosition(sf::Vector2f(300, 170));
	windowmodeinfo.setClickable(false);

	checkboxfullscreen.setPosition(windowmodeinfo.getPosition() + sf::Vector2f(50, 20 + windowmodeinfo.getSize().y));
	checkboxfullscreen.setObserver(this);
	checkboxfullscreeninfo.setPosition(checkboxfullscreen.getPosition() + sf::Vector2f(checkboxfullscreen.getSize().x+ 10, 0));
	checkboxfullscreeninfo.setObserver(this);
	checkboxwindowed.setPosition(checkboxfullscreen.getPosition() + sf::Vector2f(0, 20 + checkboxfullscreen.getSize().y));
	checkboxwindowed.setObserver(this);
	checkboxwindowedinfo.setPosition(checkboxwindowed.getPosition() + sf::Vector2f(checkboxwindowed.getSize().x + 10, 0));
	checkboxwindowedinfo.setObserver(this);

	syncFullscreen();

	volumeinfo.setPosition(sf::Vector2f(300, 400));
	volumeinfo.setClickable(false);
	////////////////////////////////////////////////////////////////////
	mastervolumeinfo.setPosition(volumeinfo.getPosition() + sf::Vector2f(50, 20 + volumeinfo.getSize().y));
	mastervolumeinfo.setClickable(false);

	mastervolumebar._changeMax(20);
	mastervolumebar._changeState(int(sf::Listener::getGlobalVolume()/5));
	mastervolumebar.setPosition(mastervolumeinfo.getPosition() + sf::Vector2f(0, 0 + mastervolumeinfo.getSize().y));
	mastervolumebar.setObserver(this);
	////////////////////////////////////////////////////////////////////
	bgmvolumeinfo.setPosition(mastervolumebar.getPosition() + sf::Vector2f(0, 20 + mastervolumebar.getSize().y));
	bgmvolumeinfo.setClickable(false);

	bgmvolumebar._changeMax(20);
	bgmvolumebar._changeState(int(options->fatherstate->getEngine()->getSettings()->getBgVolume()/5));
	bgmvolumebar.setPosition(bgmvolumeinfo.getPosition() + sf::Vector2f(0, 0 + bgmvolumeinfo.getSize().y));
	bgmvolumebar.setObserver(this);
	////////////////////////////////////////////////////////////////////
	sfxvolumeinfo.setPosition(bgmvolumebar.getPosition() + sf::Vector2f(0, 20 + bgmvolumebar.getSize().y));
	sfxvolumeinfo.setClickable(false);

	sfxvolumebar._changeMax(20);
	sfxvolumebar._changeState(int(options->fatherstate->getEngine()->getSettings()->getSfxVolume()/5));
	sfxvolumebar.setPosition(sfxvolumeinfo.getPosition() + sf::Vector2f(0, 0 + sfxvolumeinfo.getSize().y));
	sfxvolumebar.setObserver(this);
	////////////////////////////////////////////////////////////////////
	systemvolumeinfo.setPosition(sfxvolumebar.getPosition() + sf::Vector2f(0, 20 + sfxvolumebar.getSize().y));
	systemvolumeinfo.setClickable(false);

	systemvolumebar._changeMax(20);
	systemvolumebar._changeState(int(options->fatherstate->getEngine()->getSettings()->getSystemVolume()/5));
	systemvolumebar.setPosition(systemvolumeinfo.getPosition() + sf::Vector2f(0, 0 + systemvolumeinfo.getSize().y));
	systemvolumebar.setObserver(this);
	////////////////////////////////////////////////////////////////////

	gameconfiginfo.setPosition(sf::Vector2f(1110, 400));
	gameconfiginfo.setClickable(false);

	textspeedinfo.setPosition(gameconfiginfo.getPosition() + sf::Vector2f(50, 20 + gameconfiginfo.getSize().y));
	textspeedinfo.setClickable(false);

	textspeedbar._changeMax(20);
	textspeedbar._changeState(int(options->fatherstate->getEngine()->getSettings()->getTextspeed() / 5));
	textspeedbar.setPosition(textspeedinfo.getPosition() + sf::Vector2f(0, 0 + textspeedinfo.getSize().y));
	textspeedbar.setObserver(this);

	checkboxskip.setPosition(sf::Vector2f(textspeedbar.getPosition().x, bgmvolumebar.getPosition().y));
	checkboxskip.setObserver(this);
	checkboxskipinfo.setPosition(checkboxskip.getPosition() + sf::Vector2f(checkboxskip.getSize().x + 10, 0));
	checkboxskipinfo.setObserver(this);

	syncSkipping();


	defaultsbutton.setPosition(sf::Vector2f(textspeedbar.getPosition().x, systemvolumebar.getPosition().y + systemvolumebar.getSize().y - defaultsbutton.getSize().y));
	defaultsbutton.setObserver(this);
}

GeneralSettings::~GeneralSettings()
{
	options->fatherstate->getEngine()->getSettings()->saveToFile();
}


void GeneralSettings::syncFullscreen()
{
	if (options->fatherstate->getEngine()->getSettings()->isFullscreen())
	{
		checkboxfullscreen.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\OptionsCheckCircleChecked.png"));
		checkboxwindowed.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\OptionsCheckCircleNormal.png"));
		checkboxfullscreen.setClickable(false);
		checkboxfullscreeninfo.setClickable(false);
		checkboxwindowed.setClickable(true);
		checkboxwindowedinfo.setClickable(true);
	}
	else
	{
		checkboxfullscreen.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\OptionsCheckCircleNormal.png"));
		checkboxwindowed.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\OptionsCheckCircleChecked.png"));
		checkboxfullscreen.setClickable(true);
		checkboxfullscreeninfo.setClickable(true);
		checkboxwindowed.setClickable(false);
		checkboxwindowedinfo.setClickable(false);
	}
}

void GeneralSettings::syncSkipping()
{
	if (options->fatherstate->getEngine()->getSettings()->isSkipping())
		checkboxskip.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\OptionsCheckSquareChecked.png"));
	else
		checkboxskip.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\OptionsCheckSquareNormal.png"));
}

void GeneralSettings::syncBars()
{
	mastervolumebar._changeState(int(sf::Listener::getGlobalVolume() / 5));
	bgmvolumebar._changeState(int(options->fatherstate->getEngine()->getSettings()->getBgVolume() / 5));
	sfxvolumebar._changeState(int(options->fatherstate->getEngine()->getSettings()->getSfxVolume() / 5));
	systemvolumebar._changeState(int(options->fatherstate->getEngine()->getSettings()->getSfxVolume() / 5));
	textspeedbar._changeState(int(options->fatherstate->getEngine()->getSettings()->getTextspeed() / 5));
}

void GeneralSettings::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::GuiElementEvent::BarValueChanged)
	{
		if (from == &mastervolumebar)
		{
			options->fatherstate->getEngine()->getSettings()->setMasterVolume(event.bar.state * 5);
		}
		else if (from == &bgmvolumebar)
		{
			options->fatherstate->getEngine()->getSettings()->setBgVolume(event.bar.state * 5);
		}
		else if (from == &sfxvolumebar)
		{
			options->fatherstate->getEngine()->getSettings()->setSfxVolume(event.bar.state * 5);
		}
		else if (from == &systemvolumebar)
		{
			options->fatherstate->getEngine()->getSettings()->setSystemVolume(event.bar.state * 5);
		}
		else if (from == &textspeedbar)
		{
			options->fatherstate->getEngine()->getSettings()->setTextspeed(event.bar.state * 5);
		}
		SoundEngine::playSound("hover");
	}
	else if (event.type == GuiNS::GuiElementEvent::Mouse && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
	{
		if (from == &checkboxfullscreen|| from == &checkboxfullscreeninfo)
		{
			if (!options->fatherstate->getEngine()->getSettings()->isFullscreen())
			{
				options->fatherstate->getEngine()->getSettings()->setFullscreen(true);
				options->fatherstate->getEngine()->syncWindow();
				syncFullscreen();
			}
			return;
		}
		else if (from == &checkboxwindowed || from == &checkboxwindowedinfo)
		{
			if (options->fatherstate->getEngine()->getSettings()->isFullscreen())
			{
				options->fatherstate->getEngine()->getSettings()->setFullscreen(false);
				options->fatherstate->getEngine()->syncWindow();
				syncFullscreen();
			}
			return;
		}
		else if (from == &checkboxskip || from == &checkboxskipinfo)
		{
			options->fatherstate->getEngine()->getSettings()->setSkipping(!options->fatherstate->getEngine()->getSettings()->isSkipping());
			syncSkipping();
		}
		else if (from == &defaultsbutton)
		{
			options->choicepopup.changeText("Are you sure you want to restore default settings?");
			options->localgui.setPopup(&options->choicepopup);
			options->choicepopup.changeId("defaults");
		}
		else return;
		SoundEngine::playSound("click");
	}
}