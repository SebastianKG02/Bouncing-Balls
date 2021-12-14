#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "Config.h"
#include "AssetManager.h"

//Main method
int main() {
	//Initalise window
	sf::RenderWindow window(sf::VideoMode(720, 480), "Bouncing Balls");
	//Define window event handle
	sf::Event windowEvent;

	//Load user config
	Config config = Config("config.cfg");

	//Initalise AssetManager
	AssetManager::init();

	//Testing
	sf::Sprite test;
	test.setTexture(*AssetManager::getTexture("0_stars"));
	test.setPosition(300, 200);

	sf::Sound sound;
	sound.setBuffer(*AssetManager::getSound("coin"));
	sound.setVolume(100.0f);

	sf::Text text;
	text.setFont(*AssetManager::getFont("title"));
	text.setString("Siema boys");
	text.setPosition(25, 25);
	text.setCharacterSize(30);

	AssetManager::getMusic("bass")->setVolume(100.f);
	AssetManager::getMusic("bass")->setLoop(true);
	

	//ConfigFile cf = ConfigFile("res/tex.cfg");

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

		if (sf::Keyboard::isKeyPressed(config.user_key_left)) {
			if (sound.getStatus() != sf::Sound::Status::Playing) {
				sound.play();
			}
		}

		if (sf::Keyboard::isKeyPressed(config.user_key_right)) {
			AssetManager::getMusic("bass")->setLoop(!AssetManager::getMusic("bass")->getLoop());
			
			if (AssetManager::getMusic("bass")->getLoop() == true) {
				AssetManager::getMusic("bass")->play();
			}
		}

		//Clear the window
		window.clear();

		window.draw(test);
		window.draw(text);

		//Display what has been draw for this frame
		window.display();
	}

	//Close the window & release resources
	window.close();

	//Close as expected
	return 0;
}