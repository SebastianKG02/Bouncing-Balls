#include "Config.h"

//Default constructor
Config::Config() {
	assignDefaultValues();
	config.addValue("key_left", Utils::keyToStr(user_key_left));
	config.addValue("key_right", Utils::keyToStr(user_key_right));
	config.addValue("key_pause", Utils::keyToStr(user_key_pause));
	config.addValue("key_shoot", Utils::keyToStr(user_key_shoot));
	config.addValue("key_exit", Utils::keyToStr(user_key_exit));
	config.addValue("resolution", user_resolution.name());
	config.addValue("fullscreen", Utils::boolToString(user_do_fullscreen));
	config.addValue("vsync", Utils::boolToString(user_do_vsync));
	config.addValue("frame_limit", Utils::boolToString(user_do_frame_limit));
	config.addValue("frame_limit_value", std::to_string(user_frame_limit_val));

	saveCurrentConfig("config.cfg");
}

Config::Config(std::string fromFile, bool isNoUseDefault) {
	std::fstream file;
	file.open(fromFile);

	if (file.is_open() == true) {
		file.close();
		loadFromConfig(fromFile);
	}
	else {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= 1
		std::cerr << "Error loading config @" << fromFile << "- please check the path provided. Using default values instead." << std::endl;
	#endif
#endif
		assignDefaultValues();
		loadFromConfig(fromFile);
		saveCurrentConfig(fromFile);
	}
}

bool Config::saveCurrentConfig(std::string path) {
	Utils::saveToConfigFile(path, config.getData());
	return false;
}

bool Config::loadFromConfig(std::string fpath) {
	config.loadConfigFile(fpath);

	if (tryGetValue("key_left").empty()) {
		config.addValue("key_left", Utils::keyToStr(user_key_left));
	}
	user_key_left = Utils::charToKey(config.getValue("key_left").c_str());

	if (tryGetValue("key_right").empty()) {
		config.addValue("key_right", Utils::keyToStr(user_key_right));
	}
	user_key_right = Utils::charToKey(config.getValue("key_right").c_str());

	if (tryGetValue("key_pause").empty()) {
		config.addValue("key_pause", Utils::keyToStr(user_key_pause));
	}
	user_key_pause= Utils::charToKey(config.getValue("key_pause").c_str());

	if (tryGetValue("key_shoot").empty()) {
		config.addValue("key_shoot", Utils::keyToStr(user_key_shoot));
	}
	user_key_shoot = Utils::charToKey(config.getValue("key_shoot").c_str());

	if (tryGetValue("key_exit").empty()) {
		config.addValue("key_exit", "" + Utils::keyToStr(user_key_exit));
	}
	user_key_exit = Utils::charToKey(config.getValue("key_exit").c_str());

	if (tryGetValue("resolution").empty()) {
		config.addValue("resolution", user_resolution.name());
	}

	if (tryGetValue("fullscreen").empty()) {
		config.addValue("fullscreen", Utils::boolToString(user_do_fullscreen));
	}

	if (tryGetValue("vsync").empty()) {
		config.addValue("vsync", Utils::boolToString(user_do_vsync));
	}

	if (tryGetValue("frame_limit").empty()) {
		config.addValue("frame_limit", Utils::boolToString(user_do_vsync));
	}

	if (tryGetValue("frame_limit_value").empty()) {
		config.addValue("frame_limit_value", std::to_string(user_frame_limit_val));
	}

	return saveCurrentConfig("config.cfg");
}

std::string Config::tryGetValue(std::string key) {
	try {
		config.getData()->find(key);
		return config.getData()->at(key);
	}
	catch (std::exception e) {
#ifdef DEBUG_ENABLED 
	#if DEBUG_LEVEL >= 3
		std::cerr << "Error getting value <" << key << "> from config." << std::endl;
		std::cerr << e.what() << std::endl;
	#endif
#endif
		return "";
	}
}

void Config::assignDefaultValues() {
	user_key_left = default_key_left;
	user_key_right = default_key_right;
	user_key_pause = default_key_pause;
	user_key_shoot = default_key_shoot;
	user_key_exit = default_key_exit;
	user_resolution = default_resolution;
	user_do_fullscreen = default_do_fullscreen;
	user_do_vsync = default_do_vsync;
	user_do_frame_limit = default_do_frame_limit;
	user_frame_limit_val = default_frame_limit_val;
}