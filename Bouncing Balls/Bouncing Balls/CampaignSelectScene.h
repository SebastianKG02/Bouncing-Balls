#pragma once
#include "SceneManager.h"

#ifndef SCN_CAMPAIGN_SELECT
#define SCN_CAMPAIGN_SELECT

class CampaignSelectScene : public Scene
{
public:
	CampaignSelectScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0.f;
};

#endif