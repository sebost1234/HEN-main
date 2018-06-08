#pragma once

#include "Defines.h"
#include "Gui.h"
#include <string>

namespace GuiNS
{
	class Popup : public sf::Drawable
	{
	public:
		Popup()
		{
			drawEverything = true;
			stopEverything = true;

			to_delete = false;
		}
		virtual ~Popup()
		{

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

	private:
		bool drawEverything;
		bool stopEverything;

	protected:
		Gui localgui;
		bool to_delete;

	private:
		// Inherited via Drawable
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(localgui, states);
		}
	};


	class InfoPopup : public Popup, public GuiElementObserver
	{
	public:
		InfoPopup(Style*style, sf::String text, sf::String okbuttontext = "OK", sf::Vector2f size = sf::Vector2f(400, 300)):
			rectangle(style, size),
			info(style, *ResourceManager::getFont(), sf::Vector2f(size.x, size.y-50), text, 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Top, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing),
			button(style, *ResourceManager::getFont(), sf::Vector2f(100, 50), okbuttontext, 30, 1, 30, GuiNS::GuiText::FormatVer::Ver_Center, GuiNS::GuiText::FormatHor::Hor_Center, GuiNS::GuiText::Nothing)
		{
			rectangle.setPosition(gamesize*0.5f - rectangle.getSize()*0.5f);


			info.setPosition(rectangle.getPosition());
			button.setPosition(rectangle.getPosition() + sf::Vector2f(size.x*0.5f-button.getSize().x*0.5f,size.y - 50));



			localgui.addElement(&rectangle);
			localgui.addElement(&info);
			localgui.addElement(&button);

			button.setObserver(this);
		}


		virtual void notifyEvent(MyEvent event, GuiElement*from)
		{
			if (event.type == GuiNS::MyEvent::Pressed && event.mouse.type == GuiNS::MyEvent::Type::Released)
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
}