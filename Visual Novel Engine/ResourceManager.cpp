#include "ResourceManager.h"



sf::Font* ResourceManager::font = nullptr;
std::map<StyleTypes, GuiNS::Style*> ResourceManager::styles;
std::map<std::string, sf::Texture*> ResourceManager::textures;
std::map<int, sf::Texture*> ResourceManager::savetextures;
std::map<std::string, std::pair<sf::Texture*, sf::Texture*>> ResourceManager::charactertextures;