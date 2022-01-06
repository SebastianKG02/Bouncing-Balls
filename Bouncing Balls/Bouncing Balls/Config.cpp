#include "Config.h"

sf::Keyboard::Key Config::user_key_left = sf::Keyboard::Left;
sf::Keyboard::Key Config::user_key_right = sf::Keyboard::Right;
sf::Keyboard::Key Config::user_key_pause = sf::Keyboard::P;
sf::Keyboard::Key Config::user_key_shoot = sf::Keyboard::Space;
sf::Keyboard::Key Config::user_key_exit = sf::Keyboard::Escape;
Resolution Config::user_resolution = Resolution();
bool Config::user_do_fullscreen = false;
bool Config::user_do_vsync = false;
bool Config::user_do_frame_limit = false;
ConfigFile Config::config;
signed short Config::user_frame_limit_val = false;
signed short Config::user_sfx_volume = 10;
signed short Config::user_music_volume = 10;

ConfigFile* Config::getSettings() {
	return &config;
}

float Config::applyRDX(float xval) {
	return (xval * user_resolution.diffX());
}

float Config::applyRDY(float yval) {
	return (yval * user_resolution.diffY());
}

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
	config.addValue("sfx_volume", std::to_string(user_sfx_volume));
	config.addValue("music_volume", std::to_string(user_music_volume));

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
	user_resolution = Resolution(config.getValue("resolution"));

	if (tryGetValue("fullscreen").empty()) {
		config.addValue("fullscreen", Utils::boolToString(user_do_fullscreen));
	}
	else {
		user_do_fullscreen = Utils::stringToBool(tryGetValue("fullscreen"));
	}

	if (tryGetValue("vsync").empty()) {
		config.addValue("vsync", Utils::boolToString(user_do_vsync));
	}
	else {
		user_do_vsync = Utils::stringToBool(tryGetValue("vsync"));
	}

	if (tryGetValue("frame_limit").empty()) {
		config.addValue("frame_limit", Utils::boolToString(user_do_vsync));
	}
	else {
		user_do_frame_limit = Utils::stringToBool(tryGetValue("frame_limit"));
	}

	if (tryGetValue("frame_limit_value").empty()) {
		config.addValue("frame_limit_value", std::to_string(user_frame_limit_val));
	}
	else {
		user_frame_limit_val = std::stoi(tryGetValue("frame_limit_value"));
	}

	if (tryGetValue("sfx_volume").empty()) {
		config.addValue("sfx_volume", std::to_string(user_sfx_volume));
	}
	else {
		user_sfx_volume = std::stoi(tryGetValue("sfx_volume"));
	}

	if (tryGetValue("music_volume").empty()) {
		config.addValue("music_volume", std::to_string(user_music_volume));
	}
	else {
		user_music_volume = std::stoi(tryGetValue("music_volume"));
	}

	return saveCurrentConfig("config.cfg");
}

void Config::applySettings(sf::RenderWindow* window) {
	if (user_do_frame_limit == true) {
		window->setFramerateLimit(user_frame_limit_val);
	}
	else {
		window->setFramerateLimit(0);
	}
	window->setVerticalSyncEnabled(user_do_vsync);

	if (user_do_fullscreen == true) {
		window->create(sf::VideoMode(user_resolution.X(), user_resolution.Y()), "Bouncing Balls", sf::Style::Fullscreen);
	}
	else {
		window->create(sf::VideoMode(user_resolution.X(), user_resolution.Y()), "Bouncing Balls", sf::Style::Close);
	}
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