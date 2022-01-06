#pragma once
#include "SceneManager.h"

#ifndef SCN_SETTINGS
#define SCN_SETTINGS

class SettingsScene : public Scene
{
public:
	SettingsScene(uint8_t id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	float centerX = 0.f;
	bool resize = false;
	sf::Clock clock;
};

#endif

