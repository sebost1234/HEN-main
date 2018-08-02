#include "Engine.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


#include <windows.h>


//#include <vld.h>

void print_exception(const std::exception& e, int level = 0)
{
	std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
	try 
	{
		std::rethrow_if_nested(e);
	}
	catch (const std::exception& e) {
		print_exception(e, level + 1);
	}
	catch (...) {}
}


#ifdef _DEBUG
int main()
#else
int __stdcall wWinMain(HINSTANCE hInst, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
#endif
{
	sf::RenderWindow window;
	Engine engine(&window);

	VisualNovelEvent::loadEvents();
	SoundEngine::clear_audio();

	try
	{
		engine.start();
	}
	catch (const std::exception& e)
	{
		print_exception(e);
	}

	ResourceManager::clear();
	SoundEngine::clear_audio();
}