#include "AssetManager.h"

//Check if initalisation is complete
bool AssetManager::isInitComplete() {
	return hasFinishedInit;
}

//Check if cleanup is complete
bool AssetManager::isCleanupComplete() {
	return hasFinishedCleanup;
}

//Method to get loaded texture
sf::Texture* AssetManager::getTexture(std::string textureName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (tex.at(textureName) != nullptr) {
				return tex.at(textureName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e){
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ALL
			std::cout << "[DEBUG]Failed to load texture <" << textureName << "> from map." << std::endl;
	#endif
#endif
		}
	}
	else {
		return nullptr;
	}
}

//Method to get loaded sound
sf::SoundBuffer* AssetManager::getSound(std::string soundName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (sound.at(soundName) != nullptr) {
				return sound.at(soundName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e) {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ALL
			std::cout << "[DEBUG]Failed to load sound <" << soundName << "> from map." << std::endl;
	#endif
#endif
		}
	}
	else {
		return nullptr;
	}
}

//Method to get loaded font
sf::Font* AssetManager::getFont(std::string fontName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (fonts.at(fontName) != nullptr) {
				return fonts.at(fontName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e) {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ALL
			std::cout << "[DEBUG]Failed to load font <" << fontName << "> from map." << std::endl;
	#endif
#endif
		}
	}
	else {
		return nullptr;
	}
}

//Method to get loaded Music
sf::Music* AssetManager::getMusic(std::string fontName) {
	//This method should only run if AM has completed init AND cleanup is not started
	if (isInitComplete() == true && isCleanupComplete() == false) {
		try {
			if (music.at(fontName) != nullptr) {
				return music.at(fontName);
			}
			else {
				return nullptr;
			}
		}
		catch (std::exception e) {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_ALL
			std::cout << "[DEBUG]Failed to load Music object <" << fontName << "> from map." << std::endl;
#endif
#endif
		}
	}
	else {
		return nullptr;
	}
}

//Initalise AssetManager, load all resources 
void AssetManager::init() {
	hasFinishedInit = false;
	hasFinishedCleanup = false;
	//Only load everything once
	//Set init to be complete after
	if (isInitComplete() != true) {
		//initalise all maps
		tex = std::map<std::string, sf::Texture*>();
		sound = std::map<std::string, sf::SoundBuffer*>();
		fonts = std::map<std::string, sf::Font*>();
		music = std::map<std::string, sf::Music*>();

		//Only pass forward if asset file is found
		if (mainConfig.loadConfigFile("asset.cfg") == true) {
			ConfigFile texcfg, soundcfg, fontcfg, musiccfg;
			
			//Check if resource manifest for textures has been found
			if (texcfg.loadConfigFile(mainConfig.getValue("tex")) == true) {
				//Loop through each entry in resource manifest
				for (auto& ref : *texcfg.getData()) {
					//Insert blank texture under reference key
					tex.insert({ ref.first, new sf::Texture()});
					//Attempt to load texture
					if (tex[ref.first]->loadFromFile(ref.second) == true) {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_MINERR
						//Diagnostic message
						std::cout << "[AM]Loaded <" << ref.first << ">@" << ref.second << std::endl;
	#endif
#endif
					}
					else {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ERRO
						//Debug only! Print error message
						std::cout << "[AM]Could not load <" << ref.first << ">@" << ref.second << std::endl;
	#endif
#endif
					}
				}
			}
			else {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ERRO
				//If resource manifest not found, show debug message and do nothing else (also do not set initComplete flag)
				std::cout << "[DEBUG]Fatal error! Could not load resource manifest @" << mainConfig.getValue("tex") << std::endl;
	#endif
#endif
			}

			//Check if resource manifest for sounds has been found
			if (soundcfg.loadConfigFile(mainConfig.getValue("sound")) == true) {
				//Loop through each entry in resource manifest
				for (auto& ref : *soundcfg.getData()) {
					//Insert blank soundbuffer under reference key
					sound.insert({ ref.first, new sf::SoundBuffer() });
					//Attempt to load soundBuffer
					if (sound[ref.first]->loadFromFile(ref.second) == true) {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
						//Diagnostic message
						std::cout << "[AM]Loaded <" << ref.first << ">@" << ref.second << std::endl;
#endif
#endif
					}
					else {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_ERRO
						//Debug only! Print error message
						std::cout << "[AM]Could not load <" << ref.first << ">@" << ref.second << std::endl;
#endif
#endif
					}
				}
			}
			else {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_ERRO
				//If resource manifest not found, show debug message and do nothing else (also do not set initComplete flag)
				std::cout << "[DEBUG]Fatal error! Could not load resource manifest @" << mainConfig.getValue("sound") << std::endl;
#endif
#endif
			}

			//Check if resource manifest for fonts has been found
			if (fontcfg.loadConfigFile(mainConfig.getValue("fonts")) == true) {
				//Loop through each entry in resource manifest
				for (auto& ref : *fontcfg.getData()) {
					//Insert blank font under reference key
					fonts.insert({ ref.first, new sf::Font() });
					//Attempt to load font
					if (fonts[ref.first]->loadFromFile(ref.second) == true) {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
						//Diagnostic message
						std::cout << "[AM]Loaded <" << ref.first << ">@" << ref.second << std::endl;
#endif
#endif
					}
					else {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_ERRO
						//Debug only! Print error message
						std::cout << "[AM]Could not load <" << ref.first << ">@" << ref.second << std::endl;
#endif
#endif
					}
				}
			}
			else {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_ERRO
				//If resource manifest not found, show debug message and do nothing else (also do not set initComplete flag)
				std::cout << "[DEBUG]Fatal error! Could not load resource manifest @" << mainConfig.getValue("fonts") << std::endl;
#endif
#endif
			}

			//Check if resource manifest for music has been found
			if (musiccfg.loadConfigFile(mainConfig.getValue("music")) == true) {
				//Loop through each entry in resource manifest
				for (auto& ref : *musiccfg.getData()) {
					//Insert blank music object under reference key
					music.insert({ ref.first, new sf::Music() });
					//Attempt to load music object 
					//The method "openFromFile" will load the file into memory & into the map, but will not play it
					if (music[ref.first]->openFromFile(ref.second) == true) {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
						//Diagnostic message
						std::cout << "[AM]Loaded <" << ref.first << ">@" << ref.second << std::endl;
#endif
#endif
					}
					else {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_ERRO
						//Debug only! Print error message
						std::cout << "[AM]Could not load <" << ref.first << ">@" << ref.second << std::endl;
#endif
#endif
					}
				}
			}
			else {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_ERRO
				//If resource manifest not found, show debug message and do nothing else (also do not set initComplete flag)
				std::cout << "[DEBUG]Fatal error! Could not load resource manifest @" << mainConfig.getValue("music") << std::endl;
#endif
#endif
			}

			hasFinishedInit = true;
		}
		else {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ERRO
			std::cout << "[DEBUG]Fatal error! Could not load asset.cfg" << std::endl;
	#endif
#endif
		}
	}
}

//Simple cleanup method
void AssetManager::cleanup() {
	//Only run this method if init is complete and cleanup has not ran
	if (isInitComplete() == true && isCleanupComplete() == false) {
		//Loop through texture map and delete all loaded textures
		for (auto& res : tex) {
			delete res.second;
		}

		//Loop through music map and delete all loaded music
		for (auto& res : music) {
			delete res.second;
		}

		//Loop through font map and delete all loaded fonts
		for (auto& res : fonts) {
			delete res.second;
		}

		//Loop through sound map and delete all loaded sounds
		for (auto& res : sound) {
			delete res.second;
		}

		//Set cleanup flag
		hasFinishedCleanup == true;
	}
}