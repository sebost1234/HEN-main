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
			Mouse=0,
			Text=1,
			BarValueChanged=2
		};
		
		enum Type
		{
			Pressed=0,
			Released=1,
			Hover=2,
			Unhover=3
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
			frozen = false;
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
			changeState(currentstate);
		}

		void changeState(States newstate)
		{
			if (!frozen)
			{
				changeStateInternal(newstate);

				if (newstate == States::hover)
				{
					GuiElementEvent newevent;
					newevent.type = GuiElementEvent::EventType::Mouse;
					newevent.mouse.type = GuiElementEvent::Type::Hover;
					notifyObserver(newevent);
				}
				else if (currentstate == States::hover&&newstate != States::hover)
				{
					GuiElementEvent newevent;
					newevent.type = GuiElementEvent::EventType::Mouse;
					newevent.mouse.type = GuiElementEvent::Type::Unhover;
					notifyObserver(newevent);
				}
			}
			currentstate = newstate;
		}

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
		void setFrozen(bool _frozen)
		{
			frozen = _frozen;
		}
		bool isFocusable() const
		{
			return focusable;
		}
		bool isClickable() const
		{
			return clickable;
		}
		bool isFrozen() const
		{
			return frozen;
		}

		virtual bool contains(sf::Vector2f position) const
		{
			sf::Vector2f pos = getPosition();
			sf::Vector2f size = getSize();

			return sf::FloatRect(pos.x, pos.y, size.x, size.y).contains(position);
		}
	
	protected:
		virtual void changeStateInternal(States newstate)
		{
		}
		States currentstate;
		GuiElementObserver * observer;
		Style*style;
		Gui*gui;

		bool clickable;
		bool focusable;
		bool frozen;
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

		virtual void changeStateInternal(States newstate) override
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
			formatText();
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
					if (text.getString()[i] == ' ')
					{
						if (text.findCharacterPos(i + 1).x + paddinghor >= getSize().x + getPosition().x)
						{
							nroflines++;
							auto str = text.getString();
							str.insert(lastspace + 1, "\n");
							text.setString(str);
							i++;
						}
						lastspace = i;
					}
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
		
	protected:
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

	class GuiTextSprite : public GuiText
	{
	public:
		GuiTextSprite(GuiText text, std::string nothingtexture = "none", std::string hovertexture = "none", std::string heldtexture = "none", std::string location = "", int border = 0) :
			GuiText(text), nothingtexture(location + nothingtexture), hovertexture(location + hovertexture), heldtexture(location + heldtexture), border(border)
		{
			changeState(nothing);
		}

		virtual void changeStateInternal(States newstate) override
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
				if (isClickable())
					background.setFillColor(style->backcolor2);
				else
					background.setFillColor(style->backcolor);

				break;
			}
			sync(newstate);
		}

		void sync(States newstate)
		{
			switch (newstate)
			{
			default:
			case GuiNS::GuiElement::nothing:
				if (nothingtexture != "none")
					background.setTexture(ResourceManager::getTexture(nothingtexture));
				else
					background.setTexture(NULL);
				break;
			case GuiNS::GuiElement::hover:
				if (isClickable())
				{
					if (hovertexture != "none")
						background.setTexture(ResourceManager::getTexture(hovertexture));
					else
						background.setTexture(NULL);
				}
				else
				{
					if (nothingtexture != "none")
						background.setTexture(ResourceManager::getTexture(nothingtexture));
					else
						background.setTexture(NULL);
				}

				break;
			case GuiNS::GuiElement::held:
			case GuiNS::GuiElement::focused:
				if (nothingtexture != "none")
					background.setTexture(ResourceManager::getTexture(heldtexture));
				else
					background.setTexture(NULL);
				break;
			}
			background.setTextureRect(sf::IntRect(border, border, background.getTexture()->getSize().x - border * 2, background.getTexture()->getSize().y - border * 2));
		}

		void setNothingTexture(std::string newtexture)
		{
			nothingtexture = newtexture;
		}
		void setHoverTexture(std::string newtexture)
		{
			hovertexture = newtexture;
		}
		void setHeldTexture(std::string newtexture)
		{
			heldtexture = newtexture;
		}
	private:
		std::string nothingtexture;
		std::string hovertexture;
		std::string heldtexture;
		int border;
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


		virtual void changeStateInternal(States newstate) override
		{
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
			forceSetString(getVisibleString() + to_show);
			to_show.clear();
		}

		void setText(sf::String string)
		{
			setString(string);
			to_show = getVisibleString();
			setString("");
		}

		void addText(sf::String string)
		{
			sf::String current = getVisibleString();
			to_show += string;
			setString(current+to_show);

			to_show = getVisibleString();

			if(to_show.getSize()>=current.getSize())
				to_show = to_show.substring(current.getSize());
			else to_show = "";

			setString(current);
		}

		bool isDone() const
		{
			return to_show.getSize() == 0;
		}

		void setSpeed(float newspeed)
		{
			new_every = newspeed;
		}

		virtual void sync(float time) override
		{
			timer += time;
			if (timer > new_every)
			{
				if (to_show.getSize() > 0)
				{
					forceSetString(getString() + to_show[0]);
					to_show.erase(0, 1);
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
			background(backgroundsize)
		{
			setStyle(style);
		}

		virtual void setStyle(Style*style) override
		{
			background.setFillColor(style->backcolor);
			background.setOutlineColor(style->decalcolor);
			GuiElement::setStyle(style);
		}
		virtual void changeStateInternal(States newstate)
		{
			switch (newstate)
			{
			default:
			case GuiElement::nothing:
				background.setFillColor(style->backcolor);
				break;

			case GuiElement::States::hover:
				if (isClickable())
					SoundEngine::playSound("hover");	
			case GuiElement::held:
				if (isClickable())
					background.setFillColor(style->backcolor2);
				else
					background.setFillColor(style->backcolor);
				break;
			}
		}

		virtual void setPosition(sf::Vector2f position) override
		{
			background.setPosition(position);
		}
		virtual sf::Vector2f getPosition() const override
		{
			return background.getPosition();
		}
		virtual void setSize(sf::Vector2f size) override
		{
			background.setSize(size);
		}
		virtual sf::Vector2f getSize() const override
		{
			return background.getSize();
		}

		sf::RectangleShape*changeRectangle()
		{
			return &background;
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(background, states);
		}
	protected:
		sf::RectangleShape background;
	};

	class GuiSprite : public GuiElement
	{
	public:
		GuiSprite(Style*style)
		{
			background.setTexture(*ResourceManager::getBigTexture("Data\\empty.png"));
			setStyle(style);
		}

		void setTexture(thor::BigTexture*texture)
		{
			background.setTexture(*texture);
		}
	

		virtual void setStyle(Style*style) override
		{
			background.setColor(style->backcolor);
			GuiElement::setStyle(style);
		}
		virtual void changeStateInternal(States newstate)
		{
			switch (newstate)
			{
			default:
			case GuiElement::nothing:
				background.setColor(style->backcolor);
				break;

			case GuiElement::States::hover:
				if (isClickable())
					SoundEngine::playSound("hover");
			case GuiElement::held:
				if (isClickable())
					background.setColor(style->backcolor2);
				else
					background.setColor(style->backcolor);
				break;
			}
		}

		virtual void setPosition(sf::Vector2f position) override
		{
			background.setPosition(position);
		}
		virtual sf::Vector2f getPosition() const override
		{
			return background.getPosition();
		}
		virtual void setSize(sf::Vector2f size) override
		{
			auto tmp = background.getLocalBounds();
			background.setScale(size.x / tmp.width, size.y / tmp.height);
		}
		virtual sf::Vector2f getSize() const override
		{
			return sf::Vector2f(background.getLocalBounds().width, background.getLocalBounds().height);
		}

		thor::BigSprite*changeSprite()
		{
			return &background;
		}
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
		{
			target.draw(background, states);
		}
	protected:
		thor::BigSprite background;
	};

	class GuiRectangleSprite : public GuiRectangle
	{
	public:
		GuiRectangleSprite(GuiRectangle rectangle, std::string nothingtexture = "none", std::string hovertexture = "none", std::string heldtexture = "none", std::string location = "", int border = 0, bool flipver = false) :
			GuiRectangle(rectangle), nothingtexture(location + nothingtexture), hovertexture(location + hovertexture), heldtexture(location + heldtexture), border(border), flipver(flipver)
		{
			changeState(nothing);
		}

		virtual void changeStateInternal(States newstate) override
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
				if (isClickable())
					background.setFillColor(style->backcolor2);
				else
					background.setFillColor(style->backcolor);

				break;
			}
			sync(newstate);
		}

		void sync(States newstate)
		{
			switch (newstate)
			{
			default:
			case GuiNS::GuiElement::nothing:
				if (nothingtexture != "none")
					background.setTexture(ResourceManager::getTexture(nothingtexture));
				else
					background.setTexture(NULL);
				break;
			case GuiNS::GuiElement::hover:
				if (hovertexture != "none")
					background.setTexture(ResourceManager::getTexture(hovertexture));
				else
					background.setTexture(NULL);
				break;
			case GuiNS::GuiElement::held:
			case GuiNS::GuiElement::focused:
				if (nothingtexture != "none")
					background.setTexture(ResourceManager::getTexture(heldtexture));
				else
					background.setTexture(NULL);
				break;
			}
			if(!flipver)
				background.setTextureRect(sf::IntRect(border, border, background.getTexture()->getSize().x-border*2, background.getTexture()->getSize().y - border * 2));
			else
				background.setTextureRect(sf::IntRect(int(background.getTexture()->getSize().x) - border, border, -(int(background.getTexture()->getSize().x) - border * 2), background.getTexture()->getSize().y - border * 2));
		}

		void setNothingTexture(std::string newtexture)
		{
			nothingtexture = newtexture;
		}
		void setHoverTexture(std::string newtexture)
		{
			hovertexture = newtexture;
		}
		void setHeldTexture(std::string newtexture)
		{
			heldtexture = newtexture;
		}
	private:
		std::string nothingtexture;
		std::string hovertexture;
		std::string heldtexture;
		int border;

		bool flipver;
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

		
		virtual void changeStateInternal(States newstate)
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

	class GuiBarSprite : public GuiElement
	{
	public:

		GuiBarSprite(Style*style, sf::Vector2f backgroundsize, sf::Vector2f barsize, std::string nothingtexture = "none", std::string hovertexture = "none", std::string heldtexture = "none", std::string backgroundtexture = "none", std::string location = "") :
			background(backgroundsize), bar(barsize), nothingtexture(location + nothingtexture), hovertexture(location + hovertexture), heldtexture(location + heldtexture), backgroundtexture(location + backgroundtexture)
		{
			min = 1;
			state = 1;
			max = 1;

			bar.setOrigin(barsize.x*0.5f, 0);

			setStyle(style);

			_sync();
		}

		virtual void changeStateInternal(States newstate)
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

			if (backgroundtexture != "none")
				background.setTexture(ResourceManager::getTexture(backgroundtexture));
			else
				background.setTexture(NULL);

			switch (newstate)
			{
			default:
			case GuiNS::GuiElement::nothing:
				if (nothingtexture != "none")
					bar.setTexture(ResourceManager::getTexture(nothingtexture));
				else
					bar.setTexture(NULL);
				break;
			case GuiNS::GuiElement::hover:
				if (hovertexture != "none")
					bar.setTexture(ResourceManager::getTexture(hovertexture));
				else
					bar.setTexture(NULL);
				break;
			case GuiNS::GuiElement::held:
			case GuiNS::GuiElement::focused:
				if (nothingtexture != "none")
					bar.setTexture(ResourceManager::getTexture(heldtexture));
				else
					bar.setTexture(NULL);
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
			max = value + 1;
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
			int newstate = int((mousepos.x - background.getPosition().x) / ((background.getSize().x-bar.getSize().x)*(float(min) / (max-1))));

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
			if (state < 0)
				state = 0;
			else if (state > max - 1)
				state = max - 1;


			bar.setPosition(background.getPosition().x + state*(background.getSize().x - bar.getSize().x)*(float(min) / (max-1)) + bar.getSize().x*0.5f, background.getPosition().y);
		}

		sf::RectangleShape bar;
		sf::RectangleShape background;
		int min;
		int state;
		int max;

		std::string nothingtexture;
		std::string hovertexture;
		std::string heldtexture;
		std::string backgroundtexture;
	};
}

#include "Popup.h"