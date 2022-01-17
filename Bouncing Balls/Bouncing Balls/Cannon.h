#pragma once

#ifndef ENT_CANNON
#define ENT_CANNON

#include <SFML/Graphics.hpp>
#include "Defines.h"
class Cannon
{
public:
	Cannon(int x, int y, bool centerX=false, bool centerY=false);
	Cannon();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
	float* X();
	float* Y();
	sf::Sprite* getBase();
	sf::Sprite* getAxel();
	sf::Sprite* getBody();
protected:
	sf::Sprite base;
	sf::Sprite axel;
	sf::Sprite body;
	float minRot = GM_MIN_CANNON_ROT;
	float maxRot = GM_MAX_CANNON_ROT;
	float rot;
	float x, y;
	bool mouseMove = false;
};

#endif