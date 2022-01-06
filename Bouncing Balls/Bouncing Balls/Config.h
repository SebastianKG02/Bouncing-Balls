#pragma once

#ifndef CONFIG_H
#define CONFIG_H

/*
Simple class to contain all user-based and game-based configuration
Will also be able to load current user's settings from disk via a simple method
*/

#include <SFML/Graphics.hpp>
#include "Resolution.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>

class Config
{
public:
	static sf::Keyboard::Key user_key_left;
	static sf::Keyboard::Key user_key_right;
	static sf::Keyboard::Key user_key_pause;
	static sf::Keyboard::Key user_key_shoot;
	static sf::Keyboard::Key user_key_exit;
	static signed short user_sfx_volume;
	static signed short user_music_volume;
	static Resolution user_resolution;
	static bool user_do_fullscreen;
	static bool user_do_vsync;
	static bool user_do_frame_limit;
	static signed short user_frame_limit_val;
	Config();
	Config(std::string fromFile, bool ifNoUseDefault=true);
	static bool saveCurrentConfig(std::string path);
	static bool loadFromConfig(std::string fpath);
	static ConfigFile* getSettings();
	static std::string tryGetValue(std::string key);
	static void applySettings(sf::RenderWindow* window);
	static float applyRDX(float xval);
	static float applyRDY(float yval);
private:
	const sf::Keyboard::Key default_key_left = sf::Keyboard::Left;
	const sf::Keyboard::Key default_key_right = sf::Keyboard::Right;
	const sf::Keyboard::Key default_key_pause = sf::Keyboard::P;
	const sf::Keyboard::Key default_key_shoot = sf::Keyboard::Space;
	const sf::Keyboard::Key default_key_exit = sf::Keyboard::Escape;
	const signed short default_sfx_volume = 10;
	const signed short default_music_volume = 10;
	const Resolution default_resolution = Resolution();
	const bool default_do_fullscreen = false;
	const bool default_do_vsync = false;
	const bool default_do_frame_limit = false;
	const signed short default_frame_limit_val = 60;
	static ConfigFile config;
	void assignDefaultValues();

};

#endif

