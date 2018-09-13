#pragma once

#include <fstream>
#include <string>
#include <map>
#include <algorithm>

#include <iostream>
#include <vector>

#include "Warnings.h"

#define SayEventname 0
#define SayEventstring 1
#define SayEventtype 2

#define SetSceneEventtype 0

#define MoveIntoRowEventmodelid 0
#define MoveIntoRowEventrowid 1
#define MoveIntoRowEventpos 2
#define MoveIntoRowEventteleport 3

#define RemoveFromRowEventmodelid 0

#define BgchangeEventpath 0
#define CGEventpath 0

#define ModelEventid 0

#define TextureModelEventid 0
#define TextureModelEventpath 1

#define SetModelPositionEventid 0
#define SetModelPositionEventx 1
#define SetModelPositionEventy 2

#define SetModelFreeEventid 0
#define SetModelFreeEventbool 1

#define WaitEventtime 0

#define ChangeFileEventpath 0
#define GoToEventScope 0

#define ScopeEventId 0

#define ChoiceEventnr 0
#define ChoiceEventstart 1

#define SoundEventpath 0

#define MusicEventpath 0
#define MusicEventloop 1

#define FlagData 0
#define FlagDataSuccessScope 1
#define FlagDataFailureScope 2

class Comparewstring
{
public:
	bool operator() (const std::wstring& left, const std::wstring& right) const
	{
		unsigned int size = std::min(left.size(), right.size());
		for (unsigned int i = 0u; i < size; i++)
			if (left[i] < right[i])
				return true;
			else if (left[i] > right[i])
				return false;
		return left.size()<right.size();
	}
};

class VisualNovelEvent
{
public:
	//Say event 1s-string 2s-name
	//BgChangeEvent event 1s-path
	//ModelEvent event 1s-id
	//TextureModelEvent 1s-id 2s-path
	//WaitEvent 1f-time
	//ChangeFileEvent 1s-path
	//ScopeEvent 1s-id
	//ChoiceEvent 1i - n 1s*n - names 1s*n - scopeid
	//SoundEvent 1s - path
	//MusicEvent 1s - path 1i - loop



	enum Type : int
	{
		STARTOFGAME,
		None,
		//Game
		Wait,
		Stop,
		Say,
		Choice,
		Flag,
		GlobalFlag,

		ENDOFGAME,
		//Scene manipulation
		STARTOFSCENE,

		MoveIntoRow,
		RemoveFromRow,

		BgChange,
		CG,

		AddModel,
		TextureModel,
		DeleteModel,

		SetModelPosition,
		SetModelTargetPosition,
		SetModelFreeEvent,


		ENDOFSCENE,
		//Sound and fx
		STARTOFSOUND,

		PlaySound,
		PlayMusic,

		ENDOFSOUND,
		STARTOFCONTROL,
		//Control
		ChangeFile,
		GoTo,
		Scope,
		FlagTest,
		GlobalFlagTest,

		ENDOFCONTROL,
	};

	VisualNovelEvent(VisualNovelEvent::Type type = VisualNovelEvent::None) : typ(type) {}

	VisualNovelEvent(VisualNovelEvent::Type type, std::initializer_list<std::wstring> list) : typ(type) { arguments = list; }

	void loadFromFile(std::wistream&file)
	{
		loadArguments(file);

		if (arguments.size() >= 1)
		{
			std::wstring type = arguments.front();

			arguments.erase(arguments.begin());

			for (int i = type.size() - 1; i >= 0; i--)
			{
				if (type[i] == L'\t' || type[i] == L' ')
					type.erase(type.begin() + i);
				else
					type[i] = towlower(type[i]);
			}

			typ = stringToType(type);
		}
	}

	void loadFromString(std::wstring line)
	{
		loadArguments(line);

		if (arguments.size() >= 1)
		{
			std::wstring type = arguments.front();

			arguments.erase(arguments.begin());

			for (int i = type.size() - 1; i >= 0; i--)
			{
				if (type[i] == L'\t' || type[i] == L' ')
					type.erase(type.begin() + i);
				else
					type[i] = towlower(type[i]);
			}

			typ = stringToType(type);
		}
	}

	static std::wstring typeToString(Type typ);
	static Type stringToType(std::wstring name);


	std::wstring toString() const
	{
		std::wstring string;
		string += typeToString(typ) + L";";
		for (unsigned int i = 0; i < arguments.size(); i++)
		{
			string += L"\"" + arguments[i] + L"\";";
		}

		return string;
	}

	Type getType() const
	{
		return typ;
	}

	void addArgument(std::wstring string)
	{
		arguments.push_back(string);
	}

	bool isArgument(unsigned int i)
	{
		return i >= 0u && i < arguments.size();
	}
	std::wstring getArgument(unsigned int i)
	{
		if (i >= 0u && i < arguments.size())
			return arguments[i];
		else
		{
			throwWarning(L"At line: " + toString());
			throwWarning(L"Trying to access argument nr " + std::to_wstring(i) + L" out of " + std::to_wstring(arguments.size()) + L" arguments.");
			return L"0";
		}
	}
	std::string getArgumentS(unsigned int i)
	{
		if (i >= 0u && i < arguments.size())
			return std::string(arguments[i].begin(), arguments[i].end());
		else
		{
			throwWarning(L"At line: " + toString());
			throwWarning(L"Trying to access argument nr " + std::to_wstring(i) + L" out of " + std::to_wstring(arguments.size()) + L" arguments.");
			return "0";
		}
	}
	int getArgumentAsInt(int i)
	{
		return stoi(getArgument(i));
	}
	float getArgumentAsFloat(int i)
	{
		return stof(getArgument(i));
	}

	static void loadEvents();
private:
	std::wstring getLine(std::wistream&file)
	{
		std::wstring line;
		std::getline(file, line, L'\n');

		if (line.size() > 3)
			if (line.substr(0, 3) == L"\xEF\xBB\xBF")
				line = line.substr(3, line.size() - 3);

		return line;
	}

	void loadArguments(std::wistream&file)
	{
		std::wstring inbracket = L"";
		bool bracketstarted = false;
		int last = 0;

		std::wstring line = getLine(file);

		for (unsigned int i = 0; i < line.size(); i++)
		{
			if (line[i] == '"')
			{
				if (bracketstarted)
				{
					inbracket = line.substr(last + 1, i - last - 1);
					if (i == line.size() - 1)
						arguments.push_back(inbracket);
					bracketstarted = false;
				}
				else bracketstarted = true;
			}
			else if (line[i] == ';' && !bracketstarted)
			{
				if (inbracket != L"")
					arguments.push_back(inbracket);
				else
					arguments.push_back(line.substr(last, i - last));

				inbracket = L"";
				last = i + 1;
			}
			else if (i == line.size() - 1)
			{
				if (!bracketstarted)
					arguments.push_back(line.substr(last, line.size() - last));
				else
				{
					line += L'\n' + getLine(file);
				}
			}
		}
	}

	void loadArguments(std::wstring line)
	{
		std::wstring inbracket = L"";
		bool bracketstarted = false;
		int last = 0;

		for (unsigned int i = 0; i < line.size(); i++)
		{
			if (line[i] == '"')
			{
				if (bracketstarted)
				{
					inbracket = line.substr(last + 1, i - last - 1);
					if (i == line.size() - 1)
						arguments.push_back(inbracket);
					bracketstarted = false;
				}
				else bracketstarted = true;
			}
			else if (line[i] == ';' && !bracketstarted)
			{
				if (inbracket != L"")
					arguments.push_back(inbracket);
				else
					arguments.push_back(line.substr(last, i - last));

				inbracket = L"";
				last = i + 1;
			}
			else if (i == line.size() - 1)
			{
				arguments.push_back(line.substr(last, line.size() - last));
			}
		}
	}

	std::vector <std::wstring> arguments;
	Type typ;


	static void addEvent(VisualNovelEvent::Type typ, std::wstring string);
	static std::map < std::wstring, Type, Comparewstring> stringtotypemap;
	static std::map < Type, std::wstring> typetostringmap;
};