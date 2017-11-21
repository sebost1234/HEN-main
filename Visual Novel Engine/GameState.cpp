﻿#include "GameState.h"
#include "Engine.h"

GameState::GameState(Engine * engine, SaveData*data) :
	tekst(Gui::GuiText(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(300, 40), "", 30, 15, 15, Gui::GuiText::FormatVer::Ver_Top, Gui::GuiText::FormatHor::Hor_Left, Gui::GuiText::NewLine), 0.1f),
	name(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(300, 150), "Name", 30, 5, 5, Gui::GuiText::FormatVer::Ver_Center, Gui::GuiText::FormatHor::Hor_Left, Gui::GuiText::Nothing),
	options(getWindow(), Optiontype::InGame, this),
	State(engine)
{
	float x = (gamesize.x - 1280)*0.5f;

	tekst.setSize(sf::Vector2f(float(gamesize.x-x*2), 200));
	tekst.setPosition(sf::Vector2f(x, float(gamesize.y) - 200));
	name.setSize(sf::Vector2f(150, 40));
	name.setPosition(sf::Vector2f(x, float(gamesize.y) - 240));

	gui.addElement(&tekst);
	gui.addElement(&name);
	tekst.setClickable(false);
	name.setClickable(false);
	hidegui = false;


	if (data == nullptr)
	{
		if (!vnc.gotoScope(L"start"))
		{
			throwWarning("\"scope;start\" not found. Starting from the beginning of the file.");
		}
	}
	else
	{
		vnc.loadSave(*data);
		delete data;
	}
	processing = true;
	while ((timer == 0) && (processing = process(vnc.next())));
	timer = 0;
}

bool GameState::processEvent(sf::Event event)
{
	if (options.isVisible())
		return options.processEvent(event);
	else
	if (gui.processEvent(event, getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*getWindow()))))
	{
		if (
			(event.type == sf::Event::MouseButtonPressed&&event.mouseButton.button == sf::Mouse::Left) ||
			(event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Space)
			)
		{
			if (!hidegui)
			{
				if (tekst.isDone())
				{
					processing = true;
					timer = 0;
				}
				else
					tekst.show();
			}
			else hidegui = false;
			return false;
		}
		else if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::H)
		{
			hidegui = !hidegui;
			return false;
		}
	

		if (event.type == sf::Event::KeyPressed&&event.key.code == sf::Keyboard::Escape)
		{
			options.toggle();
			return false;
		}
		return true;
	}
	else return false;
	
}
void GameState::sync(float time)
{

	if (options.isVisible())
		options.sync(time);
	else
	{
		gui.sync(getWindow()->mapPixelToCoords(sf::Mouse::getPosition(*getWindow())), time);
		fxengine.sync(time);
		scene.sync(time);

		timer -= time;
		if (timer < 0)
			timer = 0;
		if (choices.empty())
			if (processing)
				while ((timer == 0) && (processing = process(vnc.next())));
	}
}

void GameState::draw()
{
	if (options.isVisible())
		getWindow()->draw(options);
	else
	{
		fxengine.begin();
		fxengine.add(scene);
		fxengine.end();

		getWindow()->draw(fxengine);

		if (!hidegui)
			getWindow()->draw(gui);
	}
}

void GameState::notifyEvent(Gui::MyEvent event, Gui::GuiElement * from)
{
	if (event.type == Gui::MyEvent::Pressed && event.mouse.type == Gui::MyEvent::Type::Released)
	{
		if(!choices.empty())
		{
			for (unsigned int i = 0; i < choices.size(); i++)
			{
				if (from == choices[i])
				{
					vnc.choose(i);
					processing = true;
					for (unsigned int i = 0; i < choices.size(); i++)
					{
						gui.eraseElement(choices[i]);
						delete choices[i];
					}
					choices.clear();
					break;
				}
			}
		}
		SoundEngine::playSound("click");
	}
}

bool GameState::process(VisualNovelEvent event)
{
	if (event.getType() > VisualNovelEvent::STARTOFSCENE&&event.getType() < VisualNovelEvent::ENDOFSCENE)
		return scene.processEvent(event);


	switch (event.getType())
	{
	case VisualNovelEvent::SetScene:
	{
		std::wstring tmp = event.getArgument(SetSceneEventtype);
		scene = Scene(true);
		return true;
	}
	case VisualNovelEvent::Say:
	{
		std::wstring tmp = event.getArgument(SayEventname);
		name.setString(sf::String::fromUtf8(tmp.begin(), tmp.end()));
		name.fitBackground(true, false);

		std::wstring type = L"";

		if (event.isArgument(SayEventtype))
			type = event.getArgument(SayEventtype);

		tmp = event.getArgument(SayEventstring);
		if (type.find(L'A') != std::string::npos)
			tekst.addText(sf::String::fromUtf8(tmp.begin(), tmp.end()));
		else
			tekst.setText(sf::String::fromUtf8(tmp.begin(), tmp.end()));

		return type.find(L"NS") != std::string::npos;
	}
	case VisualNovelEvent::Choice:
	{
		float width = 500;
		sf::Vector2f pos = sf::Vector2f(gamesize.x * 0.5f - width * 0.5f, 100);
		int nrofchoices = event.getArgumentAsInt(ChoiceEventnr);
		for (int i = 0; i < nrofchoices; i++)
		{
			choices.push_back(new Gui::GuiText(ResourceManager::getStyle(), *ResourceManager::getFont(), sf::Vector2f(width, 100), event.getArgument(ChoiceEventstart+i), 30, 1, 5, Gui::GuiText::Ver_Center, Gui::GuiText::Hor_Center));

			choices.back()->setPosition(pos);
			gui.addElement(choices.back());
			choices.back()->setObserver(this);

			pos = choices.back()->getPosition();
			pos.y += choices.back()->getSize().y + 10;
		}

		return false;
	}
	case VisualNovelEvent::PlaySound:
		SoundEngine::playSound(event.getArgumentS(SoundEventpath));
		return true;
	case VisualNovelEvent::PlayMusic:
		SoundEngine::changeMusic(event.getArgumentS(MusicEventpath));
		return true;
	case VisualNovelEvent::None:
		return true;
	case VisualNovelEvent::Stop:
		return false;
	case VisualNovelEvent::Wait:
		timer = event.getArgumentAsFloat(WaitEventtime);
		return true;
	case VisualNovelEvent::FX:
		fxengine.interpretFX(event);
		return true;
	default:
		return true;
	}
	
	return false;
}