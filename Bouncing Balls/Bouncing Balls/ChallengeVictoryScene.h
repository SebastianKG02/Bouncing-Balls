#pragma once
#pragma once
#ifndef SCN_CHALLENGE_VICTORY
#define SCN_CHALLENGE_VICTORY

#include "SceneManager.h"

class ChallengeVictoryScene : public Scene
{
public:
	ChallengeVictoryScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0;
};

#endif