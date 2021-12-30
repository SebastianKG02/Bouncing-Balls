#include "SettingsScene.h"

SettingsScene::SettingsScene(uint8_t id, std::string name) {
	this->sceneID = id;
	this->name = name;
	this->active = false;
}

void SettingsScene::init() {
	lock();
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	resize = false;
	//Set up easy centering variable
	centerX = Config::user_resolution.X() / 2;

	//Background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));

	//Title text
	text.push_back(new sf::Text(std::string("Settings"), *AssetManager::getFont("title"), 75));
	//Set real position
	text[0]->setPosition(centerX - (text[0]->getGlobalBounds().width / 2), 100);
	text[0]->setFillColor(sf::Color::Black);

	//Add button & button label
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"),new float[2]{ 0.1f, 0.1f }));
	ui[0]->getSprite()->setPosition(centerX - (ui[0]->getSprite()->getGlobalBounds().width / 2), (600 * Config::user_resolution.diffY()));
	text.push_back(new sf::Text(std::string("Back"), *AssetManager::getFont("title"), 34));
	text[1]->setPosition(centerX - (text[1]->getGlobalBounds().width / 2), (600 * Config::user_resolution.diffY()) + (text[1]->getGlobalBounds().height / 2));
	text[1]->setFillColor(sf::Color::Black);
}

void SettingsScene::update(sf::RenderWindow* w) {
	//Check for screen res update
	if (resize == false) {
		sprites[0]->setScale(w->getSize().x / sprites[0]->getGlobalBounds().width, w->getSize().y / sprites[0]->getGlobalBounds().height);
		resize = true;
	}

	//Update UI
	for (auto ui_e : ui) {
		ui_e->update(w);
	}

	//Check if "back" button is pressed
	if (*ui[0]->getState() == UIState::CLICK) {
		SceneManager::setNext(0);
		SceneManager::next();
	}
}

void SettingsScene::input() {

}