#include "Journal.h"
#include "Options.h"

Journal::Journal(Options * options) : OptionsSubType(options)
{
	options->background.changeRectangle()->setTexture(ResourceManager::getTexture("Data//bgJournal.png"));
}
