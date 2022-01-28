#include "CampLoseScene.h"
#include "PlayerData.h"

CampLoseScene::CampLoseScene(int id, std::string name) {
	this->sceneID = id;
	this->name = name;
}

void CampLoseScene::init() {
	//Lock scene during initalisaton (unavailable for input / drawing)
	lock();
	clock = sf::Clock();
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	resize = false;
	//Set up easy centering variable
	centerX = Config::user_resolution.X() / 2;

	//Background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));
	//Add back (main menu) button & label
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.15f, 0.1f }));
	ui[0]->getSprite()->setPosition(centerX - (ui[0]->getSprite()->getGlobalBounds().width / 2), Config::user_resolution.Y() * 0.7f);
	text.push_back(new sf::Text(std::string("Back to Main Menu"), *AssetManager::getFont("title"), 30));
	text[0]->setPosition(ui[0]->getSprite()->getPosition().x + (ui[0]->getSprite()->getGlobalBounds().width / 2) - (text[0]->getGlobalBounds().width / 2), ui[0]->getSprite()->getPosition().y + (ui[0]->getSprite()->getGlobalBounds().height / 2) - (text[0]->getGlobalBounds().height / 2));
	text[0]->setFillColor(sf::Color::Black);

	//Add back (level select) button & label
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.15f, 0.1f }));
	ui[1]->getSprite()->setPosition(centerX - (ui[1]->getSprite()->getGlobalBounds().width / 2), ui[0]->getSprite()->getPosition().y + ui[0]->getSprite()->getGlobalBounds().height + 2.5f);
	text.push_back(new sf::Text(std::string("Back to Level Select"), *AssetManager::getFont("title"), 30));
	text[1]->setPosition((ui[1]->getSprite()->getPosition().x + (ui[1]->getSprite()->getGlobalBounds().width / 2)) - (text[1]->getGlobalBounds().width / 2), (ui[1]->getSprite()->getPosition().y + (ui[1]->getSprite()->getGlobalBounds().height / 2)) - (text[1]->getGlobalBounds().height / 2));
	text[1]->setFillColor(sf::Color::Black);

	//Victory indicator
	text.push_back(new sf::Text(std::string("Defeat."), *AssetManager::getFont("title"), 150));
	text[2]->setPosition(centerX - (text[2]->getGlobalBounds().width / 2), (Config::user_resolution.Y() * 0.2f));
	text[2]->setFillColor(sf::Color::Black);
	text[2]->setOutlineThickness(2.5f);
	text[2]->setStyle(sf::Text::Bold);
	text[2]->setOutlineColor(sf::Color::White);

	//Level notifier
	text.push_back(new sf::Text(std::string("Level " + std::to_string(Player::getData()->last_level + 1) + " Failed."), *AssetManager::getFont("title"), 56));
	text[3]->setPosition(centerX - (text[3]->getGlobalBounds().width / 2), (Config::user_resolution.Y() * 0.25f) + (text[2]->getGlobalBounds().height) - (text[3]->getGlobalBounds().height / 2));
	text[3]->setFillColor(sf::Color::Black);
	text[3]->setOutlineThickness(1.5f);
	text[3]->setOutlineColor(sf::Color::White);

	//Amount of stars gained identifier
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("0_stars")));
	sprites[1]->setPosition((text[3]->getPosition().x + (text[3]->getGlobalBounds().width / 2)) - (sprites[1]->getGlobalBounds().width / 2), text[3]->getPosition().y + text[3]->getGlobalBounds().height + 2.5f);

	ui[0]->lock();
	ui[1]->lock();
	clock.restart();
}

void CampLoseScene::update(sf::RenderWindow* w) {
	//Check for screen res update
	if (resize == false) {
		sprites[0]->setPosition(0, 0);
		sprites[0]->setScale(Config::user_resolution.X() / sprites[0]->getGlobalBounds().width, Config::user_resolution.Y() / sprites[0]->getGlobalBounds().height);
		resize = true;
	}

	//Update UI
	for (auto ui_e : ui) {
		ui_e->update(w);
	}

	//If back (main menu) button pressed
	if (*ui[0]->getState() == UIState::CLICK) {
		clock.restart();
		ui[0]->lock();
		this->lock();
		SceneManager::setNext(0);
		SceneManager::next();
	}
	else if (*ui[0]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[0]->unlock();
		}
	}

	//If back (level select) button pressed
	if (*ui[1]->getState() == UIState::CLICK) {
		clock.restart();
		ui[1]->lock();
		this->lock();
		SceneManager::setNext(8);
		SceneManager::next();
	}
	else if (*ui[1]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[1]->unlock();
		}
	}
}

void CampLoseScene::input(sf::Event* e) {

}