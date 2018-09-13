#pragma once

#include <SFML\Graphics.hpp>
#include <Thor\Graphics.hpp>
#include <map>
#include <array>

#include <iostream>

enum StyleTypes
{
	transparentbackgrounddarktext,
	transparentbackgroundwhitetext,
	normal,
	blankwhite,
	blankwhiteshaded
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
			case transparentbackgrounddarktext:
				styles[type] = new GuiNS::Style(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color::Black);
				break;
			case transparentbackgroundwhitetext:
				styles[type] = new GuiNS::Style(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color::White);
				break;
			case blankwhite:
				styles[type] = new GuiNS::Style(sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White);
				break;
			case blankwhiteshaded:
				styles[type] = new GuiNS::Style(sf::Color::White, sf::Color(200,200,200), sf::Color::White, sf::Color::White, sf::Color::White);
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
			textures[texture] = new sf::Texture;
			textures[texture]->loadFromFile(texture);
			textures[texture]->setSmooth(true);
		}
		return textures[texture];
	}

	static thor::BigTexture*getBigTexture(std::string texture)
	{
		if (bigtextures.count(texture) == 0)
		{
			bigtextures[texture] = new thor::BigTexture();
			bigtextures[texture]->loadFromFile(texture);
			bigtextures[texture]->setSmooth(true);
		}
		return bigtextures[texture];
	}


	static sf::Texture*getCharacterTexture(std::string texture, bool big = false)
	{
		if (charactertextures.count(texture) == 0)
		{
			sf::Texture tex;
			tex.loadFromFile(texture);
			tex.setSmooth(true);


			sf::Vector2i size = sf::Vector2i(sf::Vector2f(tex.getSize())*0.4f);
			sf::Sprite tmp2(tex);
			tmp2.setScale((float)size.x / tex.getSize().x, (float)size.y / tex.getSize().y);
			sf::RenderTexture tmp3;
			tmp3.setSmooth(true);
			tmp3.create(size.x, size.y);
			tmp3.clear(sf::Color::Transparent);
			tmp3.draw(tmp2);
			tmp3.display();

			charactertextures[texture].first = new sf::Texture(tex);
			charactertextures[texture].second = new sf::Texture(tmp3.getTexture());

			charactertextures[texture].second->setSmooth(true);
		}
		
		
		if (big)
			return charactertextures[texture].first;
		else
			return charactertextures[texture].second;
	}

	static sf::Texture*getSaveTexture(int slot)
	{
		if (savetextures.count(slot) == 0)
		{
			savetextures[slot] = new sf::Texture();
			savetextures[slot]->loadFromFile("Data\\Save\\" + std::to_string(slot) + ".png");
			savetextures[slot]->setSmooth(true);
		}
		return savetextures[slot];
	}

	static void deleteSaveTexture(int slot)
	{
		if (savetextures.count(slot) != 0)
		{
			if (savetextures[slot] != nullptr)
				delete savetextures[slot];
			savetextures.erase(slot);
		}
		remove(std::string("Data\\Save\\" + std::to_string(slot) + ".png").c_str());
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
		for (auto&tmp : savetextures)
			if (tmp.second != nullptr)
				delete tmp.second;
	}
private:
	static sf::Font*font;
	static std::map<StyleTypes, GuiNS::Style*> styles;
	static std::map<std::string, sf::Texture*> textures;
	static std::map<std::string, thor::BigTexture*> bigtextures;
	static std::map<int, sf::Texture*> savetextures;

	static std::map<std::string, std::pair<sf::Texture*, sf::Texture*>> charactertextures;
};