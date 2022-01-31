#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "Config.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "UIButton.h"
#include "PlayerData.h"
#include <chrono>
#include <ctime>  
#include <iomanip>
#include <time.h>

static Config config = Config("config.cfg");
static sf::Clock gm_clock;
static sf::Time last = sf::Time::Zero;

//Main method
int main() {

	/*
	char buf[32];
	//Re-direct standard output
	std::time_t current_date = std::time(nullptr);
	std::ofstream out("mama.txt");
	std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
	*/
	//Initalise AssetManager
	AssetManager::init();

	//Initalise window
	sf::RenderWindow window(sf::VideoMode(config.user_resolution.X(), config.user_resolution.Y()), "Bouncing Balls");
	//Define window event handle
	sf::Event windowEvent;

	config.loadFromConfig("config.cfg");
	//Apply settings for window
	config.applySettings(&window);

	window.setIcon(512, 512, AssetManager::getTexture("ball_royal_blue")->copyToImage().getPixelsPtr());

	Player::init();

	//Initalise scenemanager
	SceneManager::init();
	SceneManager::next();
	//Player::init();
	//While window is open, check for user input, update game logic and draw
	while (window.isOpen()) {
		//Re-seed random number generator
		srand(time(NULL));

		last += gm_clock.restart();
		//Check for user input & events
		while (window.pollEvent(windowEvent)) {
			SceneManager::input(&windowEvent);
			if (windowEvent.type == sf::Event::Closed) {
				Player::save();
				window.close();
			}

		}

		while (last > TICK_SPEED) {
			while (window.pollEvent(windowEvent)) {
				SceneManager::input(&windowEvent);
				if (windowEvent.type == sf::Event::Closed) {
					Player::save();
					window.close();
				}

			}
			last -= TICK_SPEED;
			//Main game loop, update current scene
			SceneManager::update(&window);
		}

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
	Player::save();
	AssetManager::cleanup();
	//Close as expected
	return 0;
}