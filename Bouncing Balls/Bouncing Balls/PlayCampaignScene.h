#pragma once
#include "SceneManager.h"
#include "Cannon.h"
#include "Ball.h"
#include "GameSettings.h"

#ifndef SCN_PLAY_CAMP
#define SCN_PLAY_CAMP

class PlayCampScene : public Scene
{
public:
	PlayCampScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	void addRowReassignSprites();
	void cleanup();
	void updatePauseLabel();
	void updateScoreLabel();
	void updateClockLabel();
	void updateBallsLeft();
	BallColour getNextColour();
	sf::Clock clock;
	sf::Clock game_clock;
	long time_elapsed = 0;
	Cannon p1Cannon;
	sf::RectangleShape tracer;
	sf::RectangleShape left_bound;
	sf::RectangleShape right_bound;
	sf::RectangleShape death_bound;
	sf::RectangleShape ball_eraser;
	bool resize = false;
	//won, lost
	bool status[2] = { false, false };
	bool started = false;
	bool paused = false;
	float centerX = 0.f;
	int num_balls;
	int maxCols;
	int sprites_map_begin = 0;
	int sprites_map_end = 0;
	long playerScore = 0;
	std::vector<Ball*> balls;
	std::vector<BallSimple*> nextBalls;
	GameSettings* rules;
	BallMap map;
	bool fired = false;
};

#endif