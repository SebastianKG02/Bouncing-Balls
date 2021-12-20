#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "Config.h"
#include "AssetManager.h"
#include "SceneManager.h"

//Main method
int main() {
	//Load user config
	Config config = Config("config.cfg");

	//Initalise AssetManager
	AssetManager::init();

	//Initalise window
	sf::RenderWindow window(sf::VideoMode(config.user_resolution.X(), config.user_resolution.Y()), "Bouncing Balls");
	//Define window event handle
	sf::Event windowEvent;

	if (config.user_do_frame_limit == true) {
		window.setFramerateLimit(config.user_frame_limit_val);
	}
	window.setVerticalSyncEnabled(config.user_do_vsync);

	//Initalise scenemanager
	SceneManager::init();
	//Go to default scene
	SceneManager::next();

	//While window is open, check for user input, update game logic and draw
	while (window.isOpen()) {
		//Check for user input & events
		while (window.pollEvent(windowEvent)) {
			if (windowEvent.type == sf::Event::Closed) {
				window.close();
			}

		}

		if (sf::Keyboard::isKeyPressed(config.user_key_exit)) {
			window.close();
		}

		//Main game loop, process input for current scene, then update current scene
		SceneManager::input();
		SceneManager::update();

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