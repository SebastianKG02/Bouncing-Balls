#pragma once
#include "SceneManager.h"
#include "Cannon.h"
#include "Ball.h"

#ifndef SCN_PLAY_L1
#define SCN_PLAY_L1

class PlayL1Scene : public Scene
{
public:
	PlayL1Scene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	Cannon p1Cannon;
	sf::RectangleShape tracer;
	sf::RectangleShape left_bound;
	sf::RectangleShape right_bound;
	sf::RectangleShape death_bound;
	sf::RectangleShape ball_eraser;
	bool resize = false;
	float centerX = 0.f;
	int num_balls;
	int maxCols;
	int sprites_map_begin = 0;
	int sprites_map_end = 0;
	int sprites_opp_begin = 0;
	int sprites_opp_end = 0;
	int sprites_pball_begin = 0;
	int sprites_pball_end = 0;
	float num_sprites_killed = 0;
	std::vector<Ball*> balls;
	BallMap map;
	BallMap opp;
	bool fired = false;
};

#endif