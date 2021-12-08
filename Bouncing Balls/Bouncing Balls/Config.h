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
	signed short user_key_left;
	signed short user_key_right;
	signed short user_key_pause;
	signed short user_key_shoot;
	signed short user_key_exit;
	Resolution user_resolution;
	bool user_do_fullscreen;
	bool user_do_vsync;
	bool user_do_frame_limit;
	signed short user_frame_limit_val;
	Config();
	Config(std::string fromFile, bool ifNoUseDefault=true);
	bool saveCurrentConfig(std::string path);
	bool loadFromConfig(std::string fpath);
private:
	const signed short default_key_left = sf::Keyboard::Left;
	const signed short default_key_right = sf::Keyboard::Right;
	const signed short default_key_pause = sf::Keyboard::P;
	const signed short default_key_shoot = sf::Keyboard::Space;
	const signed short default_key_exit = sf::Keyboard::Escape;
	const Resolution default_resolution = Resolution();
	const bool default_do_fullscreen = false;
	const bool default_do_vsync = false;
	const bool default_do_frame_limit = false;
	const signed short default_frame_limit_val = 60;
	ConfigFile config;
	void assignDefaultValues();

};

#endif

