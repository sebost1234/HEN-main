#include "SaveGui.h"
#include "GameState.h"
#include "GeneralSettings.h"

void GuiSaveManager::notifyEvent(GuiNS::MyEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::MyEvent::Pressed && event.mouse.type == GuiNS::MyEvent::Type::Released)
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
