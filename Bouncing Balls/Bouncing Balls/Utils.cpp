#include "Utils.h"
#include "Config.h"

//Will convert string to all lowercase
std::string* Utils::strToLower(std::string* str) {
	//Loop through each char in string
	for (auto& c : *str) {
		c=std::tolower(c);
	}
	return str;
}

//Convert string to all uppercase
std::string* Utils::strToUpper(std::string* str) {
	//Loop through each char in string
	for (auto& c : *str) {
		c = std::toupper(c);
	}
	return str;
}

//Converstion switch-case to get from char to key
sf::Keyboard::Key Utils::charToKey(std::string keyt) {
	Utils::strToUpper(&keyt);

	const char *c = keyt.c_str();
	switch (c[0]) {
	case 'A':
		return sf::Keyboard::Key::A;
	case 'B':
		if (keyt.size() >= 4) {
			return sf::Keyboard::Key::BackSpace;
		}
		else {
			return sf::Keyboard::Key::B;
		}
	case 'C':
		if (keyt.size() >= 3) {
			return sf::Keyboard::Key::Comma;
		}
		else {
			return sf::Keyboard::Key::C;
		}
	case 'D':
		if (keyt.size() >= 3) {
			switch (keyt[1]) {
			case 'A':
				return sf::Keyboard::Key::Dash;
			case 'E':
				return sf::Keyboard::Key::Delete;
			}
		}
		else {
			return sf::Keyboard::Key::D;
		}
	case 'E':
		if (keyt.size() > 1) {
			switch (keyt[2]) {
			case 'D':
				return sf::Keyboard::Key::End;
			case 'T':
				return sf::Keyboard::Key::Enter;
			case 'C':
				return sf::Keyboard::Key::Escape;
			}
		}
		else {
			return sf::Keyboard::Key::E;
		}
	case 'F':
		if (keyt.size() > 1) {
			if (keyt.size() > 2) {
				switch (keyt[2]) {
				case '0':
					return sf::Keyboard::Key::F10;
				case '1':
					return sf::Keyboard::Key::F11;
				case '2':
					return sf::Keyboard::Key::F12;
				case '3':
					return sf::Keyboard::Key::F13;
				case '4':
					return sf::Keyboard::Key::F14;
				case '5':
					return sf::Keyboard::Key::F15;
				}
			}
			switch (keyt[1]) {
			case '1':
				return sf::Keyboard::Key::F1;
			case '2':
				return sf::Keyboard::Key::F2;
			case '3':
				return sf::Keyboard::Key::F3;
			case '4':
				return sf::Keyboard::Key::F4;
			case '5':
				return sf::Keyboard::Key::F5;
			case '6':
				return sf::Keyboard::Key::F6;
			case '7':
				return sf::Keyboard::Key::F7;
			case '8':
				return sf::Keyboard::Key::F8;
			case '9':
				return sf::Keyboard::Key::F9;
			}
		}
		else {
			return sf::Keyboard::Key::F;
		}
	case 'G':
		return sf::Keyboard::Key::G;
	case 'H':
		if (keyt.size() >= 4) {
			return sf::Keyboard::Key::Home;
		}
		return sf::Keyboard::Key::H;
	case 'I':
		if (keyt.size() >= 3) {
			return sf::Keyboard::Key::Insert;
		}
		else {
			return sf::Keyboard::Key::I;
		}
	case 'J':
		return sf::Keyboard::Key::J;
	case 'K':
		if (keyt.size() == 4) {
			switch (keyt[3]) {
			case 'R':
				return sf::Keyboard::Key::Right;
			case 'U':
				return sf::Keyboard::Key::Up;
			case 'D':
				return sf::Keyboard::Key::Down;
			case 'L':
				return sf::Keyboard::Key::Left;
			}
		}
		else {
			return sf::Keyboard::Key::K;
		}
	case 'L':
		if (keyt.size() > 1) {
			switch (keyt[1]) {
			case 'S':
				if (keyt.size() == 4 && keyt[3] == 't') {
					//Lsft
					return sf::Keyboard::Key::LShift;
				}
				else {
					//Lsys
					return sf::Keyboard::Key::LSystem;
				}
			case 'A':
				return sf::Keyboard::Key::LAlt;
			case 'C':
				return sf::Keyboard::Key::LControl;
			}
		}
		else {
			return sf::Keyboard::Key::L;
		}
	case 'M':
		if (keyt.size() >= 4) {
			return sf::Keyboard::Key::Menu;
		}
		else {
			return sf::Keyboard::Key::M;
		}
	case 'N':
		if (keyt.size() == 4) {
			switch (keyt[1]) {
			case 'U':
				switch (keyt[3]) {
				case '0':
					return sf::Keyboard::Key::Num0;
				case '1':
					return sf::Keyboard::Key::Num1;
				case '2':
					return sf::Keyboard::Key::Num2;
				case '3':
					return sf::Keyboard::Key::Num3;
				case '4':
					return sf::Keyboard::Key::Num4;
				case '5':
					return sf::Keyboard::Key::Num5;
				case '6':
					return sf::Keyboard::Key::Num6;
				case '7':
					return sf::Keyboard::Key::Num7;
				case '8':
					return sf::Keyboard::Key::Num8;
				case '9':
					return sf::Keyboard::Key::Num9;
				default:
					return sf::Keyboard::Key::Num0;
				}
			case 'P':
				switch (keyt[3]) {
				case '0':
					return sf::Keyboard::Key::Numpad0;
				case '1':
					return sf::Keyboard::Key::Numpad1;
				case '2':
					return sf::Keyboard::Key::Numpad2;
				case '3':
					return sf::Keyboard::Key::Numpad3;
				case '4':
					return sf::Keyboard::Key::Numpad4;
				case '5':
					return sf::Keyboard::Key::Numpad5;
				case '6':
					return sf::Keyboard::Key::Numpad6;
				case '7':
					return sf::Keyboard::Key::Numpad7;
				case '8':
					return sf::Keyboard::Key::Numpad8;
				case '9':
					return sf::Keyboard::Key::Numpad9;
				}
			}
		}
		else {
			return sf::Keyboard::Key::N;
		}
	case 'O':
		return sf::Keyboard::Key::O;
	case 'P':
		if (keyt.size() >= 3) {
			switch (keyt[3]) {
			case 'N':
				return sf::Keyboard::Key::PageDown;
			case 'P':
				return sf::Keyboard::Key::PageUp;
			case 'U':
				return sf::Keyboard::Key::Pause;
			}

		}
		else {
			return sf::Keyboard::Key::P;
		}
	case 'Q':
		return sf::Keyboard::Key::Q ;
	case 'R':
		if (keyt.size() > 1) {
			switch (keyt[1]) {
			case 'S':
				if (keyt.size() == 4 && keyt[3] == 't') {
					//Rsft
					return sf::Keyboard::Key::RShift;
				}
				else {
					//Rsys
					return sf::Keyboard::Key::RSystem;
				}
			case 'A':
				return sf::Keyboard::Key::RAlt;
			case 'C':
				return sf::Keyboard::Key::RControl;
			}
		}
		else {
			return sf::Keyboard::Key::R;
		}
	case 'S':
		if (keyt.size() >= 3) {
			return sf::Keyboard::Key::Space;
		}
		else {
			return sf::Keyboard::Key::S;
		}
	case 'T':
		if (keyt.size() >= 3) {
			return sf::Keyboard::Key::Tab;
		}
		else {
			return sf::Keyboard::Key::T;
		}
	case 'U':
		return sf::Keyboard::Key::U ;
	case 'V':
		return sf::Keyboard::Key::V ;
	case 'W':
		return sf::Keyboard::Key::W ;
	case 'X':
		return sf::Keyboard::Key::X ;
	case 'Y':
		return sf::Keyboard::Key::Y ;
	case 'Z':
		return sf::Keyboard::Key::B ;
	case '+':
		return sf::Keyboard::Key::Add ;
	case '\\':
		return sf::Keyboard::Key::Backslash ;
	case '/':
		return sf::Keyboard::Key::Divide ;
	case '=':
		return sf::Keyboard::Key::Equal ;
	case '[':
		return sf::Keyboard::Key::LBracket ;
	case ']':
		return sf::Keyboard::Key::RBracket ;
	case '.':
		return sf::Keyboard::Key::Period ;
	case ';':
		return sf::Keyboard::Key::Semicolon ;
	case '\'':
		return sf::Keyboard::Key::Quote ;
	case '~':
		return sf::Keyboard::Key::Tilde;
	case '*':
		return sf::Keyboard::Key::Multiply ;
	default:
		return sf::Keyboard::Key::Unknown ;
	}
}

//Converstion switch-case to get from key to char
std::string Utils::keyToStr(sf::Keyboard::Key key) {
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
		return "Num0";
	case sf::Keyboard::Key::Num1:
		return "Num1";
	case sf::Keyboard::Key::Num2:
		return "Num2";
	case sf::Keyboard::Key::Num3:
		return "Num3";
	case sf::Keyboard::Key::Num4:
		return "Num4";
	case sf::Keyboard::Key::Num5:
		return "Num5";
	case sf::Keyboard::Key::Num6:
		return "Num6";
	case sf::Keyboard::Key::Num7:
		return "Num7";
	case sf::Keyboard::Key::Num8:
		return "Num8";
	case sf::Keyboard::Key::Num9:
		return "Num9";
	case sf::Keyboard::Key::Numpad0:
		return "Npd0";
	case sf::Keyboard::Key::Numpad1:
		return "Npd1";
	case sf::Keyboard::Key::Numpad2:
		return "Npd2";
	case sf::Keyboard::Key::Numpad3:
		return "Npd3";
	case sf::Keyboard::Key::Numpad4:
		return "Npd4";
	case sf::Keyboard::Key::Numpad5:
		return "Npd5";
	case sf::Keyboard::Key::Numpad6:
		return "Npd6";
	case sf::Keyboard::Key::Numpad7:
		return "Npd7";
	case sf::Keyboard::Key::Numpad8:
		return "Npd8";
	case sf::Keyboard::Key::Numpad9:
		return "Npd9";
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
		return "Esc";
	case sf::Keyboard::Key::Up:
		return "KeyU";
	case sf::Keyboard::Key::Down:
		return "KeyD";
	case sf::Keyboard::Key::Left:
		return "KeyL";
	case sf::Keyboard::Key::Right:
		return "KeyR";
	case sf::Keyboard::Key::LShift:
		return "LSft";
	case sf::Keyboard::Key::RShift:
		return "RSft";
	case sf::Keyboard::Key::LAlt:
		return "LAlt";
	case sf::Keyboard::Key::RAlt:
		return "RAlt";
	case sf::Keyboard::Key::LControl:
		return "LCtl";
	case sf::Keyboard::Key::RControl:
		return "RCtl";
	case sf::Keyboard::Key::LSystem:
		return "LSys";
	case sf::Keyboard::Key::RSystem:
		return "RSys";
	case sf::Keyboard::Key::Add:
		return "+";
	case sf::Keyboard::Key::Subtract:
		return "-";
	case sf::Keyboard::Key::PageDown:
		return "PgDn";
	case sf::Keyboard::Key::PageUp:
		return "PgUp";
	case sf::Keyboard::Key::BackSlash:
		return "\\";
	case sf::Keyboard::Key::BackSpace:
		return "Bksp";
	case sf::Keyboard::Key::Comma:
		return ",";
	case sf::Keyboard::Key::Dash:
		return "-";
	case sf::Keyboard::Key::Delete:
		return "Del";
	case sf::Keyboard::Key::Divide:
		return "/";
	case sf::Keyboard::Key::End:
		return "End";
	case sf::Keyboard::Key::Enter:
		return "Ent";
	case sf::Keyboard::Key::Equal:
		return "=";
	case sf::Keyboard::Key::F13:
		return "F13";
	case sf::Keyboard::Key::F14:
		return "F14";
	case sf::Keyboard::Key::F15:
		return "F15";
	case sf::Keyboard::Key::Home:
		return "Home";
	case sf::Keyboard::Key::Insert:
		return "Ins";
	case sf::Keyboard::Key::LBracket:
		return "[";
	case sf::Keyboard::Key::RBracket:
		return "]";
	case sf::Keyboard::Key::Period:
		return ".";
	case sf::Keyboard::Key::Semicolon:
		return ";";
	case sf::Keyboard::Key::Quote:
		return "\'";
	case sf::Keyboard::Key::Tilde:
		return "~";
	case sf::Keyboard::Key::Space:
		return "Sp";
	case sf::Keyboard::Key::Menu:
		return "Menu";
	case sf::Keyboard::Key::Tab:
		return "Tab";
	case sf::Keyboard::Key::Multiply:
		return "*";
	case sf::Keyboard::Key::Slash:
		return "/";
	case sf::Keyboard::Key::Pause:
		return "Paus";
	default:
		return "Ukwn";
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
	#if DEBUG_LEVEL >= DB_LEVEL_MINERR
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
	#if DEBUG_LEVEL >= DB_LEVEL_MINERR
		std::cerr << "Could not open file @" << file << " please try again." << std::endl;
	#endif
#endif
		return tempMap;
	}
}

//Save config file to system
bool Utils::saveToConfigFile(std::string file, std::map<std::string, std::string>* data, std::string delim) {
	//Open file in output
	std::ofstream file_s(file);
	
	//std::cout << data->at("frame_limit") << std::endl;

	//If file opened
	if (file_s.is_open()) {
		//Clear entire file
		file_s.clear();
		//Loop through each element in data
		for (auto const& val : *data) {
#ifdef DEBUG_ENABLED 
	#if DEBUG_LEVEL >= DB_LEVEL_ALL
			//Debug only! See if all values loading
			std::cout << "Saving K: " << val.first << "\tV: " << val.second << std::endl;
	#endif
#endif
			//Write to file
			file_s << val.first << delim << val.second << std::endl;
		}

		//Close file & save
		file_s.flush();
		file_s.close();
	}
	else {
		//File could not be opened or created
		return false;
	}
}

//Convert bool to string method
std::string Utils::boolToString(bool b)
{
	switch (b) {
	case true:
		return "true";
	case false:
		return "false";
	}
}

//Get bool from string method
bool Utils::stringToBool(std::string s) {
	//get all chars in string s and lower 
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

//Preferred constructor, using file to load data
ConfigFile::ConfigFile(std::string pathn, std::string delim) {
	data = Utils::loadConfigFile(pathn, delim);
}

//Default constructor with empty map
ConfigFile::ConfigFile() {
	data = std::map<std::string, std::string>();
}

//Get value from map using string key
std::string ConfigFile::getValue(std::string key) {
	try {
		return data.at(key);
	}
	catch (std::exception e) {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
		std::cout << "[DEBUG]Could not get value <" << key << "> from map. " << e.what() << std::endl;
		return "";
#endif
#endif
	}
}

//Add value (val) to map under key (key)
bool ConfigFile::addValue(std::string key, std::string val) {
	data.insert({ key, val });

	//See if successful, return true/false based on result
	if (data.at(key) == val) {
		return true;
	}
	else {
		return false;
	}
}

//Delete value from map, return true to confirm deletion has been successful
bool ConfigFile::delValue(std::string key) {
	data.erase(key);
	return true;
}

//Get pointer to entire map
std::map<std::string, std::string>* ConfigFile::getData() {
	return &data;
}

//Load config file using Utils method
bool ConfigFile::loadConfigFile(std::string file, std::string delim) {
	data = Utils::loadConfigFile(file, delim);
	return true;
}

//Set value in map (checks for existing values and uses doOverride to see if user wants to override existing data
bool ConfigFile::setValue(std::string key, std::string val, bool doOverride) {
	//If data is present and override is true, overwrite data and return true
	if (data.at(key).length() > 0 && doOverride == true) {
		data[key] = val;
		return true;
	}
	//If data is present and override is false, do nothing and return false
	else if (data.at(key).length() > 0 && doOverride == false) {
		return false;
	}
	//If no data present BUT key is present, set value and return true
	else if (data.at(key).length() <= 0) {
		data[key] = val;
		return true;
	}
	else {
		//If no data and no key present, add data to map and return true
		data.insert({ key, val });
		return true;
	}
}