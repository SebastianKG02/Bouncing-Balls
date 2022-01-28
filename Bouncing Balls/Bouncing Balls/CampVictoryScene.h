#pragma once

#ifndef SCN_CAMPAIGN_VICTORY
#define SCN_CAMPAIGN_VICTORY

#include "SceneManager.h"

class CampVictoryScene : public Scene
{
public:
	CampVictoryScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0;
};

#endif

