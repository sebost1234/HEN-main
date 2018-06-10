#include "Save.h"

#include <fstream>

std::wostream& operator<<(std::wostream& str, SaveData&save)
{
	str << save.slot << '\n';
	str << save.desc << '\n';
	str << save.date << '\n';
	str << save.file << '\n';
	str << save.currentline << '\n';

	str << save.toLoad.size() << '\n';
	for (unsigned int i = 0; i < save.toLoad.size(); i++)
		str << save.toLoad[i] << '\n';
	return str;
}

std::wistream& operator >> (std::wistream&is, SaveData&save)
{
	std::wstring tmp;

	std::getline(is, tmp, L'\n');
	save.slot = std::stoi(tmp);
	std::getline(is, save.desc, L'\n');
	std::getline(is, save.date, L'\n');
	std::getline(is, save.file, L'\n');
	std::getline(is, tmp, L'\n');
	save.currentline = std::stoi(tmp);
	
	std::getline(is, tmp, L'\n');
	int nr = stoi(tmp);
	for (int i = 0; i < nr; i++)
	{
		std::getline(is, tmp, L'\n');
		save.toLoad.push_back(tmp);
	}

	return is;
}
