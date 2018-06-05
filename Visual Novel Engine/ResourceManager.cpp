#include "ResourceManager.h"



sf::Font* ResourceManager::font = nullptr;
std::map<StyleTypes, GuiNS::Style*> ResourceManager::styles;
std::map<std::string, sf::Texture*> ResourceManager::textures;
