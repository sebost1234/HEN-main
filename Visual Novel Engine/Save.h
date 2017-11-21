#pragma once

#include <string>
#include <vector>

struct SaveData
{
	unsigned int slot;
	std::wstring desc;
	std::wstring date;
	std::wstring file;
	int currentline;

	std::vector <std::wstring> toLoad;
};

std::wostream& operator<<(std::wostream& str, SaveData&save);

std::wistream& operator>>(std::wistream& str, SaveData&save);