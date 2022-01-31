#pragma once

#include "SceneManager.h"

#ifndef SCN_ENDLESS_PRE
#define SCN_ENDLESS_PRE

class EndlessPreGameScene :public Scene
{
public:
	EndlessPreGameScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0.f;
};

#endif