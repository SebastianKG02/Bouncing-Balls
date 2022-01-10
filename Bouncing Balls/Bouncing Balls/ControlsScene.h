#pragma once
#include "SceneManager.h"

#ifndef SCN_CONTROLS
#define SCN_CONTROLS

class ControlsScene : public Scene
{
public:
	ControlsScene(int id, std::string name);
	void init();
	void update(sf::RenderWindow* w);
	void input(sf::Event* e);
private:
	sf::Clock clock;
	bool resize = false;
	float centerX = 0.f;
	bool changing[5] = { false, false, false, false, false };
	bool render_square = false;
	bool complete = false;
	bool keyReady = false;
	sf::Keyboard::Key key;
};

#endif SCN_CONTROLS