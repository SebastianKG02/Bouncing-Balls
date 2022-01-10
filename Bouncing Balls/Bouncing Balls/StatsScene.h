#pragma once
#include "SceneManager.h"

#ifndef SCN_STATS
#define SCN_STATS

class StatsScene :  public Scene
{
public:
	StatsScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0.f;
};

#endif

