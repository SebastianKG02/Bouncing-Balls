#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "AssetManager.h"
#include "Defines.h"

#ifndef ENT_BALL_H
#define ENT_BALL_H

/*
BallColour - A simple enum to facilitate easy sorting of ball colours in-game
*/
enum BallColour
{
	RED,
	YELLOW,
	LIME,
	ROYAL_BLUE,
	DARK_BLUE,
	GREEN,
	EM_GREEN,
	TURQOUISE,
	CYAN,
	ORANGE,
	WINE,
	VIOLET,
	PURPLE,
	DARK_PURPLE,
	BRONZE,
	COBALT,
	AMBER
};

//Lightweight wrapper around sf::Sprite, does not need as many method * variables as Ball
struct BallSimple
{
	//Sprite to be rendered
	sf::Sprite* spr;

	//BallColour to determine logic
	BallColour col;

	//Death flag
	bool alive;

	float id = 0;

	float spr_id = 0;
};

struct BallRow
{
	std::vector<BallSimple*> row = std::vector<BallSimple*>();
	float id = 0;
};

struct BallNode
{
	int pos[2] = { 0, 0 };
};

class BallMap
{
public:
	BallMap(int rows, int columns, int min_colours, int max_colours, int startx, int starty, int dir);
	BallMap();
	void update(float mx, float my);
	std::vector<BallRow*> getMap();
	sf::Vector2f getEnd();
	sf::Vector2f getStart();
	BallRow* getRow(int rownum);
	BallSimple* getBall(int rownum, int colnum);
	float addRow(bool isBlank=false);
	float getCols();
	float getRows();
	float getBlanks();
private:
	float map_rows;
	float map_cols;
	float min_colours;
	float max_colours;
	float start_x;
	float start_y;
	float end_x;
	float end_y;
	float blanks_start;
	int dir;
	sf::Sprite ref;
	std::vector<BallRow*> map;
};

class Ball
{
public:
	Ball(int x, int y, BallColour b);
	static std::string ballColourToStr(BallColour b);
	static BallColour ballColourFromStr(std::string s);
	BallColour* getColour();
	void update();
	void path_plot(float y, float cannon_rot, sf::Vector2f spos);
	bool* alive();
	void kill();
	sf::Sprite* getSprite();
	sf::Vector2f* getTarget();
	float* getID();
	void setID(float id);
private:
	BallColour col;
	sf::Sprite* spr;
	sf::Vector2f target;
	sf::Vector2f motion;
	float *idRef = new float(0.f);
	bool *isAlive = new bool(true);
};

#endif