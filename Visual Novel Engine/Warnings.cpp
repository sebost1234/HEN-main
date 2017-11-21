#include "Warnings.h"

void throwWarning(std::string text)
{
	std::cout << text << std::endl;
}

void throwWarning(std::wstring text)
{
	std::wcout << text << std::endl;
}
