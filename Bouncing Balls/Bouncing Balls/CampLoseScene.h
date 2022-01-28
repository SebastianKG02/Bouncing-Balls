#pragma once
#ifndef SCN_CAMPAIGN_LOSS
#define SCN_CAMPAIGN_LOSS

#include "SceneManager.h"

class CampLoseScene : public Scene
{
public:
	CampLoseScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0;
};

#endif


