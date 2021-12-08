#include "Config.h"

Config::Config() {
	assignDefaultValues();
	config.addValue("key_left", std::to_string(user_key_left));
	config.addValue("key_right", std::to_string(user_key_left));
	config.addValue("key_pause", std::to_string(user_key_pause));
	config.addValue("key_shoot", std::to_string(user_key_shoot));
	config.addValue("key_exit", std::to_string(user_key_exit));
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
	}
}

bool Config::saveCurrentConfig(std::string path) {
	Utils::saveToConfigFile(path, config.getData());
	return false;
}

bool Config::loadFromConfig(std::string fpath) {
	config.loadConfigFile(fpath);
	return false;
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