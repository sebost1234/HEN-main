#pragma once

#include "Defines.h"
#include "Gui.h"
#include <string>

namespace GuiNS
{
	class Popup : public sf::Drawable
	{
	public:
		Popup(std::string id):id(id)
		{
			drawEverything = true;
			stopEverything = true;

			to_delete = false;
		}
		virtual ~Popup()
		{

		}

		void changeId(std::string _id)
		{
			id = _id;
		}

		std::string getId()
		{
			return id;
		}

		bool isDrawing()
		{
			return drawEverything;
		}

		bool isStopping()
		{
			return stopEverything;
		}

		void setStopping(bool set)
		{
			stopEverything = set;
		}

		void setDrawing(bool set)
		{
			drawEverything = set;
		}

		virtual bool sync(sf::Vector2f mousepos, float time)//1 - delete 0 - don't
		{
			localgui.sync(mousepos, time);
			return to_delete;
		}

		virtual bool processEvent(sf::Event event, sf::Vector2f mousepos)
		{
			return localgui.processEvent(event, mousepos);
		}

		void reset()
		{
			to_delete = false;
		}

		Gui*getLocalGui()
		{
			return &localgui;
		}

	private:
		bool drawEverything;
		bool stopEverything;

	protected:
		Gui localgui;
		bool to_delete;

	private:
		std::string id;
		// Inherited via Drawable
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(localgui, states);
		}
	};


	class InfoPopup : public Popup, public GuiElementObserver
	{
	public:
		InfoPopup(std::string id,Style*style, sf::String text, sf::String okbuttontext = "OK", sf::Vector2f size = sf::Vector2f(400, 300)):
			Popup(id),
			rectangle(style, size),
			info(style, *ResourceManager::getFont(), sf::Vector2f(size.x, size.y-50), text, 30, 1, 5, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Type::NewLine),
			button(style, *ResourceManager::getFont(), sf::Vector2f(100, 50), okbuttontext, 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing)
		{
			rectangle.setPosition(gamesize*0.5f - rectangle.getSize()*0.5f);
			rectangle.setClickable(false);
			info.setClickable(false);


			info.setPosition(rectangle.getPosition());
			button.setPosition(rectangle.getPosition() + sf::Vector2f(size.x*0.5f-button.getSize().x*0.5f,size.y - 50));



			localgui.addElement(&rectangle);
			localgui.addElement(&info);
			localgui.addElement(&button);

			button.setObserver(this);
		}


		virtual void notifyEvent(GuiElementEvent event, GuiElement*from)
		{
			if (event.type == GuiNS::GuiElementEvent::Mouse && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
			{
				if (from == &button)
				{
					to_delete = true;
				}

			}
		}

	private:
		GuiRectangle rectangle;
		GuiText info;
		GuiText button;
	};

	class ChoicePopup : public Popup, public GuiElementObserver
	{
	public:
		ChoicePopup(std::string id, Style*style, sf::String text, sf::String button1text = "Yes", sf::String button2text = "No", sf::Vector2f size = sf::Vector2f(800, 400)) :
			Popup(id),
			rectangle(GuiNS::GuiRectangle(ResourceManager::getStyle(StyleTypes::blankwhite), size),
				"Popup.png", "Popup.png", "Popup.png", "Data\\"),
			info(ResourceManager::getStyle(StyleTypes::transparentbackgrounddarktext), *ResourceManager::getFont(), sf::Vector2f(size.x, size.y - 50), text, 40, 50, 50, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Type::NewLine),
			button1(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), button1text, 45, 5, 5, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
				"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5),
			button2(GuiNS::GuiText(ResourceManager::getStyle(StyleTypes::blankwhite), *ResourceManager::getFont(), sf::Vector2f(200, 70), button2text, 45, 5, 5, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
				"ButtonNormal.png", "ButtonHover.png", "ButtonHover.png", "Data\\", 5)
		{
			rectangle.setPosition(gamesize*0.5f - rectangle.getSize()*0.5f);
			rectangle.setClickable(false);
			info.setClickable(false);


			info.setPosition(rectangle.getPosition());
			button1.setPosition(rectangle.getPosition() + sf::Vector2f(rectangle.getSize().x*0.25f - button1.getSize().x*0.5f, size.y - 90));
			button2.setPosition(rectangle.getPosition() + sf::Vector2f(rectangle.getSize().x*0.75f - button2.getSize().x*0.5f, size.y - 90));



			localgui.addElement(&rectangle);
			localgui.addElement(&info);
			localgui.addElement(&button1);
			localgui.addElement(&button2);

			button1.setObserver(this);
			button2.setObserver(this);

			clickedButton = Clicked::None;
		}

		void changeText(sf::String string)
		{
			info.setString(string);
		}


		virtual void notifyEvent(GuiElementEvent event, GuiElement*from)
		{
			if (event.type == GuiNS::GuiElementEvent::Mouse && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
			{
				if (from == &button1)
				{
					clickedButton = Clicked::Button1;
					to_delete = true;
				}
				else if (from == &button2)
				{
					clickedButton = Clicked::Button2;
					to_delete = true;
				}
			}
		}


		enum Clicked
		{
			None,
			Button1,
			Button2
		};
	private:
		GuiRectangleSprite rectangle;
		GuiText info;
		GuiTextSprite button1;
		GuiTextSprite button2;

		Clicked clickedButton;
	public:
		Clicked getClicked()
		{
			Clicked tmp = clickedButton;
			clickedButton = None;
			return tmp;
		}
	};
}