#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <queue>

namespace Input
{


	enum Type
	{
		Pressed,
		Released
	};

	template < class T >
	struct Event
	{
		Event(T button, Type typ) :
			button(button),
			typ(typ)
		{

		}

		T button;
		Type typ;
	};

	typedef Event<sf::Mouse::Button> MouseEvent;

	typedef Event<sf::Keyboard::Key> KeyboardEvent;



	class Keyboard
	{
	public:
		//BUTTONS
		void clickKey(sf::Keyboard::Key key)
		{
			events.push(KeyboardEvent(key, Input::Pressed));
			held.push_back(key);
		}
		void releaseKey(sf::Keyboard::Key key)
		{
			events.push(KeyboardEvent(key, Input::Released));
			auto tmp = std::find(held.begin(), held.end(), key);
			if (!held.empty() && tmp != held.end())
				held.erase(tmp);
		}

		bool hasEvents()
		{
			return !events.empty();
		}
		KeyboardEvent popLastEvent()
		{
			KeyboardEvent tmp = events.front();
			events.pop();
			return tmp;
		}

		bool isHeld(sf::Keyboard::Key key)
		{
			auto tmp = std::find(held.begin(), held.end(), key);
			if (!held.empty() && tmp != held.end())
				return true;
			return false;
		}

		void reset()
		{
			held.clear();
			while (!events.empty())
				events.pop();
		}


	private:
		std::vector <sf::Keyboard::Key> held;
		std::queue <KeyboardEvent> events;
	};


	class MouseMovement
	{
	public:
		void bindWindow(sf::RenderWindow*bindwindow)
		{
			window = bindwindow;
		}

		void update()
		{
			mouse = sf::Mouse::getPosition(*window);
			move = mouse - mouseprev;
			mouseprev = mouse;
		}
		sf::Vector2i getMove() const
		{
			return move;
		}
		sf::Vector2i getMouse() const
		{
			return mouse;
		}
		sf::Vector2f getMove(sf::View view) const
		{
			return window->mapPixelToCoords(mouse, view) - window->mapPixelToCoords(mouse + move, view);
		}
		sf::Vector2f getMouse(sf::View view) const
		{
			return window->mapPixelToCoords(mouse, view);
		}
	private:


		sf::Vector2i mouseprev;
		sf::Vector2i mouse;
		sf::Vector2i move;
		sf::RenderWindow*window;
	};

	class Mouse
	{
	public:
		Mouse(sf::RenderWindow*bindwindow)
		{
			movement.bindWindow(bindwindow);
		}
		//BUTTONS
		void clickKey(sf::Mouse::Button key)
		{
			events.push(MouseEvent(key, Input::Pressed));
			held.push_back(key);
		}
		void releaseKey(sf::Mouse::Button key)
		{
			events.push(MouseEvent(key, Input::Released));
			auto tmp = std::find(held.begin(), held.end(), key);
			if (!held.empty() && tmp != held.end())
				held.erase(tmp);
		}

		bool hasEvents()
		{
			return !events.empty();
		}
		MouseEvent popLastEvent()
		{
			MouseEvent tmp = events.front();
			events.pop();
			return tmp;
		}

		bool isHeld(sf::Mouse::Button key)
		{
			auto tmp = std::find(held.begin(), held.end(), key);
			if (!held.empty() && tmp != held.end())
				return true;
			return false;
		}

		void reset()
		{
			held.clear();
			while (!events.empty())
				events.pop();
		}

		//MOVEMENT CLASS
		MouseMovement movement;
	private:
		std::vector <sf::Mouse::Button> held;
		std::queue <MouseEvent> events;
	};
}
