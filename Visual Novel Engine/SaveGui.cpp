#include "SaveGui.h"
#include "GameState.h"
#include "GeneralSettings.h"

void GuiSaveManager::notifyEvent(Gui::MyEvent event, Gui::GuiElement * from)
{
	if (event.type == Gui::MyEvent::Pressed && event.mouse.type == Gui::MyEvent::Type::Released)
	{
		if (from == &prevpagebutton)
			changePage(currentpage - 1);
		else if (from == &nextpagebutton)
			changePage(currentpage + 1);
		else
		{
			for (unsigned int i = 0; i < savesgui.size(); i++)
				if (from == &savesgui[i].desc)
				{
					fatherstate->saveMangerEvent(i + (currentpage - 1)*rows*columns);
					
					SoundEngine::playSound("click");
					return;
				}
			return;
		}
		SoundEngine::playSound("click");
	}
}
