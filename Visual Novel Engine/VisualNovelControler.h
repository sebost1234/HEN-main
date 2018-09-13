#pragma once

#include <fstream>
#include <string>
#include <map>
#include <algorithm>

#include <iostream>
#include <vector>

#include "Save.h"
#include "Warnings.h"
#include "VisualNovelEvent.h"

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
		
		/*
		for (unsigned int i = 0; i < events.size(); i++)
			std::wcout << events[i].toString() << std::endl;
		*/

		file.close();
	}
	void loadSave(SaveData data)
	{
		loadFile(data.file);
		nextevent = data.currentline;

		loadevents = data.toLoad;
	}

	SaveData createSaveBase()
	{
		SaveData data;
		data.file = currentfile;
		data.desc = L"Save";
		data.currentline = lasttext;

		for (unsigned int i = 0; i < flags.size(); i++)
		{
			VisualNovelEvent tmp(VisualNovelEvent::Flag);
			tmp.addArgument(flags[i]);
			data.toLoad.push_back(tmp);
		}

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
				case VisualNovelEvent::GlobalFlagTest:
				{
					std::vector<std::wstring> globalflags = getGlobalFlags();

					if (std::find(globalflags.begin(), globalflags.end(), tmp.getArgument(0)) != globalflags.end())
						gotoScope(tmp.getArgument(FlagDataSuccessScope));
					else
						gotoScope(tmp.getArgument(FlagDataFailureScope));
					break;
				}
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

	static std::vector<std::wstring> getGlobalFlags()
	{
		std::vector<std::wstring> globalflags;

		std::wfstream file;

		file.open("Data\\Save\\globalflags.czpal");
		if (file.is_open())
			while (!file.eof())
			{
				std::wstring path;
				std::getline(file, path, L'\n');
				globalflags.push_back(path);
			}

		file.close();

		return globalflags;
	}
private:
	void flagCheck(VisualNovelEvent&tmp)
	{
		if(tmp.getType() == VisualNovelEvent::Flag)
			flags.push_back(tmp.getArgument(0));
		else if (tmp.getType() == VisualNovelEvent::GlobalFlag)
		{
			std::vector<std::wstring> globalflags = getGlobalFlags();

			bool found = false;
			for (unsigned int i = 0; i < globalflags.size(); i++)
				if (globalflags[i] == tmp.getArgument(0))
				{
					found = true;
					break;
				}

			if (!found)
			{
				globalflags.push_back(tmp.getArgument(0));
				std::wfstream file;

				std::locale  defaultLocale("");
				file.imbue(defaultLocale);

				file.open("Data\\Save\\globalflags.czpal", std::ios::out | std::ios::trunc);
				for (unsigned int i = 0; i < globalflags.size(); i++)
					file << globalflags[i] << std::endl;

				file.close();
			}
		}
	}
	VisualNovelEvent loadNext(std::wfstream&file)
	{
		VisualNovelEvent to_return;

		to_return.loadFromFile(file);

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
