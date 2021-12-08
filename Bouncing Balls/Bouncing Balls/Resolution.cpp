#include "Resolution.h"

//Simple constructor to assign X and Y
Resolution::Resolution(int screenX, int screenY) {
	screen_X = screenX;
	screen_Y = screenY;
	updateName();
}

//Destructor for reource management
Resolution::~Resolution() {
	screen_X = NULL;
	screen_Y = NULL;
	resName.clear();
}

//Simple get for X res (width)
int Resolution::X() {
	return screen_X;
}

//Simple get for Y res (height)
int Resolution::Y() {
	return screen_Y;
}

//Simple function to update screen size
void Resolution::update(int screenX, int screenY) {
	screen_X = screenX;
	screen_Y = screenY;
	updateName();
}

//Function to only update X
void Resolution::updateX(int screenX) {
	screen_X = screenX;
	updateName();
}

//Function to only update Y
void Resolution::updateY(int screenY) {
	screen_Y = screenY;
	updateName();
}

//Get reference name
std::string Resolution::name() {
	return resName;
}
//Private name variable update function
//Casts both X and Y into string, concat with "x" separator and assign back to resName
void Resolution::updateName() {
	resName = std::to_string(screen_X) + "x" + std::to_string(screen_Y);
}