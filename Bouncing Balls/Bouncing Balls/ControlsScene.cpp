#include "ControlsScene.h"

ControlsScene::ControlsScene(int id, std::string name) {
	this->sceneID = id;
	this->name = name;
}

void ControlsScene::init() {
	//Default initaliser for SceneManager
	lock();
	for (auto& v : changing) {
		v = false;
	}
	//Set up clock for button timings
	clock = sf::Clock();
	//Set up maps
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	shapes = std::vector<sf::Shape*>();
	resize = false;
	//Set up easy centering variable
	centerX = Config::user_resolution.X() / 2;
	//Add background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));

	//Add title text (top of scene)
	text.push_back(new sf::Text("Controls", *AssetManager::getFont("title"), 75));
	text[0]->setPosition(centerX - (text[0]->getGlobalBounds().width / 2), 100);
	text[0]->setOutlineColor(sf::Color::White);
	text[0]->setOutlineThickness(3.5f);
	text[0]->setFillColor(sf::Color::Black);

	//Add back button
	ui.push_back(new UIButton(new float[2]{ centerX, 0 }, "long", new float[2]{ 0.1f, 0.1f }));
	ui[0]->getSprite()->move(-(ui[0]->getSprite()->getGlobalBounds().width / 2), (580));
	text.push_back(new sf::Text(std::string("Back"), *AssetManager::getFont("title"), 34));
	text[1]->setPosition(centerX - (text[1]->getGlobalBounds().width / 2), (580) + (text[1]->getGlobalBounds().height / 2));
	text[1]->setFillColor(sf::Color::Black);

	//Escape key label
	text.push_back(new sf::Text(std::string("Key to exit: "), *AssetManager::getFont("title"), 25));
	text[2]->setPosition(centerX - (text[2]->getGlobalBounds().width), text[0]->getPosition().y + text[0]->getGlobalBounds().height + text[1]->getGlobalBounds().height + Config::applyRDY(5));
	text[2]->setFillColor(sf::Color::Black);
	//Escape key value
	text.push_back(new sf::Text(std::string("< " + Utils::keyToStr(Config::user_key_exit) + " >"), *AssetManager::getFont("title"), 25));
	text[3]->setPosition(centerX + Config::applyRDX(25), text[2]->getPosition().y);
	text[3]->setFillColor(sf::Color::Black);
	//Escape key change button
	ui.push_back(new UIButton(new float[2]{ text[0]->getPosition().x+text[0]->getGlobalBounds().width, text[3]->getPosition().y }, "long", new float[2]{ 0.05f, 0.05f }));
	ui[1]->getSprite()->move(Config::applyRDX(25), -Config::applyRDY(text[2]->getGlobalBounds().height/2));
	//Escape key change button label
	text.push_back(new sf::Text(std::string("Modify"), *AssetManager::getFont("title"), 24));
	text[4]->setPosition(ui[1]->getSprite()->getPosition().x + (ui[1]->getSprite()->getGlobalBounds().width/2) - (text[4]->getGlobalBounds().width/2), ui[1]->getSprite()->getPosition().y + (ui[1]->getSprite()->getGlobalBounds().height / 2) - (text[4]->getGlobalBounds().height / 2));
	text[4]->setFillColor(sf::Color::Black);

	//Cannon left key label
	text.push_back(new sf::Text(std::string("Move cannon to the Left key: "), *AssetManager::getFont("title"), 25));
	text[5]->setPosition(centerX - (text[5]->getGlobalBounds().width), text[2]->getPosition().y + text[2]->getGlobalBounds().height + text[5]->getGlobalBounds().height + Config::applyRDY(5));
	text[5]->setFillColor(sf::Color::Black);
	//Cannon left key value
	text.push_back(new sf::Text(std::string("< " + Utils::keyToStr(Config::user_key_left) + " >"), *AssetManager::getFont("title"), 25));
	text[6]->setPosition(centerX + Config::applyRDX(25), text[5]->getPosition().y);
	text[6]->setFillColor(sf::Color::Black);
	//Canon left key change button
	ui.push_back(new UIButton(new float[2]{ text[0]->getPosition().x+text[0]->getGlobalBounds().width, text[6]->getPosition().y }, "long", new float[2]{ 0.05f, 0.05f }));
	ui[2]->getSprite()->move(Config::applyRDX(25), 0);
	//Cannon left key change button label
	text.push_back(new sf::Text(std::string("Modify"), *AssetManager::getFont("title"), 24));
	text[7]->setPosition(ui[2]->getSprite()->getPosition().x + (ui[2]->getSprite()->getGlobalBounds().width / 2) - (text[7]->getGlobalBounds().width / 2), ui[2]->getSprite()->getPosition().y + (ui[2]->getSprite()->getGlobalBounds().height / 2) - (text[7]->getGlobalBounds().height / 2));
	text[7]->setFillColor(sf::Color::Black);

	//Cannon right key label
	text.push_back(new sf::Text(std::string("Move cannon to the Right key: "), *AssetManager::getFont("title"), 25));
	text[8]->setPosition(centerX - (text[8]->getGlobalBounds().width), text[5]->getPosition().y + text[5]->getGlobalBounds().height + text[8]->getGlobalBounds().height + Config::applyRDY(5));
	text[8]->setFillColor(sf::Color::Black);
	//Cannon right key value
	text.push_back(new sf::Text(std::string("< " + Utils::keyToStr(Config::user_key_right) + " >"), *AssetManager::getFont("title"), 25));
	text[9]->setPosition(centerX + Config::applyRDX(25), text[8]->getPosition().y);
	text[9]->setFillColor(sf::Color::Black);
	//Canon right key change button
	ui.push_back(new UIButton(new float[2]{ text[0]->getPosition().x+text[0]->getGlobalBounds().width, text[9]->getPosition().y }, "long", new float[2]{ 0.05f, 0.05f }));
	ui[3]->getSprite()->move(Config::applyRDX(25), 0);
	//Cannon right key change button label
	text.push_back(new sf::Text(std::string("Modify"), *AssetManager::getFont("title"), 24));
	text[10]->setPosition(ui[3]->getSprite()->getPosition().x + (ui[3]->getSprite()->getGlobalBounds().width / 2) - (text[10]->getGlobalBounds().width / 2), ui[3]->getSprite()->getPosition().y + (ui[3]->getSprite()->getGlobalBounds().height / 2) - (text[10]->getGlobalBounds().height / 2));
	text[10]->setFillColor(sf::Color::Black);

	//Shoot key label
	text.push_back(new sf::Text(std::string("Shoot ball key: "), *AssetManager::getFont("title"), 25));
	text[11]->setPosition(centerX - (text[11]->getGlobalBounds().width), text[8]->getPosition().y + text[8]->getGlobalBounds().height + text[11]->getGlobalBounds().height + Config::applyRDY(5));
	text[11]->setFillColor(sf::Color::Black);
	//Shoot key value
	text.push_back(new sf::Text(std::string("< " + Utils::keyToStr(Config::user_key_shoot) + " >"), *AssetManager::getFont("title"), 25));
	text[12]->setPosition(centerX + Config::applyRDX(25), text[11]->getPosition().y);
	text[12]->setFillColor(sf::Color::Black);
	//Canon right key change button
	ui.push_back(new UIButton(new float[2]{ text[0]->getPosition().x+text[0]->getGlobalBounds().width, text[12]->getPosition().y }, "long", new float[2]{ 0.05f, 0.05f }));
	ui[4]->getSprite()->move(Config::applyRDX(25), 0);
	//Shoot key change button label
	text.push_back(new sf::Text(std::string("Modify"), *AssetManager::getFont("title"), 24));
	text[13]->setPosition(ui[4]->getSprite()->getPosition().x + (ui[4]->getSprite()->getGlobalBounds().width / 2) - (text[13]->getGlobalBounds().width / 2), ui[4]->getSprite()->getPosition().y + (ui[4]->getSprite()->getGlobalBounds().height / 2) - (text[13]->getGlobalBounds().height / 2));
	text[13]->setFillColor(sf::Color::Black);

	//Pause key label
	text.push_back(new sf::Text(std::string("Pause: "), *AssetManager::getFont("title"), 25));
	text[14]->setPosition(centerX - (text[14]->getGlobalBounds().width), text[11]->getPosition().y + text[11]->getGlobalBounds().height + text[14]->getGlobalBounds().height + Config::applyRDY(5));
	text[14]->setFillColor(sf::Color::Black);
	//Pause key value
	text.push_back(new sf::Text(std::string("< " + Utils::keyToStr(Config::user_key_pause) + " >"), *AssetManager::getFont("title"), 25));
	text[15]->setPosition(centerX + Config::applyRDX(25), text[14]->getPosition().y);
	text[15]->setFillColor(sf::Color::Black);
	//Pause key change button
	ui.push_back(new UIButton(new float[2]{ text[0]->getPosition().x+text[0]->getGlobalBounds().width, text[15]->getPosition().y }, "long", new float[2]{ 0.05f, 0.05f }));
	ui[5]->getSprite()->move(Config::applyRDX(25), 0);
	//Pause key change button label
	text.push_back(new sf::Text(std::string("Modify"), *AssetManager::getFont("title"), 24));
	text[16]->setPosition(ui[5]->getSprite()->getPosition().x + (ui[5]->getSprite()->getGlobalBounds().width / 2) - (text[16]->getGlobalBounds().width / 2), ui[5]->getSprite()->getPosition().y + (ui[5]->getSprite()->getGlobalBounds().height / 2) - (text[16]->getGlobalBounds().height / 2));
	text[16]->setFillColor(sf::Color::Black);

	//Start key label
	text.push_back(new sf::Text(std::string("Start: "), *AssetManager::getFont("title"), 25));
	text[17]->setPosition(centerX - (text[17]->getGlobalBounds().width), text[14]->getPosition().y + text[14]->getGlobalBounds().height + text[17]->getGlobalBounds().height + Config::applyRDY(5));
	text[17]->setFillColor(sf::Color::Black);
	//Start key value
	text.push_back(new sf::Text(std::string("< " + Utils::keyToStr(Config::user_key_start) + " >"), *AssetManager::getFont("title"), 25));
	text[18]->setPosition(centerX + Config::applyRDX(25), text[17]->getPosition().y);
	text[18]->setFillColor(sf::Color::Black);
	//Start key change button
	ui.push_back(new UIButton(new float[2]{ text[0]->getPosition().x + text[0]->getGlobalBounds().width, text[18]->getPosition().y }, "long", new float[2]{ 0.05f, 0.05f }));
	ui[6]->getSprite()->move(Config::applyRDX(25), 0);
	//Start key change button label
	text.push_back(new sf::Text(std::string("Modify"), *AssetManager::getFont("title"), 24));
	text[19]->setPosition(ui[6]->getSprite()->getPosition().x + (ui[6]->getSprite()->getGlobalBounds().width / 2) - (text[19]->getGlobalBounds().width / 2), ui[6]->getSprite()->getPosition().y + (ui[6]->getSprite()->getGlobalBounds().height / 2) - (text[19]->getGlobalBounds().height / 2));
	text[19]->setFillColor(sf::Color::Black);

	//Powerup 1 key label
	text.push_back(new sf::Text(std::string("Powerup 1: "), *AssetManager::getFont("title"), 25));
	text[20]->setPosition(centerX - (text[20]->getGlobalBounds().width), text[17]->getPosition().y + text[17]->getGlobalBounds().height + text[20]->getGlobalBounds().height + Config::applyRDY(5));
	text[20]->setFillColor(sf::Color::Black);
	//Powerup 1 key value
	text.push_back(new sf::Text(std::string("< " + Utils::keyToStr(Config::user_key_pw_1) + " >"), *AssetManager::getFont("title"), 25));
	text[21]->setPosition(centerX + Config::applyRDX(25), text[20]->getPosition().y);
	text[21]->setFillColor(sf::Color::Black);
	//Powerup 1 key change button
	ui.push_back(new UIButton(new float[2]{ text[0]->getPosition().x + text[0]->getGlobalBounds().width, text[21]->getPosition().y }, "long", new float[2]{ 0.05f, 0.05f }));
	ui[7]->getSprite()->move(Config::applyRDX(25), 0);
	//Powerup 1 key change button label
	text.push_back(new sf::Text(std::string("Modify"), *AssetManager::getFont("title"), 24));
	text[22]->setPosition(ui[7]->getSprite()->getPosition().x + (ui[7]->getSprite()->getGlobalBounds().width / 2) - (text[22]->getGlobalBounds().width / 2), ui[7]->getSprite()->getPosition().y + (ui[7]->getSprite()->getGlobalBounds().height / 2) - (text[22]->getGlobalBounds().height / 2));
	text[22]->setFillColor(sf::Color::Black);

	//Powerup 2 key label
	text.push_back(new sf::Text(std::string("Powerup 2: "), *AssetManager::getFont("title"), 25));
	text[23]->setPosition(centerX - (text[23]->getGlobalBounds().width), text[20]->getPosition().y + text[20]->getGlobalBounds().height + text[23]->getGlobalBounds().height + Config::applyRDY(5));
	text[23]->setFillColor(sf::Color::Black);
	//Powerup 2 key value
	text.push_back(new sf::Text(std::string("< " + Utils::keyToStr(Config::user_key_pw_2) + " >"), *AssetManager::getFont("title"), 25));
	text[24]->setPosition(centerX + Config::applyRDX(25), text[23]->getPosition().y);
	text[24]->setFillColor(sf::Color::Black);
	//Powerup 2 key change button
	ui.push_back(new UIButton(new float[2]{ text[0]->getPosition().x + text[0]->getGlobalBounds().width, text[24]->getPosition().y }, "long", new float[2]{ 0.05f, 0.05f }));
	ui[8]->getSprite()->move(Config::applyRDX(25), 0);
	//Powerup 2 key change button label
	text.push_back(new sf::Text(std::string("Modify"), *AssetManager::getFont("title"), 24));
	text[25]->setPosition(ui[8]->getSprite()->getPosition().x + (ui[8]->getSprite()->getGlobalBounds().width / 2) - (text[25]->getGlobalBounds().width / 2), ui[8]->getSprite()->getPosition().y + (ui[8]->getSprite()->getGlobalBounds().height / 2) - (text[25]->getGlobalBounds().height / 2));
	text[25]->setFillColor(sf::Color::Black);

	//Set up "Fade" background for setting key
	shapes.push_back(new sf::RectangleShape(sf::Vector2f(Config::user_resolution.X(), Config::user_resolution.Y())));
	shapes[0]->setFillColor(sf::Color(0, 0, 0, 0));

	//Await
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 30));
	text[26]->setFillColor(sf::Color::Red);

	//Initisalisation finished, unlock scene
	unlock();
}

void ControlsScene::update(sf::RenderWindow* w) {
	//Check for screen res update
	if (resize == false) {
		sprites[0]->setScale(w->getSize().x / sprites[0]->getGlobalBounds().width, w->getSize().y / sprites[0]->getGlobalBounds().height);
		centerX = Config::user_resolution.X() / 2;
		resize = true;
	}

	//Update UI
	for (auto ui_e : ui) {
		ui_e->update(w);
	}

	if (render_square == false) {
		//Loop through each "Modify" button
		for (int i = 1; i < 9; i++) {
			//If it is clicked
			if (*ui[i]->getState() == UIState::CLICK) {
				//Lock all buttons - including the "Back" button
				for (int j = 0; j < 9; j++) {
					ui[j]->lock();
				}
				//Set changing var by reference to which modify button is pressed
				changing[i - 1] = true;

				//If exit key is to be set
				if (i == 1) {
					text[26]->setString("Please enter new Exit key: ");
				}
				//If left move key is to be set
				else if (i == 2) {
					text[26]->setString("Please enter new Left Move key: ");
				}
				//If right move key is to be set
				else if (i == 3) {
					text[26]->setString("Please enter new Right Move key: ");
				}
				//If pause key is to be set
				else if (i == 5) {
					text[26]->setString("Please enter new Pause key: ");
				}
				//If shoot key is to be set
				else if (i == 4) {
					text[26]->setString("Please enter new Shoot key: ");
				}
				//If start key is to be set
				else if (i == 6) {
					text[26]->setString("Please enter new Start key: ");
				}
				//If powerup 1 key is to be set
				else if (i == 7) {
					text[26]->setString("Please enter new Powerup 1 key: ");
				}
				//If powerup 2 key is to be set
				else if (i == 8) {
					text[26]->setString("Please enter new Powerup 2 key: ");
				}

				text[26]->setPosition(centerX - (text[26]->getGlobalBounds().width / 2), (Config::user_resolution.Y() / 2) - (text[26]->getGlobalBounds().height / 2));
				render_square = true;
				keyReady = false;
				complete = false;

				//Restart clock
				clock.restart();
			}
			else {
				//If clock reached correct timestep, unlock ui elements
				if (clock.getElapsedTime().asMilliseconds() >= 200) {
					ui[i]->unlock();
				}
			}
		}

		//Check if "back" button is pressed
		if (*ui[0]->getState() == UIState::CLICK) {
			SceneManager::setNext(1);
			SceneManager::next();
			ui[0]->lock();
			clock.restart();
		}
		else {
			if (clock.getElapsedTime().asMilliseconds() >= 200) {
				ui[0]->unlock();
			}
		}
	}
	else {
		shapes[0]->setFillColor(sf::Color(0, 0, 0, 200));

		if (complete == false && keyReady == true) {
			for (int i = 0; i < 8; i++) {
				if (changing[i] == true) {
					switch (i) {
					case 0:
						Config::user_key_exit = key;
						Config::getSettings()->setValue("key_exit", Utils::keyToStr(key), true);
						break;
					case 1:
						Config::user_key_left = key;
						Config::getSettings()->setValue("key_left", Utils::keyToStr(key), true);
						break;
					case 2:
						Config::user_key_right = key;
						Config::getSettings()->setValue("key_right", Utils::keyToStr(key), true);
						break;
					case 4:
						Config::user_key_pause = key;
						Config::getSettings()->setValue("key_pause", Utils::keyToStr(key), true);
						break;
					case 3:
						Config::user_key_shoot = key;
						Config::getSettings()->setValue("key_shoot", Utils::keyToStr(key), true);
						break;
					case 5:
						Config::user_key_start = key;
						Config::getSettings()->setValue("key_start", Utils::keyToStr(key), true);
						break;
					case 6:
						Config::user_key_pw_1 = key;
						Config::getSettings()->setValue("key_powerup_1", Utils::keyToStr(key), true);
						break;
					case 7:
						Config::user_key_pw_2 = key;
						Config::getSettings()->setValue("key_powerup_2", Utils::keyToStr(key), true);
						break;
					}
					text[(i + 1) * 3]->setString("< " + Utils::keyToStr(key) + " >");
					changing[i] = false;
				}
			}

			complete = true;
			clock.restart();
			text[26]->setString(text[26]->getString() + Utils::keyToStr(key));
			text[26]->setPosition(centerX - (text[26]->getGlobalBounds().width / 2), (Config::user_resolution.Y() / 2) - (text[26]->getGlobalBounds().height / 2));
			Config::saveCurrentConfig("config.cfg");
		}
		else if (complete == true){
			if (clock.getElapsedTime().asMilliseconds() >= 200) {
				shapes[0]->setFillColor(sf::Color(0, 0, 0, 0));
				text[26]->setString("");
				render_square = false;
			}
		}
	}
}

void ControlsScene::input(sf::Event* e) {
	if (e->type == sf::Event::KeyPressed) {
		key = e->key.code;
		keyReady = true;
		std::cout << Utils::keyToStr(key) << std::endl;
	}
}