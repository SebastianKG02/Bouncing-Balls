#include "Resolution.h"

Resolution::Resolution(int screenX, int screenY) {
	screen_X = screenX;
	screen_Y = screenY;
	updateName();
}
Resolution::~Resolution() {
	screen_X = NULL;
	screen_Y = NULL;
	resName.clear();
}

int Resolution::X() {
	return screen_X;
}

int Resolution::Y() {
	return screen_Y;
}

void Resolution::update(int screenX, int screenY) {
	screen_X = screenX;
	screen_Y = screenY;
	updateName();
}

void Resolution::updateX(int screenX) {
	screen_X = screenX;
	updateName();
}

void Resolution::updateY(int screenY) {
	screen_Y = screenY;
	updateName();
}

std::string Resolution::name() {
	return resName;
}

void Resolution::updateName() {
	resName = std::to_string(screen_X) + "x" + std::to_string(screen_Y);
}