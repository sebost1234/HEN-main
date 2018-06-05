#pragma once


#include "Gui.h"

namespace GuiNS
{
	class Popup : public sf::Drawable
	{
	public:
		Popup()
		{
			drawEverything = true;
			stopEverything = true;
			rectangle.setSize(sf::Vector2f(100, 100));
		}
		virtual ~Popup()
		{

		}


		bool Popup::isDrawingEverything()
		{
			return drawEverything;
		}

		bool Popup::isStoppingEverything()
		{
			return stopEverything;
		}

		bool Popup::sync(sf::Vector2f mousepos, float time)//1 - delete 0 - don't
		{
			localgui.sync(mousepos, time);
			return false;
		}

		bool Popup::processEvent(sf::Event event, sf::Vector2f mousepos)
		{
			return localgui.processEvent(event, mousepos);
		}

	private:
		bool drawEverything;
		bool stopEverything;


		Gui localgui;

		sf::RectangleShape rectangle;

		// Inherited via Drawable
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(localgui, states);
			target.draw(rectangle);
		}
	};
}