#include "Utils.h"

std::string Utils::keyToChar(sf::Keyboard::Key key) {
	switch (key) {
	case sf::Keyboard::Key::A:
		return "A";
	case sf::Keyboard::Key::B:
		return "B";
	case sf::Keyboard::Key::C:
		return "C";
	case sf::Keyboard::Key::D:
		return "D";
	case sf::Keyboard::Key::E:
		return "E";
	case sf::Keyboard::Key::F:
		return "F";
	case sf::Keyboard::Key::G:
		return "G";
	case sf::Keyboard::Key::H:
		return "H";
	case sf::Keyboard::Key::I:
		return "I";
	case sf::Keyboard::Key::J:
		return "J";
	case sf::Keyboard::Key::K:
		return "K";
	case sf::Keyboard::Key::L:
		return "L";
	case sf::Keyboard::Key::M:
		return "M";
	case sf::Keyboard::Key::N:
		return "N";
	case sf::Keyboard::Key::O:
		return "O";
	case sf::Keyboard::Key::P:
		return "P";
	case sf::Keyboard::Key::Q:
		return "Q";
	case sf::Keyboard::Key::R:
		return "R";
	case sf::Keyboard::Key::S:
		return "S";
	case sf::Keyboard::Key::T:
		return "T";
	case sf::Keyboard::Key::U:
		return "U";
	case sf::Keyboard::Key::V:
		return "V";
	case sf::Keyboard::Key::W:
		return "W";
	case sf::Keyboard::Key::X:
		return "X";
	case sf::Keyboard::Key::Y:
		return "Y";
	case sf::Keyboard::Key::Z:
		return "B";
	case sf::Keyboard::Key::Num0:
		return "0";
	case sf::Keyboard::Key::Num1:
		return "1";
	case sf::Keyboard::Key::Num2:
		return "2";
	case sf::Keyboard::Key::Num3:
		return "3";
	case sf::Keyboard::Key::Num4:
		return "4";
	case sf::Keyboard::Key::Num5:
		return "5";
	case sf::Keyboard::Key::Num6:
		return "6";
	case sf::Keyboard::Key::Num7:
		return "7";
	case sf::Keyboard::Key::Num8:
		return "8";
	case sf::Keyboard::Key::Num9:
		return "9";
	case sf::Keyboard::Key::Numpad0:
		return "0";
	case sf::Keyboard::Key::Numpad1:
		return "1";
	case sf::Keyboard::Key::Numpad2:
		return "2";
	case sf::Keyboard::Key::Numpad3:
		return "3";
	case sf::Keyboard::Key::Numpad4:
		return "4";
	case sf::Keyboard::Key::Numpad5:
		return "5";
	case sf::Keyboard::Key::Numpad6:
		return "6";
	case sf::Keyboard::Key::Numpad7:
		return "7";
	case sf::Keyboard::Key::Numpad8:
		return "8";
	case sf::Keyboard::Key::Numpad9:
		return "9";
	case sf::Keyboard::Key::F1:
		return "F1";
	case sf::Keyboard::Key::F2:
		return "F2";
	case sf::Keyboard::Key::F3:
		return "F3";
	case sf::Keyboard::Key::F4:
		return "F4";
	case sf::Keyboard::Key::F5:
		return "F5";
	case sf::Keyboard::Key::F6:
		return "F6";
	case sf::Keyboard::Key::F7:
		return "F7";
	case sf::Keyboard::Key::F8:
		return "F8";
	case sf::Keyboard::Key::F9:
		return "F9";
	case sf::Keyboard::Key::F10:
		return "F10";
	case sf::Keyboard::Key::F11:
		return "F11";
	case sf::Keyboard::Key::F12:
		return "F12";
	case sf::Keyboard::Key::Escape:
		return "Escape";
	case sf::Keyboard::Key::Up:
		return "Up";
	case sf::Keyboard::Key::Down:
		return "Down";
	case sf::Keyboard::Key::Left:
		return "Left";
	case sf::Keyboard::Key::Right:
		return "Right";
	case sf::Keyboard::Key::LShift:
		return "LShift";
	case sf::Keyboard::Key::RShift:
		return "RShift";
	case sf::Keyboard::Key::LAlt:
		return "LAlt";
	case sf::Keyboard::Key::RAlt:
		return "RAlt";
	case sf::Keyboard::Key::LControl:
		return "LControl";
	case sf::Keyboard::Key::RControl:
		return "RControl";
	case sf::Keyboard::Key::LSystem:
		return "LSystem";
	case sf::Keyboard::Key::RSystem:
		return "RSystem";
	case sf::Keyboard::Key::Add:
		return "+";
	case sf::Keyboard::Key::Subtract:
		return "-";
	case sf::Keyboard::Key::PageDown:
		return "PageDown";
	case sf::Keyboard::Key::PageUp:
		return "PageUp";
	case sf::Keyboard::Key::BackSlash:
		return "\\";
	}
}

//Function to load a file ready for testing
std::map<std::string, std::string> Utils::loadConfigFile(std::string file, std::string delim) {
	//Temporary map used for loading values
	std::map<std::string, std::string> tempMap;
	//File to be opened
	std::fstream file_s;
	//Try to open file
	file_s.open(file);

	//If file successfully opened, load data into tempMap and return to caller
	if (file_s.is_open()) {
		//Reference temp vars for string splitting
		signed short start = 0;
		signed short split = 0;
		signed short end = 0;

		//Loop through each line and extract data
		for (std::string fline; std::getline(file_s, fline); ) {
			//Find occurance of delim
			split = fline.find(delim);
			//Determine end of line
			end = fline.length();
			//Insert found data upon completion
			//Split+1 for the v so the delim char isn"t included
			tempMap.insert({ fline.substr(start, split), fline.substr(split + 1,end) });
			
#ifdef DEBUG_ENABLED
	#if DEBUG_ENABLED == true && DEBUG_LEVEL >= 2
			//ONLY IF DEBUG IS ENABLED!!
			//Detail everything loaded into the map
			std::cout << "[DEBUG]Map loading K:" << fline.substr(start, split) << " V:" << fline.substr(split + 1, end) << std::endl;
	#endif
#endif
		}

		//Clear all temporary variables
		start = NULL;
		split = NULL;
		end = NULL;
		file_s.close();

		//Return full map
		return tempMap;
	}
	else {
		//if file not loaded successfully, return blank map
#ifdef DEBUG_ENABLED
	#if DEBUG_ENABLED == true && DEBUG_LEVEL >= 1
		std::cerr << "Could not open file @" << file << "- please try again." << std::endl;
	#endif
#endif
		return tempMap;
	}
}

bool Utils::saveToConfigFile(std::string file, std::map<std::string, std::string>* data, std::string delim) {
	std::ofstream file_s(file);
	
	if (file_s.is_open()) {
		std::cout << "OUT" << std::endl;
		file_s.clear();
		for (auto const& val : *data) {
			file_s << val.first << delim << val.second << std::endl;
		}

		file_s.flush();
		file_s.close();
	}
	else {
		return false;
	}
}

std::string Utils::boolToString(bool b)
{
	switch (b) {
	case true:
		return "true";
	case false:
		return "false";
	}
}

bool Utils::stringToBool(std::string s) {
	for (auto& c : s) {
		c = std::tolower(c);
	}

	if (s == "true") {
		return true;
	}
	else {
		return false;
	}
}

ConfigFile::ConfigFile(std::string pathn, std::string delim) {
	data = Utils::loadConfigFile(pathn, delim);
}

ConfigFile::ConfigFile() {
	data = std::map<std::string, std::string>();
}

std::string ConfigFile::getValue(std::string key) {
	return data.at(key);
}

bool ConfigFile::addValue(std::string key, std::string val) {
	data.insert({ key, val });

	if (data.at(key) == val) {
		return true;
	}
	else {
		return false;
	}
}

bool ConfigFile::delValue(std::string key) {
	data.erase(key);
	return true;
}

std::map<std::string, std::string>* ConfigFile::getData() {
	return &data;
}

bool ConfigFile::loadConfigFile(std::string file, std::string delim) {
	data = Utils::loadConfigFile(file, delim);
	return true;
}

bool ConfigFile::setValue(std::string key, std::string val, bool doOverride) {
	if (data.at(key).length() > 0 && doOverride == true) {
		data[key] = val;
		return true;
	}
	else if (data.at(key).length() > 0 && doOverride == false) {
		return false;
	}
	else if (data.at(key).length() <= 0) {
		data[key] = val;
		return true;
	}
	else {
		data.insert({ key, val });
		return true;
	}
}