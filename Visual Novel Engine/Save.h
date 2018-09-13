#pragma once

#include <string>
#include <vector>

#include "VisualNovelEvent.h"

struct SaveData
{
	SaveData()
	{
		slot = -1;
	}
	unsigned int slot;
	std::wstring desc;
	std::wstring date;
	std::wstring file;
	int currentline;

	std::vector <VisualNovelEvent> toLoad;
};

std::wostream& operator<<(std::wostream& str, SaveData&save);

std::wistream& operator>>(std::wistream& str, SaveData&save);