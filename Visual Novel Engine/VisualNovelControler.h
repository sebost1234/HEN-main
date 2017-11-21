#pragma once

#include <fstream>
#include <string>

#include <iostream>
#include <vector>

#include "Save.h"
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

struct VisualNovelEvent
{
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



	enum Type
	{
		STARTOFGAME,
		None,
		//Game
		Wait,
		Stop,
		Say,
		Choice,
		Flag,

		SetScene,

		ENDOFGAME,
		//Scene manipulation
		STARTOFSCENE,

		MoveIntoRow,
		RemoveFromRow,

		BgChange,

		AddModel,
		TextureModel,
		DeleteModel,

		SetModelPosition,
		SetModelTargetPosition,
		SetModelFreeEvent,


		ENDOFSCENE,
		//Sound and fx
		STARTOFFX,

		PlaySound,
		PlayMusic,

		FX,

		ENDOFFX,
		STARTOFCONTROL,
		//Control
		ChangeFile,
		GoTo,
		Scope,
		FlagTest,

		ENDOFCONTROL,
	};

	VisualNovelEvent(VisualNovelEvent::Type type = VisualNovelEvent::None) : typ(type) {}

	void loadFromString(std::wstring line)
	{
		
		typ = VisualNovelEvent::None;

		size_t tmp=0;
		std::wstring type = nextArgument(line, tmp);

		for(auto&n:type)
			n = towlower(n);

		if (type != L"//")
			if (type == L"stop")
			{
				typ = VisualNovelEvent::Stop;
			}
			else if (type == L"wait")
			{
				typ = VisualNovelEvent::Wait;
				loadArguments(line, tmp);
			}
			else if (type == L"changebg")
			{
				typ = VisualNovelEvent::BgChange;
				loadArguments(line, tmp);
			}
			else if (type == L"say")
			{
				typ = VisualNovelEvent::Say;
				loadArguments(line, tmp);
			}
			else if (type == L"addmodel")
			{
				typ = VisualNovelEvent::AddModel;
				loadArguments(line, tmp);
			}
			else if (type == L"texturemodel")
			{
				typ = VisualNovelEvent::TextureModel;
				loadArguments(line, tmp);
			}
			else if (type == L"deletemodel")
			{
				typ = VisualNovelEvent::DeleteModel;
				loadArguments(line, tmp);
			}
			else if (type == L"changefile")
			{
				typ = VisualNovelEvent::ChangeFile;
				loadArguments(line, tmp);
			}
			else if (type == L"scope")
			{
				typ = VisualNovelEvent::Scope;
				loadArguments(line, tmp);
			}
			else if (type == L"goto")
			{
				typ = VisualNovelEvent::GoTo;
				loadArguments(line, tmp);
			}
			else if (type == L"choice")
			{
				typ = VisualNovelEvent::Choice;

				arguments.push_back(nextArgument(line, tmp));

				int nrofchoices = stoi(arguments.back());

				loadArguments(line, tmp);
			}
			else if (type == L"playsound")
			{
				typ = VisualNovelEvent::PlaySound;
				loadArguments(line, tmp);
			}
			else if (type == L"playmusic")
			{
				typ = VisualNovelEvent::PlayMusic;
				loadArguments(line, tmp);
			}
			else if (type == L"fx")
			{
				typ = VisualNovelEvent::FX;
				loadArguments(line, tmp);
			}			
			else if (type == L"flag")
			{
				typ = VisualNovelEvent::Flag;
				loadArguments(line, tmp);
			}
			else if (type == L"flagtest")
			{
				typ = VisualNovelEvent::FlagTest;
				loadArguments(line, tmp);
			}
			else if (type == L"setscene")
			{
				typ = VisualNovelEvent::SetScene;
				loadArguments(line, tmp);
			}
			else if (type == L"moveintorow")
			{
				typ = VisualNovelEvent::MoveIntoRow;
				loadArguments(line, tmp);
			}
			else if (type == L"removefromrow")
			{
				typ = VisualNovelEvent::RemoveFromRow;
				loadArguments(line, tmp);
			}
			else if (type == L"setmodelposition")
			{
				typ = VisualNovelEvent::SetModelPosition;
				loadArguments(line, tmp);
			}
			else if (type == L"setmodeltargetposition")
			{
				typ = VisualNovelEvent::SetModelTargetPosition;
				loadArguments(line, tmp);
			}
			else if (type == L"setmodelfree")
			{
				typ = VisualNovelEvent::SetModelFreeEvent;
				loadArguments(line, tmp);
			}
	}

	static std::wstring typeToString(Type typ);

	std::wstring toString() const
	{
		std::wstring string;
		string += typeToString(typ) + L';';
		for (unsigned int i = 0; i < arguments.size(); i++)
			string += arguments[i] + L';';
		return string;
	}

	Type getType() const
	{
		return typ;
	}

	bool isArgument(unsigned int i)
	{
		return i >= 0u && i < arguments.size();
	}
	std::wstring getArgument(unsigned int i)
	{
		if(i>=0u&&i<arguments.size())
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
		if (i >= 0u && i<arguments.size())
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
private:
	std::wstring nextArgument(std::wstring string, size_t&pos)
	{
		size_t find = string.find(';', pos);
		std::wstring to_return;
		if (find == std::wstring::npos)
		{
			to_return = string.substr(pos, string.size() - pos);
			pos = std::wstring::npos;
		}
		else
		{
			to_return = string.substr(pos, find - pos);
			pos = find + 1;
		}
		return to_return;
	}
	void loadArguments(std::wstring string, size_t pos)
	{
		while (pos != std::wstring::npos)
			arguments.push_back(nextArgument(string, pos));
	}

	std::vector <std::wstring> arguments;
	Type typ;
};

class VisualNovelControler
{
public:
	VisualNovelControler()
	{
		loadFile(L"start.txt");
		nextevent = 0;
		lasttext = 0;
	}
	~VisualNovelControler()
	{
		clearEvents();
	}

	void clearEvents()
	{
		events.clear();
	}

	void loadFile(std::wstring path)
	{
		nextevent = 0;
		currentfile = path;
		clearEvents();
		std::wfstream file;
		file.open(L"Data\\Game\\" + path);

		if(!file.is_open())
			std::throw_with_nested(std::runtime_error("ERROR 01: Failed to load game file: Data\\Game\\" + std::string(path.begin(), path.end())));

		while (!file.eof())
		{
			auto&tmp = loadNext(file);
			if(tmp.getType() != VisualNovelEvent::None)
				events.push_back(tmp);
		}
		
		//for (unsigned int i = 0; i < events.size(); i++)
		//	std::wcout << events[i].toString() << std::endl;

		file.close();
	}
	void loadSave(SaveData data)
	{
		loadFile(data.file);
		nextevent = data.currentline;

		for (unsigned int i = 0; i < data.toLoad.size(); i++)
		{
			VisualNovelEvent tmp;
			tmp.loadFromString(data.toLoad[i]);
			loadevents.push_back(tmp);
		}
	}
	SaveData createSaveBase()
	{
		SaveData data;
		data.file = currentfile;
		data.desc = L"Save";
		data.currentline = lasttext;

		for (unsigned int i = 0; i < flags.size(); i++)
			data.toLoad.push_back(L"flag;" + flags[i]);

		return data;
	}

	//returns false if scope not found
	bool gotoScope(std::wstring id)
	{
		for (unsigned int i = 0; i < events.size(); i++)
			if (events[i].getType() == VisualNovelEvent::Scope)
				if (events[i].getArgument(ScopeEventId) == id)
				{
					nextevent = i;
					return true;
				}
		return false;
	}

	int getNextEvent()
	{
		return nextevent;
	}
	std::wstring getCurrentFile()
	{
		return currentfile;
	}

	VisualNovelEvent next()
	{
		if (loadevents.empty())
		{
			auto tmp = events[nextevent++];
			if (tmp.getType() > VisualNovelEvent::STARTOFCONTROL&&tmp.getType() < VisualNovelEvent::ENDOFCONTROL)
			{
				switch (tmp.getType())
				{
				case VisualNovelEvent::ChangeFile:
					loadFile(tmp.getArgument(ChangeFileEventpath));
					break;
				case VisualNovelEvent::GoTo:
					gotoScope(tmp.getArgument(GoToEventScope));
					break;
				case VisualNovelEvent::FlagTest:
					if (std::find(flags.begin(), flags.end(), tmp.getArgument(0)) != flags.end())
						gotoScope(tmp.getArgument(FlagDataSuccessScope));
					else
						gotoScope(tmp.getArgument(FlagDataFailureScope));
					break;
				default:
					break;
				}
				return next();
			}
			else
			{
				if (tmp.getType() == VisualNovelEvent::Say)
					lasttext = nextevent-1;
				flagCheck(tmp);
				return tmp;
			}
		}
		else
		{
			auto tmp = loadevents.front();
			loadevents.erase(loadevents.begin());
			flagCheck(tmp);
			return tmp;
		}
	}

	void choose(int nr)
	{
		if (events[nextevent-1].getType() == VisualNovelEvent::Choice&&nr>=0&&nr<events[nextevent-1].getArgumentAsInt(ChoiceEventnr))
		{
			int nrofchoices = events[nextevent - 1].getArgumentAsInt(ChoiceEventnr);
			gotoScope(events[nextevent-1].getArgument(ChoiceEventstart+nrofchoices + nr));
		}
	}
private:
	void flagCheck(VisualNovelEvent&tmp)
	{
		if(tmp.getType() == VisualNovelEvent::Flag)
			flags.push_back(tmp.getArgument(0));
	}
	VisualNovelEvent loadNext(std::wfstream&file)
	{
		VisualNovelEvent to_return;
		std::wstring line = getLine(file);
		if (line.size() > 3)
			if (line.substr(0, 3) == L"\xEF\xBB\xBF")
				line = line.substr(3, line.size() - 3);

		to_return.loadFromString(line);


		return to_return;
	}
	std::wstring getLine(std::wfstream&file)
	{
		std::wstring tmp;
		std::getline(file, tmp, L'\n');
		return tmp;
	}

	int lasttext;
	int nextevent;
	std::wstring currentfile;
	std::vector<VisualNovelEvent> events;

	std::vector<VisualNovelEvent> loadevents;

	std::vector<std::wstring> flags;
};
