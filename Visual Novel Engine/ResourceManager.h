#pragma once

#include <SFML\Graphics.hpp>
#include <map>
#include <array>

#include "Gui.h"

#include <iostream>

enum StyleTypes
{
	transparent,
	normal,
	blankwhite
};

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
	static Gui::Style*getStyle(StyleTypes type = transparent)
	{
		if (styles.count(type) == 0)
		{
			switch (type)
			{
			case transparent:
				styles[type] = new Gui::Style(sf::Color(50, 50, 50, 150), sf::Color(100, 100, 100, 150), sf::Color(255, 0, 0, 150), sf::Color(100, 0, 0, 150), sf::Color::White);
				break;
			case normal:
				styles[type] = new Gui::Style(sf::Color(50, 50, 50), sf::Color(100, 100, 100), sf::Color(255, 0, 0), sf::Color(100, 0, 0), sf::Color::White);
				break;
			case blankwhite:
				styles[type] = new Gui::Style(sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White);
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
	static std::map<StyleTypes, Gui::Style*> styles;
	static std::map<std::string, sf::Texture*> textures;
};