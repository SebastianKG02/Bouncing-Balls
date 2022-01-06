#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "Config.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "UIButton.h"

static Config config = Config("config.cfg");

//Main method
int main() {

	//Initalise AssetManager
	AssetManager::init();

	//Initalise window
	sf::RenderWindow window(sf::VideoMode(config.user_resolution.X(), config.user_resolution.Y()), "Bouncing Balls");
	//Define window event handle
	sf::Event windowEvent;

	//Apply settings for window
	config.applySettings(&window);

	window.setIcon(701, 701, AssetManager::getTexture("ball_blue")->copyToImage().getPixelsPtr());

	//Initalise scenemanager
	SceneManager::init();
	//Go to default scene
	SceneManager::next();

	//While window is open, check for user input, update game logic and draw
	while (window.isOpen()) {
		//Re-seed random number generator
		srand(time(NULL));

		//Check for user input & events
		while (window.pollEvent(windowEvent)) {
			SceneManager::input(&windowEvent);
			if (windowEvent.type == sf::Event::Closed) {
				window.close();
			}

		}

		if (sf::Keyboard::isKeyPressed(config.user_key_exit)) {
			window.close();
		}

		//Main game loop, update current scene
		SceneManager::update(&window);

		//Clear the window
		window.clear();

		//Draw current scene
		SceneManager::draw(&window);
		//Display what has been draw for this frame
		window.display();
	}

	//Close the window & release resources
	window.close();

	SceneManager::cleanup();

	//Close as expected
	return 0;
}