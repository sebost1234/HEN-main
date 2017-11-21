#include "ResourceManager.h"



sf::Font* ResourceManager::font = nullptr;
std::map<StyleTypes, Gui::Style*> ResourceManager::styles;
std::map<std::string, sf::Texture*> ResourceManager::textures;
