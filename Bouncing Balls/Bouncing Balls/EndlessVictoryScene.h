#pragma once
#ifndef SCN_ENDLESS_VICTORY
#define SCN_ENDLESS_VICTORY

#include "SceneManager.h"

class EndlessVictoryScene : public Scene
{
public:
	EndlessVictoryScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0;
};

#endif
