#include "Utils.h"

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