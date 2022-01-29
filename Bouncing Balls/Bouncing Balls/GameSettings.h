#pragma once
#ifndef SCN_PLAY_SETTINGS
#define SCN_PLAY_SETTINGS

/*
Structure holding four primitive game settings used for map generation and logic control
*/
struct GameSettings {
	//Number of rows (default: 10)
	int rows = 6;

	//Number of columns (default: 10)
	int cols = 10;

	//Minimum number of colours (default: 2)
	int minColours = 2;

	//Maximum number of colours (default: 2)
	int maxColours = 2;

	//Level difficulty descriptor (default: 0)
	int level = 0;

	//Level 2nd star time limit (defautlt: 120s)
	int time_l2 = 80;

	//Level 3rd star time limit (default: 100s)
	int time_l3 = 60;

	//Completion flag for level 1 objecive
	bool obj_l1 = false;

	//Completion flag for level 2 objecive
	bool obj_l2 = false;

	//Completion flag for level 3 objecive
	bool obj_l3 = false;

	//Final score holder
	long final_score = 0;
};

#endif

