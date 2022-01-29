#pragma once

#ifndef PLAYER_H
#define PLAYER_H

/*
Simple struct for player data
*/
struct PlayerData
{
	//Campaign completion markers
	bool campaign_comp[6] = { false, false, false, false, false, false };

	//Campaign completion flag
	bool campaign_finished = false;

	//Campaign completion stars
	int campaign_stars[6] = { 0, 0, 0, 0, 0, 0 };

	//Highscore in endless mode
	long endless_hs = 0;

	//Highscore in challenge mode
	long challenge_hs = 0;

	//Total coins
	long coins = 0;

	//Last played level by player
	int last_level = 0;

	//Last played game mode by player
	//-1 - Challenge
	// 0 - Campaign
	// 1 - Endless
	int last_mode = 0;

	//Number of powerups
	// 0 - Double points powerup count
	// 1 - 15s time save powerup count
	int num_powerups[2] = { 0, 0 };
};

#include <iostream>
#include <fstream>
#include "Defines.h"
/*
Simple interaction wrapper for PlayerData
*/
class Player
{
public:
	//Initalise this container (load data)
	static void init();
	//Load player data from disk (p.dat)
	static int load();
	//Save player data to disk (p.dat)
	static int save();
	//Access & maniuplate data loaded from disk
	static PlayerData* getData();
private:
	//Actual data
	static PlayerData data;
};

#endif