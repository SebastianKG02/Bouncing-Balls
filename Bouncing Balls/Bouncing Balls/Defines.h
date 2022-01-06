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

#define DB_SHOW_HITBOXES true

#endif