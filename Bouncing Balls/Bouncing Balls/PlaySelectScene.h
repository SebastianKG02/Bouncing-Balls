#pragma once
#include "SceneManager.h"

#ifndef SCN_PLAY_SELECT
#define SCN_PLAY_SELECT

class PlaySelectScene : public Scene
{
public:
	PlaySelectScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0.f;
};

#endif