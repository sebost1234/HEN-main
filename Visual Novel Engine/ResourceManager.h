#pragma once

#include <SFML\Graphics.hpp>
#include <map>
#include <array>

#include <iostream>

enum StyleTypes
{
	transparentbackground,
	normal,
	blankwhite
};

namespace GuiNS
{
	struct Style
	{
		Style(sf::Color backcolor, sf::Color backcolor2, sf::Color decalcolor, sf::Color decalcolor2, sf::Color textcolor) :
			backcolor(backcolor), backcolor2(backcolor2), decalcolor(decalcolor), decalcolor2(decalcolor2), textcolor(textcolor)
		{}

		sf::Color backcolor;
		sf::Color backcolor2;
		sf::Color decalcolor;
		sf::Color decalcolor2;
		sf::Color textcolor;
	};
}


class ResourceManager
{
public:
	static sf::Font*getFont()
	{
		if (font == nullptr)
		{
			font = new sf::Font();
			font->loadFromFile("Data\\font.ttf");
		}
		return font;
	}
	static GuiNS::Style*getStyle(StyleTypes type = normal)
	{
		if (styles.count(type) == 0)
		{
			switch (type)
			{
			case normal:
				styles[type] = new GuiNS::Style(sf::Color(50, 50, 50), sf::Color(100, 100, 100), sf::Color(255, 0, 0), sf::Color(100, 0, 0), sf::Color::White);
				break;
			case transparentbackground:
				styles[type] = new GuiNS::Style(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color::Black);
				break;
			case blankwhite:
				styles[type] = new GuiNS::Style(sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White);
				break;
			default:
				break;
			}
		}
		return styles[type];
	}
	static sf::Texture*getTexture(std::string texture)
	{
		if (textures.count(texture) == 0)
		{
			textures[texture] = new sf::Texture();
			textures[texture]->loadFromFile(texture);
		}
		return textures[texture];
	}


	static void clear()
	{
		if (font != nullptr)
			delete font;
		for (auto&tmp : styles)
			if (tmp.second != nullptr)
				delete tmp.second;
		for (auto&tmp : textures)
			if (tmp.second != nullptr)
				delete tmp.second;
	}
private:
	static sf::Font*font;
	static std::map<StyleTypes, GuiNS::Style*> styles;
	static std::map<std::string, sf::Texture*> textures;
};