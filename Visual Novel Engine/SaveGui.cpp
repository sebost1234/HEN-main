#include "SaveGui.h"
#include "GameState.h"
#include "GeneralSettings.h"

void GuiSaveManager::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::GuiElementEvent::Pressed && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
	{
		{
			if (from == &deletebutton)
			{
				changePage(currentpage, true);
			}
			for (unsigned int i = 0; i < savesgui.size(); i++)
				if (from == &savesgui[i].button)
				{
					if(savesgui[i].button.getString()=="Save"|| savesgui[i].button.getString() == "Load")
						fatherstate->saveMangerEvent(i + (currentpage - 1)*rows*columns, false);
					else
						fatherstate->saveMangerEvent(i + (currentpage - 1)*rows*columns, true);
					SoundEngine::playSound("click");
					return;
				}
			for (unsigned int i = 0; i < pages.size(); i++)
				if (from == pages[i])
				{
					changePage(i + 1);
					SoundEngine::playSound("click");
					return;
				}
			return;
		}
	}
}


