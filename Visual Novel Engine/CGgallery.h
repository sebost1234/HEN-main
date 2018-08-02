#pragma once

#include "OptionsSubtype.h"


struct CGdisplay
{
	CGdisplay(sf::Vector2f offset, sf::Vector2f size, int x, int y, int padding):
		background(ResourceManager::getStyle(StyleTypes::blankwhiteshaded), size)
	{
		sf::Vector2f pos = offset + sf::Vector2f((size.x + padding) * x, (size.y + padding)*y);
		pos.x = float(int(pos.x));
		pos.y = float(int(pos.y));

		background.setPosition(pos);
		background.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\EmptySavePicture.png"), true);
	}

	void clear()
	{
		background.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\EmptySavePicture.png"), true);
		background.setClickable(false);
	}

	void sync(std::string texture, bool unlocked)
	{
		if (unlocked)
		{
			background.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\Game\\" + texture));
			background.changeRectangle()->setTextureRect(sf::IntRect(0, 0, int(gamesize.x), int(gamesize.y)));
			background.setClickable(true);
		}
		else
		{
			background.changeRectangle()->setTexture(ResourceManager::getTexture("Data\\CGnotunlocked.png"), true);
			background.setClickable(false);
		}
	}

	void enable(GuiNS::Gui*gui)
	{
		gui->addElement(&background);
	}

	void disable(GuiNS::Gui*gui)
	{
		gui->eraseElement(&background);
	}

	GuiNS::GuiRectangle background;
};

class CGGallery : public OptionsSubType
{
public:
	CGGallery(Options*options);

	virtual void enable(GuiNS::Gui*gui) override
	{
		for (unsigned int i = 0; i < cgs.size(); i++)
			cgs[i].enable(gui);
		gui->addElement(&pagename);

		gui->addElement(&chapter1info);
		gui->addElement(&chapter1page1);
		gui->addElement(&chapter1page2);
	}

	virtual void disable(GuiNS::Gui*gui)override
	{
		for (unsigned int i = 0; i < cgs.size(); i++)
			cgs[i].disable(gui);
		gui->eraseElement(&pagename);

		gui->eraseElement(&chapter1info);
		gui->eraseElement(&chapter1page1);
		gui->eraseElement(&chapter1page2);
		gui->eraseElement(&fullscreen);
	}

	void sync(int newpage)
	{
		int nr = (newpage-1)*rows*columns;
		for (unsigned int i = 0; i < cgs.size(); i++)
		{
			cgs[i].clear();
			if (nr+i < cglist.size())
				cgs[i].sync(cglist[nr+i], hasCG(cglist[nr + i]));
		}
		currentpage = newpage;
	}

	bool hasCG(std::string path)
	{
		for (unsigned int i = 0; i < seencglist.size(); i++)
			if (seencglist[i] == path)
				return true;
		return false;
	}

	virtual OptionsSubTypeEnum getSubtype()
	{
		return OptionsSubTypeEnum::Gallery_ST;
	}

	void notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from) override;
private:
	const unsigned int rows;
	const unsigned int columns;
	std::vector<CGdisplay> cgs;

	std::vector<std::string> cglist;
	std::vector<std::string> seencglist;

	GuiNS::GuiText chapter1info;
	GuiNS::GuiTextSprite chapter1page1;
	GuiNS::GuiTextSprite chapter1page2;


	GuiNS::GuiRectangle fullscreen;


	GuiNS::GuiText pagename;
	int currentpage;
};