#pragma once

#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <map>
#include <string>
#include <iostream>
#include "Defines.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/*
V0.0a-35 Added to Project
AssetManager class, responsible for loading resources and distributing them to game objects
*/
class AssetManager
{
public:
	static void init();
	static void cleanup();
	static sf::Texture* getTexture(std::string textureName);
	static sf::SoundBuffer* getSound(std::string soundName);
	static sf::Font* getFont(std::string fontName);
	static sf::Music* getMusic(std::string musicName);
	static bool isInitComplete();
	static bool isCleanupComplete();
private:
	static std::map<std::string, sf::Texture*> tex;
	static std::map<std::string, sf::Font*> fonts;
	static std::map<std::string, sf::SoundBuffer*> sound;
	static std::map<std::string, sf::Music*> music;
	static ConfigFile mainConfig;
	static bool hasFinishedInit;
	static bool hasFinishedCleanup;
};

#endif

