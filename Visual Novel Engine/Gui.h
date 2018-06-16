#pragma once

#include <algorithm>
#include <vector>
#include <SFML\Graphics.hpp>

#include "PlayerInput.h"
#include "SoundEngine.h"

#include "ResourceManager.h"

namespace GuiNS
{

	class GuiElementEvent
	{
	public:
		GuiElementEvent(){ }
		enum EventType
		{
			Mouse,
			Text,
			BarValueChanged
		};

		enum Type
		{
			Pressed,
			Released
		};

		struct MouseEvent
		{
			sf::Mouse::Button button;
			sf::Vector2f mousepos;
			Type type;
		};

		struct TextInput
		{
			sf::Uint32 unicode;
		};

		struct BarValue
		{
			int min;
			int state;
			int max;
		};

		EventType type;
		union
		{
			MouseEvent mouse;
			TextInput text;
			BarValue bar;
		};
	};

	class GuiElement;

	class GuiElementObserver
	{
	public:
		virtual void notifyEvent(GuiElementEvent event, GuiElement*from) = 0;
	};

	class Gui;

	class GuiEntity
	{

	};

	class GuiElement : public sf::Drawable, public GuiEntity
	{
	public:
		enum States : int
		{
			nothing,
			hover,
			held,
			focused
		};

		GuiElement()
		{
			clickable = true;
			focusable = false;
			observer = nullptr;
			gui = nullptr;
			style = nullptr;
		}
		virtual ~GuiElement()
		{

		}
		virtual void sync(float time)
		{

		}

		void setObserver(GuiElementObserver*_observer)
		{
			observer = _observer;
		}
		void deleteObserver()
		{
			observer = nullptr;
		}
		void notifyObserver(GuiElementEvent event)
		{
			if (observer != nullptr)
				observer->notifyEvent(event, this);
		}
		virtual void processEvent(sf::Event event, sf::Vector2f mousepos)
		{

		}
		virtual void heldEvent(sf::Vector2f mousepos, sf::Vector2f mousemove, float time)
		{

		}

		virtual void setStyle(Style*_style)
		{
			style = _style;
			changeState(nothing);
		}
		virtual void changeState(States newstate){}

		void move(sf::Vector2f move)
		{
			setPosition(move + getPosition());
		}
		virtual void setPosition(sf::Vector2f position) = 0;
		virtual sf::Vector2f getPosition() const = 0;
		virtual void setSize(sf::Vector2f size) = 0;
		virtual sf::Vector2f getSize() const = 0;

		void setGui(Gui*_gui)
		{
			gui = _gui;
		}
		Gui*getGui() const
		{
			return gui;
		}
		void setFocusable(bool _focusable)
		{
			focusable = _focusable;
		}
		void setClickable(bool _clickable)
		{
			clickable = _clickable;
		}
		bool isFocusable() const
		{
			return focusable;
		}
		bool isClickable() const
		{
			return clickable;
		}

		virtual bool contains(sf::Vector2f position) const
		{
			sf::Vector2f pos = getPosition();
			sf::Vector2f size = getSize();

			return sf::FloatRect(pos.x, pos.y, size.x, size.y).contains(position);
		}
	
	protected:
		GuiElementObserver * observer;
		Style*style;
		Gui*gui;

		bool clickable;
		bool focusable;
	};

	class Father : private GuiElement
	{
	public:
		virtual ~Father()
		{

		}
		virtual void processEvent(sf::Event event, sf::Vector2f mousepos)override
		{
			for (unsigned int i = 0; i < elements.size(); ++i)
				if (elements[i]->contains(mousepos))
				{
					elements[i]->processEvent(event, mousepos);
					return;
				}
		}
	protected:
		std::vector < GuiElement* > elements;
	};

	class Popup;

	class GuiEvent
	{
	public:
		enum EventType
		{
			PopupDeleted
		};

		GuiEvent(){}
		GuiEvent(EventType type):type(type) { }

		EventType type;
		union
		{
		};
	};


	class GuiObserver
	{
	public:
		virtual void notifyEvent(GuiEvent event, Gui*from) = 0;
	};

	class Gui : public sf::Drawable
	{
	public:
		Gui()
		{
			hover = nullptr;
			held = nullptr;
			focused = nullptr;
			popup = nullptr;
		}

		void addElement(GuiElement*toadd)
		{
			for (const auto tmp : elements)
				if (tmp == toadd)
					return;
			elements.push_back(toadd);
			toadd->setGui(this);
		}

		void eraseElement(GuiElement*toerase)
		{
			if (toerase != nullptr)
			{
				auto find = std::find(elements.begin(), elements.end(), toerase);
				if (find != elements.end())
				{
					elements.erase(find);
				}
				if (hover == toerase)
					hover = nullptr;
				if (held == toerase)
					held = nullptr;
				if (focused == toerase)
					focused = nullptr;
				toerase->changeState(GuiElement::States::nothing);
				toerase->setGui(nullptr);
			}
		}

		void sync(sf::Vector2f mousepos, float time);

		Popup*setPopup(Popup*newpopup)
		{
			Popup*tmp = popup;
			
			popup = newpopup;
			return tmp;
		}

		Popup*getCurrentPopup()
		{
			return popup;
		}

		bool isActivated()
		{
			return held != nullptr || focused != nullptr;
		}

		bool processEvent(sf::Event event, sf::Vector2f mousepos);

		bool hasPopup()
		{
			return popup!=nullptr;
		}

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
		
		void setObserver(GuiObserver*_observer)
		{
			observer = _observer;
		}
		void resetObserver()
		{
			observer = nullptr;
		}
	private:

		void deactivateAll()
		{
			hover = nullptr;
			held = nullptr;
			focused = nullptr;
		}

		GuiElement * hover;
		GuiElement * held;
		GuiElement * focused;
		std::vector <GuiElement*> elements;
		Popup*popup;

		GuiObserver*observer;

		sf::Vector2f mouseprevpos;
	};

	class GuiText : public GuiElement
	{
	public:
		virtual ~GuiText() {}
		enum FormatHor
		{
			Hor_Center = 0,
			Hor_Left = 1,
			Hor_Right = 2
		};

		enum FormatVer
		{
			Ver_Center = 0,
			Ver_Top = 1,
			Ver_Down = 2
		};

		enum Type
		{
			Nothing,
			Cut,
			NewLine
		};

		GuiText(Style*style, const sf::Font&font, sf::Vector2f backgroundsize, sf::String string = "ERROR", unsigned int size = 30, float paddingver = 1, float paddinghor = 5, FormatVer formatver = Ver_Down, FormatHor formathor = Hor_Left, Type type = Type::NewLine) :
			background(backgroundsize),
			text(string, font, size),
			string(string),
			type(type),
			formatver(formatver),
			formathor(formathor),
			paddingver(paddingver),
			paddinghor(paddinghor),
			nroflines(1),
			prefix(""),
			postfix(""),
			hidepostfix(false)
		{
			setStyle(style);
			setString(string);
			formatText();
		}

		virtual void setStyle(Style*style) override
		{
			background.setFillColor(style->backcolor);
			background.setOutlineColor(style->decalcolor);
			text.setFillColor(style->textcolor);
			GuiElement::setStyle(style);
		}

		virtual void changeState(States newstate) override
		{
			switch (newstate)
			{
			default:
			case GuiElement::nothing:
				background.setFillColor(style->backcolor);
				break;
			case GuiElement::hover:
				if (isClickable())
					SoundEngine::playSound("hover");
			case GuiElement::held:
			case GuiElement::focused:
				if(isClickable())
					background.setFillColor(style->backcolor2);
				else
					background.setFillColor(style->backcolor);
				break;
			}
		}

		void fitBackground(bool changewidth = true, bool changeheight = true)
		{
			sf::FloatRect textRect = text.getLocalBounds();
			float width, height;

			if (changewidth)
				width = textRect.width + paddinghor * 2;
			else
				width = background.getSize().x;

			if (changeheight)
				height = nroflines*text.getCharacterSize() + (text.getFont()->getLineSpacing(text.getCharacterSize()) - text.getCharacterSize())*(nroflines - 1) + paddingver * 2;
			else
				height = background.getSize().y;


			background.setSize(sf::Vector2f(width, height));
		}

		void formatText()
		{
			sf::FloatRect backRect = background.getLocalBounds();
			sf::FloatRect textRect = text.getLocalBounds();

			sf::Vector2f origin;
			sf::Vector2f position;

			switch (formathor)
			{
			default:
			case Hor_Center:
				origin.x = textRect.left +  textRect.width / 2.0f;
				position.x = background.getPosition().x + backRect.width / 2.0f;
				break;
			case Hor_Left:
				origin.x = 0;
				position.x = background.getPosition().x + paddinghor;
				break;
			case Hor_Right:
				origin.x = textRect.width;
				position.x = background.getPosition().x + backRect.width - paddinghor;
				break;
			}

			switch (formatver)
			{
			default:
			case Ver_Center:
				origin.y = textRect.top + textRect.height / 2.0f;
				position.y = background.getPosition().y + backRect.height / 2.0f;
				break;
			case Ver_Top:
				origin.y = 0;
				position.y = background.getPosition().y + paddingver;
				break;
			case Ver_Down:
				origin.y = textRect.height;
				position.y = background.getPosition().y + backRect.height - paddingver - text.getCharacterSize()*0.5f;
				break;
			}

			text.setOrigin(origin);
			text.setPosition(position);
		}

		int maxLines() const
		{
			return int((background.getSize().y - paddingver * 2) / (float)(text.getCharacterSize()*1.2));
		}


		void setString(sf::String newstring)
		{
			string = newstring;


			//Default state
			sf::Vector2f origin;
			sf::Vector2f position;
			origin.x = 0;
			position.x = background.getPosition().x + paddinghor;

			origin.y = 0;
			position.y = background.getPosition().y + paddingver;
			text.setOrigin(origin);
			text.setPosition(position);


			switch (type)
			{
			case Nothing:
				nroflines = 1;
				text.setString(getTextWithFixes());
				break;
			case Cut:
			{
				nroflines = 1;
				sf::String tmp = cutString(getTextWithFixes());
				if (tmp.getSize() < string.getSize() + postfix.getSize() + prefix.getSize())
				{
					if (hidepostfix)
					{
						string = string.substring(0, std::max((int)tmp.getSize() - (int)prefix.getSize(),0));
						text.setString(getTextWithFixes(true));
					}
					else
					{
						string = string.substring(0, std::max((int)tmp.getSize() - (int)prefix.getSize() - (int)postfix.getSize(), 0));
						text.setString(getTextWithFixes());
					}
				}
				break;
			}
			case NewLine:
			{
				nroflines = 1;
				text.setString(getTextWithFixes());
				int lastspace = 0;
				for (int i = 0; i < int(text.getString().getSize()); ++i)
				{
					if (text.findCharacterPos(i + 1).x + paddinghor >= getSize().x + getPosition().x)
					{
						nroflines++;
						if (nroflines > maxLines())
						{

							if (hidepostfix)
							{
								text.setString(text.getString().substring(0, i));
								string = text.getString().substring(prefix.getSize(), std::max(i - (int)prefix.getSize(),0));

								text.setString(getTextWithFixes(true));
							}
							else
							{
								text.setString(text.getString().substring(0, i));
								string = text.getString().substring(prefix.getSize(), std::max(i - (int)prefix.getSize() - (int)postfix.getSize(),0));

								text.setString(getTextWithFixes());
							}

							for (int o = string.getSize() - 1; o >= 0; --o)
								if (string[o] == '\n')
									string.erase(o, 1);
							break;
						}
						else
						{
							auto str = text.getString();
							str.insert(lastspace+1, "\n");
							text.setString(str);
							i++;
						}
					}
					if (text.getString()[i] == ' ')
						lastspace = i;
				}
			}
			break;
			default:
				break;
			}


			formatText();
			
		}
		void forceSetString(sf::String newstring)
		{
			string = newstring;
			nroflines = 1;
			text.setString(getTextWithFixes());
		}

		void appendString(sf::String newstring)
		{
			string += newstring;

			switch (type)
			{
			case Nothing:
				nroflines = 1;
				text.setString(getTextWithFixes());
				break;
			case Cut:
			{
				nroflines = 1;
				sf::String tmp = cutString(getTextWithFixes());
				if (tmp.getSize() < string.getSize() + postfix.getSize() + prefix.getSize())
				{
					if (hidepostfix)
					{
						string = string.substring(0, std::max((int)tmp.getSize() - (int)prefix.getSize(), 0));
						text.setString(getTextWithFixes(true));
					}
					else
					{
						string = string.substring(0, std::max((int)tmp.getSize() - (int)prefix.getSize() - (int)postfix.getSize(), 0));
						text.setString(getTextWithFixes());
					}
				}
				break;
			}
			case NewLine:
			{
				nroflines = 1;
				text.setString(getTextWithFixes());
				for (int i = 0; i < int(text.getString().getSize()); ++i)
				{
					if (text.findCharacterPos(i+1).x + paddinghor >= getSize().x + getPosition().x )
					{
						nroflines++;
						if (nroflines > maxLines())
						{

							if (hidepostfix)
							{
								text.setString(text.getString().substring(0, i));
								string = text.getString().substring(prefix.getSize(), std::max(i - (int)prefix.getSize(), 0));

								text.setString(getTextWithFixes(true));
							}
							else
							{
								text.setString(text.getString().substring(0, i));
								string = text.getString().substring(prefix.getSize(), std::max(i - (int)prefix.getSize() - (int)postfix.getSize(), 0));

								text.setString(getTextWithFixes());
							}

							for (int o = string.getSize() - 1; o >= 0; --o)
								if (string[o] == '\n')
									string.erase(o, 1);



							break;
						}
						else
						{
							auto str = text.getString();
							str.insert(i, "\n");
							text.setString(str);
							i++;
						}
					}
				}
			}
			break;
			default:
				break;
			}


			formatText();
		}
		void deleteLast()
		{
			if (string.getSize() > 0)
			{
				string.erase(string.getSize() - 1, 1);
				setString(string);
			}
		}
		
		sf::RectangleShape*changeBackground()
		{
			return &background;
		}
		sf::Text*changeTextApperance()
		{
			return &text;
		}
		sf::String getString()
		{
			return string;
		}
		sf::String getTextWithFixes(bool hiddenpostfix = false)
		{
			if(hiddenpostfix)
				return prefix + string;
			else
				return prefix + string + postfix;
		}
		sf::String getVisibleString()
		{
			return text.getString();
		}

		void setPrefix(sf::String text)
		{
			prefix = text;
			setString(string);
		}
		void setPostfix(sf::String text)
		{
			postfix = text;
			setString(string);
		}
		void hidePostfix(bool status) 
		{ 
			hidepostfix = status;
		}

		sf::String getPrefix() const
		{
			return prefix;
		}
		sf::String getPostfix() const
		{
			return postfix;
		}

		virtual void setPosition(sf::Vector2f position) override
		{
			text.move(position - background.getPosition());
			background.setPosition(position);
		}
		virtual sf::Vector2f getPosition() const override
		{
			return background.getPosition();
		}
		virtual void setSize(sf::Vector2f size) override
		{
			background.setSize(size);
			setString(string);
			formatText();
		}
		virtual sf::Vector2f getSize() const override
		{
			return background.getSize();
		}

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(background, states);
			target.draw(text, states);
		}
		virtual void processEvent(sf::Event event, sf::Vector2f mousepos)override
		{
			GuiElementEvent newevent;
			newevent.type = GuiElementEvent::EventType::Mouse;
			if (event.type == sf::Event::MouseButtonPressed)
			{
				newevent.mouse.button = event.mouseButton.button;
				newevent.mouse.type = GuiElementEvent::Type::Pressed;
				notifyObserver(newevent);
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				newevent.mouse.button = event.mouseButton.button;
				newevent.mouse.type = GuiElementEvent::Type::Released;
				notifyObserver(newevent);
			}
		}
	private:
		sf::String cutString(sf::String tocut)
		{
			text.setOrigin(sf::Vector2f(paddinghor, 0));
			text.setPosition(background.getPosition());
			text.setString(tocut);

			int i = tocut.getSize();
			for (; i >= 0; i--)
			{
				if (text.findCharacterPos(i).x < getSize().x + getPosition().x - paddinghor*3)
					break;
			}
			return tocut.substring(0, i);
		}
		
	private:
		int nroflines;
		sf::String string;
		sf::Text text;
		sf::RectangleShape background;


		sf::String prefix;
		sf::String postfix;
		bool hidepostfix;

		float paddinghor;
		float paddingver;
		FormatHor formathor;
		FormatVer formatver;
		Type type;
	};

	class GuiTextInputBox : public GuiText
	{
	public:
		enum Types
		{
			All,
			Letters,
			Numbers
		};

		GuiTextInputBox(GuiText textbox, Types inputtype = All) :
			GuiText(textbox),
			inputtype(inputtype)
		{
			setFocusable(true);
			timer = 0;
			state = false;
			isFocused = false;
			hidePostfix(true);
		}


		virtual void changeState(States newstate) override
		{
			GuiText::changeState(newstate);
		
			switch (newstate)
			{
			default:
				isFocused = false;
				setPostfix(" ");
				break;	
			case GuiElement::focused:
				isFocused = true;
				timer = 0.5f;
				state = true;
				break;
			}
		}

		virtual void sync(float time) override
		{
			if (isFocused)
			{
				if (input.getSize() > 0)
				{
					sf::String tmp;
					for (unsigned int i = 0; i < input.getSize(); i++)
					{
						if (input[i] == char(8))//Delete
						{
							deleteLast();
						}
						else
						{
							switch (inputtype)
							{
							case All:
								if (input[i] >= 32 && input[i] <= 126)
									tmp += input[i];
								break;
							case Letters:
								if (isalpha(input[i]) || input[i] == ' ')
									tmp += input[i];
								break;
							case Numbers:
								if (isdigit(input[i]))
									tmp += input[i];
								break;
							default:
								break;
							}
						}
					}
					appendString(tmp);
					formatText();
					input = "";
				}

				timer += time;
				if (timer > 0.5f)
				{
					if (state)
						setPostfix("_");
					else
						setPostfix(" ");
					timer = 0;
					state = !state;
				}
			}
		}

		void clean()
		{
			setString("");
		}

		virtual void processEvent(sf::Event event, sf::Vector2f mousepos) override
		{
			GuiElementEvent newevent;
			if (event.type == sf::Event::TextEntered)
			{
				newevent.type = GuiElementEvent::EventType::Text;
				auto tmp = static_cast<char>(event.text.unicode);
				newevent.text.unicode = tmp;
				input += sf::String(tmp);
				notifyObserver(newevent);
			}
		}
	private:

		Types inputtype;
		sf::String input;

		float timer;
		bool state;
		bool isFocused;
	};

	class GuiTextSlowShow : public GuiText
	{
	public:
		GuiTextSlowShow(GuiText textbox, float speed = 0.010f) :
			GuiText(textbox),
			new_every(speed)
		{
			timer = 0;
		}

		void show()
		{
			setString(getString() + to_show);
			to_show.clear();
		}

		void setText(sf::String string)
		{
			setString("");
			to_show = string;
		}

		void addText(sf::String string)
		{
			to_show += string;
		}

		bool isDone() const
		{
			return to_show.getSize() == 0;
		}

		virtual void sync(float time) override
		{
			timer += time;
			if (timer > new_every)
			{
				if (to_show.getSize() > 0)
				{
					setString(getString() + to_show[0]);
					to_show.erase(0, 1);
					SoundEngine::playSound("Type");
				}
				timer = 0;
			}
		}
	private:
		sf::String to_show;

		float timer;
		float new_every;
	};

	class GuiRectangle : public GuiElement
	{
	public:
		GuiRectangle(Style*style, sf::Vector2f backgroundsize) :
			rectangle(backgroundsize)
		{
			setStyle(style);
		}

		virtual void setStyle(Style*style) override
		{
			rectangle.setFillColor(style->backcolor);
			rectangle.setOutlineColor(style->decalcolor);
			GuiElement::setStyle(style);
		}
		virtual void changeState(States newstate)
		{
			switch (newstate)
			{
			default:
			case GuiElement::nothing:
				rectangle.setFillColor(style->backcolor);
				break;

			case GuiElement::States::hover:
				if (isClickable())
					SoundEngine::playSound("hover");	
			case GuiElement::held:
				if (isClickable())
					rectangle.setFillColor(style->backcolor2);
				else
					rectangle.setFillColor(style->backcolor);
				break;
			}
		}

		virtual void setPosition(sf::Vector2f position) override
		{
			rectangle.setPosition(position);
		}
		virtual sf::Vector2f getPosition() const override
		{
			return rectangle.getPosition();
		}
		virtual void setSize(sf::Vector2f size) override
		{
			rectangle.setSize(size);
		}
		virtual sf::Vector2f getSize() const override
		{
			return rectangle.getSize();
		}


		virtual void heldEvent(sf::Vector2f mousepos, sf::Vector2f mousemove, float time) override
		{
		}

		sf::RectangleShape*changeRectangle()
		{
			return &rectangle;
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(rectangle, states);
		}
	private:
		sf::RectangleShape rectangle;

	};

	class GuiBar : public GuiElement
	{
	public:
		enum Type
		{
			Horizontal,
			Vertical
		};

		GuiBar(Style*style, sf::Vector2f backgroundsize, Type type) :
			background(backgroundsize), type(type)
		{
			min = 1;
			state = 1;
			max = 1;

			_sync();

			setStyle(style);
		}

		
		virtual void changeState(States newstate)
		{
			switch (newstate)
			{
			default:
			case GuiElement::nothing:
				bar.setFillColor(style->decalcolor);
				break;
			case GuiElement::held:
				bar.setFillColor(style->decalcolor2);
				break;
			}
		}
		virtual void setStyle(Style*style) override
		{
			background.setFillColor(style->backcolor);
			background.setOutlineColor(style->decalcolor);
			bar.setFillColor(style->decalcolor);
			GuiElement::setStyle(style);
		}
		
		void _changeState(int value)
		{
			state = value;
			_sync();
		}
		void _changeMin(int value)
		{
			min = value;
			_sync();
		}
		void _changeMax(int value)
		{
			max = value+1;
			_sync();
		}

		int _getState()
		{
			return state + 1;
		}

		virtual void setPosition(sf::Vector2f position) override
		{
			background.setPosition(position);
			_sync();
		}
		virtual sf::Vector2f getPosition() const override
		{
			return background.getPosition();
		}
		virtual void setSize(sf::Vector2f size) override
		{
			background.setSize(size);
			_sync();
		}
		virtual sf::Vector2f getSize() const override
		{
			return background.getSize();
		}
		
		virtual void heldEvent(sf::Vector2f mousepos, sf::Vector2f mousemove, float time) override
		{
			int newstate;

			if (type == Vertical)
				newstate = int((mousepos.y - background.getPosition().y) / (background.getSize().y*(float(min) / max)));
			else
				newstate = int((mousepos.x - background.getPosition().x) / (background.getSize().x*(float(min) / max)));

			if (newstate != state)
			{
				state = newstate;
				_sync();
				if (state == newstate)
				{
					GuiElementEvent newevent;
					newevent.type = GuiElementEvent::EventType::BarValueChanged;
					newevent.bar.min = min - 1;
					newevent.bar.max = max - 1;
					newevent.bar.state = state;

					notifyObserver(newevent);
				}
			}
		}

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(background, states);
			target.draw(bar, states);
		}
	private:
		void _sync()
		{
			if(type == Vertical)
				bar.setSize(sf::Vector2f(background.getSize().x, background.getSize().y*(float(min) / max)));
			else
				bar.setSize(sf::Vector2f(background.getSize().x*(float(min) / max), background.getSize().y));


			if (state < 0)
				state = 0;
			else if (state > max-1)
				state = max-1;

			if (type == Vertical)
				bar.setPosition(background.getPosition().x, background.getPosition().y + state*background.getSize().y*(float(min) / max));
			else
				bar.setPosition(background.getPosition().x + state*background.getSize().x*(float(min) / max), background.getPosition().y);
		}

		sf::RectangleShape bar;
		sf::RectangleShape background;
		int min;
		int state;
		int max;

		Type type;
	};

	class HolderVertical : public GuiElement
	{
	public:
		HolderVertical(Style*style, sf::Vector2f position, sf::Vector2f size,
			float padding_between = 10,
			float padding_top = 10,
			float padding_bottom = 10,
			float padding_left = 10,
			float padding_right = 10
		) :
			padding_between(padding_between),
			padding_top(padding_top),
			padding_bottom(padding_bottom),
			padding_left(padding_left),
			padding_right(padding_right)
		{
			background.setPosition(position);
			background.setSize(size);

			setStyle(style);
		}

		virtual void setStyle(Style*style) override
		{
			background.setFillColor(style->backcolor);
			background.setOutlineColor(style->decalcolor);
			for (unsigned int i = 0; i < elements.size(); i++)
				elements[i]->setStyle(style);
			GuiElement::setStyle(style);
		}

		bool _addElement(GuiElement*toadd)
		{
			if (_formatElement(toadd, elements.size()))
			{
				elements.push_back(toadd);
				return true;
			}
			else return false;
		}
		bool _formatElement(GuiElement*toformat, int nr)
		{
			if (nr == 0)
				toformat->setPosition(background.getPosition() + sf::Vector2f(padding_left, padding_top));
			else
				toformat->setPosition(sf::Vector2f(background.getPosition().x + padding_left, padding_between + elements[nr - 1]->getPosition().y + elements[nr - 1]->getSize().y));


			toformat->setSize(sf::Vector2f(background.getSize().x - padding_left - padding_right, toformat->getSize().y));


			float tmp = (background.getPosition().y + background.getSize().y) - (toformat->getPosition().y + toformat->getSize().y + padding_bottom);
			if (tmp < 0)
			{
				if (toformat->getSize().y + tmp > 0)
					toformat->setSize(sf::Vector2f(toformat->getSize().x, toformat->getSize().y + tmp));
				else
					return false;
			}

			return true;
		}

		void _clearElements(bool deletion = false)
		{
			if (deletion)
				for (const auto&tmp : elements)
					delete tmp;
			elements.clear();
		}
		void _formatElements()
		{
			for (unsigned int i = 0; i < elements.size(); i++)
				_formatElement(elements[i], i);
		}

		sf::RectangleShape*changeBackground()
		{
			return &background;
		}

		virtual void setPosition(sf::Vector2f newPosition) override
		{
			sf::Vector2f move = newPosition - background.getPosition();

			for (auto&tmp : elements)
				tmp->setPosition(tmp->getPosition() + move);

			background.setPosition(newPosition);
		}
		virtual sf::Vector2f getPosition() const override
		{
			return background.getPosition();
		}

		virtual void setSize(sf::Vector2f newSize) override
		{
			background.setSize(newSize);

			_formatElements();
		}
		virtual sf::Vector2f getSize() const override
		{
			return background.getSize();
		}

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(background, states);
			for (const auto&tmp : elements)
				target.draw(*tmp, states);
		}
	protected:
		//std::vector <GuiElement*> elements; FROM FATHER

		sf::RectangleShape background;

		float padding_between;
		float padding_top;
		float padding_bottom;
		float padding_left;
		float padding_right;

		std::vector <GuiElement*> elements;
	};

	class HolderHorizontal : public GuiElement
	{
	public:
		HolderHorizontal(Style*style, sf::Vector2f position, sf::Vector2f size,
			float padding_between = 10,
			float padding_top = 10,
			float padding_bottom = 10,
			float padding_left = 10,
			float padding_right = 10
		) :
			padding_between(padding_between),
			padding_top(padding_top),
			padding_bottom(padding_bottom),
			padding_left(padding_left),
			padding_right(padding_right)
		{
			background.setPosition(position);
			background.setSize(size);

			background.setFillColor(style->backcolor);

			setStyle(style);
		}


		void _addElement(GuiElement*toadd)
		{
			_formatElement(toadd, elements.empty());
			elements.push_back(toadd);
		}
		void _formatElement(GuiElement*toformat, bool first = false)
		{
			if (first)
				toformat->setPosition(background.getPosition() + sf::Vector2f(padding_left, padding_top));
			else
				toformat->setPosition(sf::Vector2f(elements.back()->getPosition().x + elements.back()->getSize().x + padding_between, background.getPosition().y + padding_top));

			toformat->setSize(sf::Vector2f(toformat->getSize().x, background.getSize().y - padding_top - padding_bottom));
		}

		void _clearElements(bool deletion = false)
		{
			if (deletion)
				for (const auto&tmp : elements)
					delete tmp;
			elements.clear();
		}
		void _formatElements()
		{

			for (unsigned int i = 0; i < elements.size(); i++)
				_formatElement(elements[i], i == 0);
		}

		sf::RectangleShape*changeBackground()
		{
			return &background;
		}

		virtual void setPosition(sf::Vector2f newPosition) override
		{
			sf::Vector2f move = newPosition - background.getPosition();

			for (auto&tmp : elements)
				tmp->setPosition(tmp->getPosition() + move);
			background.setPosition(newPosition);
		}
		virtual sf::Vector2f getPosition() const override
		{
			return background.getPosition();
		}

		virtual void setSize(sf::Vector2f newSize) override
		{
			background.setSize(newSize);

			for (auto&tmp : elements)
				tmp->setSize(sf::Vector2f(tmp->getSize().x, background.getSize().y - padding_top - padding_bottom));

		}
		virtual sf::Vector2f getSize() const override
		{
			return background.getSize();
		}

		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(background, states);
			for (const auto&tmp : elements)
				target.draw(*tmp, states);
		}
	protected:
		//std::vector <GuiElement*> elements; FROM FATHER

		sf::RectangleShape background;

		float padding_between;
		float padding_top;
		float padding_bottom;
		float padding_left;
		float padding_right;

		std::vector <GuiElement*> elements;
	};
}

#include "Popup.h"