#pragma once
#include "SceneManager.h"

#ifndef SCN_CHALLENGE_PRE
#define SCN_CHALLENGE_PRE

class ChallengePreScene :public Scene
{
public:
	ChallengePreScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0.f;
};

#endif

