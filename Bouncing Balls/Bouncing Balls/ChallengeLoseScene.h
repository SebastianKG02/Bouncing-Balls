#pragma once
#ifndef SCN_CHALLENGE_LOSS
#define SCN_CHALLENGE_LOSS

#include "SceneManager.h"

class ChallengeLoseScene : public Scene
{
public:
	ChallengeLoseScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0;
};

#endif