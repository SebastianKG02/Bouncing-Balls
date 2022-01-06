#include "DefaultScene.h"
#include "UIButton.h"
#include "Config.h"

//Initalise scene
void DefaultScene::init() {
	lock();
	counter = 0;
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	resize = false;
	//Set up easy centering variable
	centerX = Config::user_resolution.X() / 2;
	//Title text
	text.push_back(new sf::Text(std::string("Bouncing Balls"), *AssetManager::getFont("title"), 75));
	//Set real position
	text[0]->setPosition(centerX-(text[0]->getGlobalBounds().width/2), 100);
	text[0]->setFillColor(sf::Color::Black);
	text[0]->setOutlineColor(sf::Color::White);
	text[0]->setOutlineThickness(3.5f);

	//Background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));

	//Play button
	ui.push_back(new UIButton(centerX, 200, "btn_long_active", 0.1f, 0.1f));
	ui[0]->getSprite()->setPosition(centerX - (ui[0]->getSprite()->getGlobalBounds().width / 2), 200);
	ui[0]->setStateTex(UIState::ACTIVE, "btn_long_active");
	ui[0]->setStateTex(UIState::HOVER, "btn_long_hover");
	ui[0]->setStateTex(UIState::CLICK, "btn_long_click");
	ui[0]->setStateTex(UIState::INACTIVE, "btn_long_lock");
	text.push_back(new sf::Text(std::string("Play"), *AssetManager::getFont("title"), 34));
	text[1]->setPosition(centerX - (text[1]->getGlobalBounds().width / 2), 200+(text[1]->getGlobalBounds().height/2));
	text[1]->setFillColor(sf::Color::Black);

	//Achievements button
	ui.push_back(new UIButton(centerX, 300, "btn_long_active", 0.1f, 0.1f));
	ui[1]->getSprite()->setPosition(centerX - (ui[1]->getSprite()->getGlobalBounds().width / 2), 300);
	ui[1]->setStateTex(UIState::ACTIVE, "btn_long_active");
	ui[1]->setStateTex(UIState::HOVER, "btn_long_hover");
	ui[1]->setStateTex(UIState::CLICK, "btn_long_click");
	ui[1]->setStateTex(UIState::INACTIVE, "btn_long_lock");
	text.push_back(new sf::Text(std::string("Achievements"), *AssetManager::getFont("title"), 34));
	text[2]->setPosition(centerX - (text[2]->getGlobalBounds().width / 2), 300 + (text[2]->getGlobalBounds().height / 2));
	text[2]->setFillColor(sf::Color::Black);

	//Settings button
	ui.push_back(new UIButton(centerX, 400, "btn_long_active", 0.1f, 0.1f));
	ui[2]->getSprite()->setPosition(centerX - (ui[2]->getSprite()->getGlobalBounds().width / 2), 400);
	ui[2]->setStateTex(UIState::ACTIVE, "btn_long_active");
	ui[2]->setStateTex(UIState::HOVER, "btn_long_hover");
	ui[2]->setStateTex(UIState::CLICK, "btn_long_click");
	ui[2]->setStateTex(UIState::INACTIVE, "btn_long_lock");
	text.push_back(new sf::Text(std::string("Settings"), *AssetManager::getFont("title"), 34));
	text[3]->setPosition(centerX - (text[3]->getGlobalBounds().width / 2), 400 + (text[3]->getGlobalBounds().height / 2));
	text[3]->setFillColor(sf::Color::Black);

	//Exit button
	ui.push_back(new UIButton(centerX, 500, "btn_long_active", 0.1f, 0.1f));
	ui[3]->getSprite()->setPosition(centerX - (ui[3]->getSprite()->getGlobalBounds().width / 2), 500);
	ui[3]->setStateTex(UIState::ACTIVE, "btn_long_active");
	ui[3]->setStateTex(UIState::HOVER, "btn_long_hover");
	ui[3]->setStateTex(UIState::CLICK, "btn_long_click");
	ui[3]->setStateTex(UIState::INACTIVE, "btn_long_lock");
	text.push_back(new sf::Text(std::string("Exit"), *AssetManager::getFont("title"), 34));
	text[4]->setPosition(centerX - (text[4]->getGlobalBounds().width / 2), 500 + (text[4]->getGlobalBounds().height / 2));
	text[4]->setFillColor(sf::Color::Black);
	ui[3]->lock();

	//Dynamic balls
	for (int i = 1; i < 100; i++) {
		sprites.push_back(new sf::Sprite(*AssetManager::getTexture("ball_blue")));
		sprites[i]->setScale(0.05f, 0.05f);
		sprites[i]->setColor(sf::Color(255, 255, 500, 255));
		sprites[i]->setPosition(rand() % (Config::user_resolution.X()-100),  (-100 - rand()%600));
	}
}

void DefaultScene::update(sf::RenderWindow* w) {
	//Resize
	if (resize == false) {
		sprites[0]->setScale(w->getSize().x / sprites[0]->getGlobalBounds().width, w->getSize().y / sprites[0]->getGlobalBounds().height);
		resize = true;
	}
	
	//Title bouncing mechanic
	if (text[0]->getPosition().y <= maxBound && minMax == false) {
		text[0]->move(0, angleDelta);
		if (text[0]->getPosition().y >= maxBound) {
			minMax = true;
		}
	}
	else if (text[0]->getPosition().y >= minBound && minMax == true) {
		text[0]->move(0, -angleDelta);
		if (text[0]->getPosition().y <= minBound) {
			minMax = false;
		}
	}

	//Falling balls mechanic
	for (int i = 1; i < 100; i++) {
		if (sprites[i]->getPosition().y < Config::user_resolution.Y()) {
			float vel_Y = (angleDelta) * 25;

			if ((rand() % 2) > 0) {
				sprites[i]->move(angleDelta * (1.5f * (rand() % 10)), vel_Y);
			}
			else {
				sprites[i]->move(-angleDelta * (1.5f * (rand() % 10)), vel_Y);
			}
			sprites[i]->rotate(angleDelta);
			//balls[i-1]->y -= (10 * angleDelta);
		}
		else {
			sprites[i]->setPosition(50+(rand() % (Config::user_resolution.X()-100)), (-100) - (rand() % 200));
		}
		//sprites[i]->setPosition(*balls[i]);
	}

	//Update UI
	for (auto ui_e : ui) {
		ui_e->update(w);
	}

	//If settings button is clicked, switch to settings screen
	if (*ui[2]->getState() == UIState::CLICK) {
		SceneManager::setNext(1);
		SceneManager::next();
	}

	//If exit button is clicked, exit game
	if (SceneManager::getCurrentSceneID() == sceneID){
		if (*ui[3]->getState() == UIState::LOCK) {
			counter++;
			if (counter > 1000) {
				ui[3]->unlock();
			}
			else {
				ui[3]->lock();
			}
		}
		if (*ui[3]->getState() == UIState::CLICK) {
			w->close();
		}
	}
}

void DefaultScene::input(sf::Event* e) {

}

DefaultScene::DefaultScene(uint8_t id, std::string name) {
	this->sceneID = id;
	this->name = name;
	this->active = false;
}