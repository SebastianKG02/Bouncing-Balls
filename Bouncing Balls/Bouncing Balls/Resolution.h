#pragma once

#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <string>
#include "Defines.h"
//Simple class to contain resolution data and a quick-fire name reference
class Resolution
{
public:
	//Constructor
	Resolution(int screenX=DEFAULT_SCREEN_X, int screenY=DEFAULT_SCREEN_Y);
	Resolution(std::string res);
	//Destructor
	~Resolution();
	//Get for X (width)
	int X();
	//Get for Y (height)
	int Y();
	//Update both X and Y
	void update(int screenX, int screenY);
	void update(std::string res);
	//Update only X
	void updateX(int screenX);
	//Update only Y
	void updateY(int screenY);
	//Get for resolution reference name
	std::string name();
private:
	//Screen width (cannot be negative as window would not init)
	signed short screen_X;
	//Screen height
	signed short screen_Y;
	//Resolution reference name
	std::string resName;
	//Function to convert both X&Y into string and concat, updating resName
	void updateName();

};

#endif

