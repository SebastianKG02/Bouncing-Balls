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
#define GM_MIN_CANNON_ROT -45.f
#define GM_MAX_CANNON_ROT 45.f

#define GM_KEYBOARD_MOVE_SPEED 0.05f
#define GM_BALL_SHOOT_SPEED 0.2f
#define GM_BALL_MOVE_SPEED 0.0025f * 4 
//delay as multiple of a second
#define GM_BALL_SHOOT_DELAY  0.5f

#define TICK_SPEED sf::seconds(1.0f / 60.f)

#endif