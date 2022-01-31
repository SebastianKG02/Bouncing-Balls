#pragma once
#ifndef SCN_ENDLESS_LOSS
#define SCN_ENDLESS_LOSS

#include "SceneManager.h"

class EndlessLoseScene : public Scene
{
public:
	EndlessLoseScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0;
};

#endif