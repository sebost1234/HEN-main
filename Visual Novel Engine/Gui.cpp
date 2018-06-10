#include "Gui.h"
#include "Popup.h"

void GuiNS::Gui::sync(sf::Vector2f mousepos, float time)
{
	if (popup == nullptr || !popup->isStopping())
		for (unsigned int i = 0; i < elements.size(); i++)
			elements[i]->sync(time);

	if (popup == nullptr)
	{
		if (held != nullptr)
			held->heldEvent(mousepos, mousepos - mouseprevpos, time);
		mouseprevpos = mousepos;
		for (int i = elements.size() - 1; i >= 0; i--)
		{
			if (elements[i]->contains(mousepos))
			{

				if (hover != elements[i])
				{
					elements[i]->changeState(GuiElement::States::hover);//hover
					if (hover != nullptr)
						hover->changeState(GuiElement::States::nothing);//unhover
				}
				hover = elements[i];
				return;
			} 
		}
		if (hover != nullptr)
			hover->changeState(GuiElement::States::nothing);//unhover
		hover = nullptr;
	}
	else if (popup->sync(mousepos, time))
	{
		if (observer != nullptr)
		{
			GuiEvent event(GuiEvent::PopupDeleted);
			observer->notifyEvent(event, this);
		}
		popup->reset();
		popup = nullptr;
	}
}

bool GuiNS::Gui::processEvent(sf::Event event, sf::Vector2f mousepos)
{
	if (popup==nullptr)
	{
		bool to_return = true;

		if (focused != nullptr)
		{
			if ((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) && event.mouseButton.button == sf::Mouse::Button::Left)
			{
				if (hover != focused)
				{
					focused->changeState(GuiElement::States::nothing);//unfocused
					focused = nullptr;
				}
			}
			else focused->processEvent(event, mousepos);
			to_return = false;
		}
		else
		{
			if (hover != nullptr&&event.type == sf::Event::MouseButtonPressed&&event.mouseButton.button == sf::Mouse::Button::Left)
			{
				if (hover->isClickable())
				{
					held = hover;

					GuiElementEvent newevent;
					newevent.type = GuiElementEvent::EventType::Mouse;
					newevent.mouse.button = sf::Mouse::Button::Left;
					newevent.mouse.type = GuiElementEvent::Type::Pressed;
					newevent.mouse.mousepos = mousepos;
					hover->changeState(GuiElement::States::held);//pressed
					hover->notifyObserver(newevent);
					to_return = false;
				}
				else to_return = true;
			}
			else if (event.type == sf::Event::MouseButtonReleased&&event.mouseButton.button == sf::Mouse::Button::Left)
			{
				if (held != nullptr)
				{

					GuiElementEvent newevent;
					newevent.type = GuiElementEvent::EventType::Mouse;
					newevent.mouse.button = sf::Mouse::Button::Left;
					newevent.mouse.type = GuiElementEvent::Type::Released;
					newevent.mouse.mousepos = mousepos;

					if (held->isFocusable())
					{
						focused = held;
						held->changeState(GuiElement::States::focused);//focused
					}
					else held->changeState(GuiElement::States::nothing);//released

					held->notifyObserver(newevent);
					to_return = false;
					held = nullptr;
				}
			}
			else if (held != nullptr)
			{
				held->processEvent(event, mousepos);
				to_return = false;
			}
		}

		return to_return;
	}
	else return popup->processEvent(event, mousepos);
}

void GuiNS::Gui::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (popup==nullptr || popup->isDrawing())
		for (unsigned int i = 0; i < elements.size(); i++)
			target.draw(*elements[i], states);

	if (popup!=nullptr)
		target.draw(*popup);
}
