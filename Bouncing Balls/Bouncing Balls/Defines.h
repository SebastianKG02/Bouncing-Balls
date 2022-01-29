#pragma once

#ifndef DEFINES_H
#define DEFINES_H

//Default screen size
#define DEFAULT_SCREEN_X 1280
#define DEFAULT_SCREEN_Y 720

//Remeber to set to false upon release
#define DEBUG_ENABLED true
/*
Remeber to set to 0 upon release
Level 0 - No debug messages
Level 1 - All error messages, no diagnostic messages
Level 2 - All error messages, most diagnostic messages
Level 3 - All error messages and all diagnostic messages
*/
#define DB_LEVEL_NONE 0 //None
#define DB_LEVEL_ERRONLY 1 //Error-only
#define DB_LEVEL_MINERR 2 //Minimal diagnostics + Errors
#define DB_LEVEL_ALL 3 //All diagnostics + Errors
#define DEBUG_LEVEL DB_LEVEL_ALL

#define DB_SHOW_HITBOXES false

//Gameplay defines
#define GM_MIN_CANNON_ROT -60.f
#define GM_MAX_CANNON_ROT 60.f

#define GM_KEYBOARD_MOVE_SPEED 1.f
#define GM_BALL_SHOOT_SPEED 0.85f
#define GM_BALL_MOVE_SPEED 0.005f
//delay as multiple of a second
#define GM_BALL_SHOOT_DELAY  0.300f

//---------------------------------------------------
//Scoring defines
#define GM_PTS_PER_BALL 50
#define GM_PTS_PER_WIN 1000
#define GM_PTS_PW_LEVEL_MOD 150
//Difficulty defines
//Increase in ball colours per level
#define GM_LEVEL_DIFF_MOD_COLOUR 1
//Increase in rows per level
#define GM_LEVEL_DIFF_MOD_ROWS 1
//Increase in 2 star time limit per level
#define GM_LEVEL_DIFF_MOD_L2 0.25f
//Increase in 3 star time limit per level
#define GM_LEVEL_DIFF_MOD_L3 0.2f
//Cost in coins of double points powerup (standard price)
#define GM_COST_PWP_2XPTS 100
//Cost in coins of time skip powerup (standard price)
#define GM_COST_PWP_TMSKP 75

#define TICK_SPEED sf::seconds(1.0f / 60.f)

#endif