#include "SaveGui.h"
#include "GameState.h"
#include "GeneralSettings.h"

void GuiSaveManager::notifyEvent(GuiNS::GuiElementEvent event, GuiNS::GuiElement * from)
{
	if (event.type == GuiNS::GuiElementEvent::Mouse && event.mouse.type == GuiNS::GuiElementEvent::Type::Released)
	{
		{
			if (from == &deletebutton)
			{
				if (deletebutton.isFrozen())
				{
					delet = false;
					copyslot = -2;
					changePage(currentpage);
					deletebutton.changeState(GuiNS::GuiElement::nothing);
					deletebutton.setFrozen(false);
					description.setString(defaultdescription);
					copyslot = -2;
				}
				else
				{
					delet = true;
					copyslot = -2;
					changePage(currentpage);
					deletebutton.changeState(GuiNS::GuiElement::hover);
					deletebutton.setFrozen(true);
					description.setString("Select a save to delete.");
				}
				SoundEngine::playSound("click");
				return;
			}
			else if (from == &copybutton)
			{
				if (copyslot == -2)//No click
				{
					copyslot = -1;
					changePage(currentpage);
					description.setString("Select a save to copy.");
				}
				else//Turn off
				{
					copyslot = -2;
					description.setString(defaultdescription);
					changePage(currentpage);
				}

				SoundEngine::playSound("click");
				return;
			}
			for (unsigned int i = 0; i < savesgui.size(); i++)
				if (from == &savesgui[i].button)
				{
					if (copyslot == -1)
					{
						copyslot = i + (currentpage - 1)*rows*columns;
						changePage(currentpage);
					}
					else if (copyslot >= 0)
					{
						fatherstate->saveMangerCopyEvent(copyslot,i + (currentpage - 1)*rows*columns);
						copyslot = -2;
						changePage(currentpage);
					}
					else if(savesgui[i].button.getString() == "Save"|| savesgui[i].button.getString() == "Load")
						fatherstate->saveMangerEvent(i + (currentpage - 1)*rows*columns, false);
					else if(savesgui[i].button.getString() == "Delete")
					{
						fatherstate->saveMangerEvent(i + (currentpage - 1)*rows*columns, true);
						deletebutton.setFrozen(false);
						deletebutton.changeState(GuiNS::GuiElement::nothing);
						description.setString(defaultdescription);
						delet = false;
					}
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


