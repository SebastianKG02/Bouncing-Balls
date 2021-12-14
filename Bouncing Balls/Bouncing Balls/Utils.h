#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "Defines.h"
#include <cctype>
#include <clocale>
#include <SFML/Window/Keyboard.hpp>

/*
UTILS - A simple class to handle a few universal functions
V0.0.a-1 Added to project
- added loadConfigFile (method for loading files)
- added saveToConfigFile (method to save file)
*/
class Utils
{
public:
	std::map<std::string, std::string> static loadConfigFile(std::string file, std::string delim="=");
	bool static saveToConfigFile(std::string file, std::map<std::string, std::string>* data, std::string delim="=");
	std::string static boolToString(bool b);
	bool static stringToBool(std::string s);
	std::string static keyToStr(sf::Keyboard::Key key);
	sf::Keyboard::Key static charToKey(std::string keyt);
	static std::string* strToUpper(std::string* str);
	static std::string* strToLower(std::string* str);
};

/*
ConfigFile - A component of the Utils class, meant for storing simple data in a form mirroring a ".properties" file
Stores data as a std::map of string as the key and string as the value
V0.0.a-1 Added to project
- inital addition
*/
class ConfigFile
{
public:
	ConfigFile(std::string pathn,std::string delim = "=");
	ConfigFile();
	std::string getValue(std::string key);
	std::map<std::string, std::string>* getData();
	bool addValue(std::string key, std::string val);
	bool delValue(std::string key);
	bool setValue(std::string key, std::string val, bool doOverride=false);
	bool loadConfigFile(std::string file, std::string delim = "=");
private:
	std::string path;
	std::string delim = "=";
	std::map<std::string, std::string> data;
};

#endif 

