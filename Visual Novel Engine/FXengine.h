#pragma once

#include "Defines.h"
#include "VisualNovelEvent.h"

struct FXbase
{
	FXbase(std::wstring id, float timer, float length, bool deleteaftertime) : length(length), timer(timer), deleteaftertime(deleteaftertime), id(id)
	{

	}
	float timer;
	float length;
	bool deleteaftertime;
	std::wstring id;
	std::wstring getString() const
	{
		std::wstring tmp;
		tmp += id + L';';
		tmp += std::to_wstring(timer) + L';';
		tmp += std::to_wstring(length) + L';';
		tmp += std::to_wstring(deleteaftertime) + L';';
		return tmp;
	}
};

class FXengine;

class FX
{
public:
	FX(FXbase base): base(base)
	{
		
	}
	virtual ~FX() {}
	virtual bool sync(float time, sf::Color&color, sf::Vector2f&position, float&scale, const sf::RenderTexture&rendertexture, sf::Sprite rendersprite) = 0;


	bool syncTimer(float time)
	{
		base.timer += time;
		return base.timer < base.length||base.length==0;
	}
	std::wstring getId() const
	{
		return base.id;
	}
	std::wstring getString() const
	{
		std::wstring tmp;
		tmp += L"fx;";
		tmp += getFront();
		tmp += base.getString();
		tmp += getBack();
		return tmp;
	}
protected:
	friend class FXengine;
	virtual std::wstring getFront() const { return L""; };
	virtual std::wstring getBack() const { return L""; };
	FXbase base;
};

class FXScreenShake : public FX
{
public:
	FXScreenShake(FXbase base, float intensity):intensity(intensity), FX(base)
	{
	}

	virtual bool sync(float time, sf::Color&color, sf::Vector2f&position, float&scale, const sf::RenderTexture&rendertexture, sf::Sprite rendersprite) override
	{
		position += sf::Vector2f(rendertexture.getSize().x*(cos(base.timer * 50.0f))*intensity, rendertexture.getSize().y*(sin(base.timer * 50.0f))*intensity);
		return true;
	}
private:
	virtual std::wstring getFront() const override
	{
		return L"screenshake";
	}
	virtual std::wstring getBack() const override
	{
		std::wstring tmp;
		tmp += std::to_wstring(intensity) + L';';
		return tmp;
	};
	float intensity;
};

class FXCameraZoom : public FX
{
public:
	FXCameraZoom(FXbase base, float scale) :newscale(scale), FX(base)
	{
	}

	virtual bool sync(float time, sf::Color&color, sf::Vector2f&position, float&scale, const sf::RenderTexture&rendertexture, sf::Sprite rendersprite) override
	{
		float value = base.timer / base.length;
		if (value > 1)
			value = 1;
		if (base.length == 0)
			value = 1;
		scale *= (newscale*value + 1*  (1 - value));
		return true;
	}
private:
	virtual std::wstring getFront() const override
	{
		return L"camerazoom;";
	}
	virtual std::wstring getBack() const override
	{
		std::wstring tmp;
		tmp += std::to_wstring(newscale) + L';';
		return tmp;
	};
	float newscale; 
};

class FXCameraMove : public FX
{
public:
	FXCameraMove(FXbase base, float xpos, float ypos) :xpos(xpos), ypos(ypos), FX(base)
	{
		base.timer = 0;
	}

	virtual bool sync(float time, sf::Color&color, sf::Vector2f&position, float&scale, const sf::RenderTexture&rendertexture, sf::Sprite rendersprite) override
	{
		float value = base.timer / base.length;
		if (value > 1)
			value = 1;
		if (base.length == 0)
			value = 1;
	

		position.x += xpos*rendertexture.getSize().x*value;
		position.y += ypos*rendertexture.getSize().y*value;
		return true;
	}
private:
	virtual std::wstring getFront() const override
	{
		return L"cameramove;";
	}
	virtual std::wstring getBack() const override
	{
		std::wstring tmp;
		tmp += std::to_wstring(xpos) + L';';
		tmp += std::to_wstring(ypos) + L';';
		return tmp;
	};
	float xpos; 
	float ypos;
};

class FXColor : public FX
{
public:
	FXColor(FXbase base, int r1, int g1, int b1, int r2, int g2, int b2) :FX(base), firstcolor(r1, g1, b1), targetcolor(r2, g2, b2)
	{
	}

	virtual bool sync(float time, sf::Color&color, sf::Vector2f&position, float&scale, const sf::RenderTexture&rendertexture, sf::Sprite rendersprite) override
	{
		float value = base.timer / base.length;
		if (value > 1)
			value = 1;
		if (base.length == 0)
			value = 1;

		sf::Color tmp;
		tmp.r = sf::Uint8(firstcolor.r*(1 - value) + targetcolor.r*value);
		tmp.g = sf::Uint8(firstcolor.g*(1 - value) + targetcolor.g*value);
		tmp.b = sf::Uint8(firstcolor.b*(1 - value) + targetcolor.b*value);
		color *= tmp;

		return true;
	}
	virtual std::wstring getFront() const override
	{
		return L"color;";
	}
	virtual std::wstring getBack() const override
	{
		std::wstring tmp;
		tmp += std::to_wstring(targetcolor.r) + L';';
		tmp += std::to_wstring(targetcolor.g) + L';';
		tmp += std::to_wstring(targetcolor.b) + L';';
		return tmp;
	};
private:
	sf::Color firstcolor;
	sf::Color targetcolor;
};

class FXengine : public sf::Drawable
{
public:
	FXengine()
	{
		rendertexture.create((unsigned int)gamesize.x, (unsigned int)gamesize.y);
		rendertexture.setView(sf::View(sf::FloatRect(0, 0, gamesize.x, gamesize.y)));
		rendertexture.setSmooth(true);
	}

	~FXengine()
	{
		for (int i = fx.size() - 1; i >= 0; i--)
			delete fx[i];
	}

	void save(SaveData&data)
	{
		for (int i = fx.size() - 1; i >= 0; i--)
		{
			VisualNovelEvent tmp;
			tmp.loadFromString(fx[i]->getString());
			data.toLoad.push_back(tmp);
		}
	}

	void deleteByID(std::wstring id)
	{
		for (int i = fx.size() - 1; i >= 0; i--)
			if (fx[i]->getId() == id)
			{
				delete fx[i];
				fx.erase(fx.begin() + i);
			}
	}

	void interpretFX(VisualNovelEvent event)
	{
		std::wstring type = event.getArgument(0);
		if (type == L"delete")
		{
			deleteByID(event.getArgument(1));
		}
		else if (type == L"finish")
		{
			std::wstring id = event.getArgument(1);
			for (auto&tmp : fx)
				if(tmp->getId() == id)
					tmp->base.timer = tmp->base.length;
		}
		else if (type == L"screenshake")
			fx.push_back(new FXScreenShake(FXbase(event.getArgument(1), stof(event.getArgument(2)), stof(event.getArgument(3)), !!stoi(event.getArgument(4))), stof(event.getArgument(5))));
		else if (type == L"color")
			fx.push_back(new FXColor(FXbase(event.getArgument(1), stof(event.getArgument(2)), stof(event.getArgument(3)), !!stoi(event.getArgument(4))), stoi(event.getArgument(5)), stoi(event.getArgument(6)), stoi(event.getArgument(7)), stoi(event.getArgument(8)), stoi(event.getArgument(9)), stoi(event.getArgument(10))));
		else if (type == L"camerazoom")
			fx.push_back(new FXCameraZoom(FXbase(event.getArgument(1), stof(event.getArgument(2)), stof(event.getArgument(3)), !!stoi(event.getArgument(4))), stof(event.getArgument(5))));
		else if (type == L"cameramove")
			fx.push_back(new FXCameraMove(FXbase(event.getArgument(1), stof(event.getArgument(2)), stof(event.getArgument(3)), !!stoi(event.getArgument(4))), stof(event.getArgument(5)), stof(event.getArgument(6))));
	}

	inline void sync(float time)
	{
		sf::Color color = sf::Color(255, 255, 255);
		sf::Vector2f position;
		float scale = 1.0f;

		for (int i = fx.size() - 1; i >= 0; i--)
		{
			bool tmp1 = fx[i]->syncTimer(time);
			bool tmp2 = fx[i]->sync(time, color, position, scale, rendertexture, rendersprite);
			if (!tmp1&&fx[i]->base.deleteaftertime||!tmp2)
			{
				delete fx[i];
				fx.erase(fx.begin() + i);
			}
		}

		view = sf::View(sf::FloatRect((float)rendertexture.getSize().x*(1-scale)/2 + position.x*scale, (float)rendertexture.getSize().y*(1-scale)/2 + position.y*scale, (float)rendertexture.getSize().x*scale, (float)rendertexture.getSize().y*scale));

		rendersprite.setColor(color);
	}

	inline void begin()
	{
		rendertexture.setView(view);
		rendertexture.clear();
	}
	inline void add(const sf::Drawable&to_draw)
	{

		rendertexture.draw(to_draw);
	}
	inline void end()
	{
		rendertexture.display();
		rendersprite.setTexture(rendertexture.getTexture());
	}
	const sf::RenderTexture*getTexture() const
	{
		return &rendertexture;
	}
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override
	{
		target.draw(rendersprite, states);
	}
private:
	sf::View view;
	sf::RenderTexture rendertexture;
	sf::Sprite rendersprite;

	std::vector <FX*> fx;
};
