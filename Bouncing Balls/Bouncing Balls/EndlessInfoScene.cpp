#include "EndlessInfoScene.h"

EndlessInfoScene::EndlessInfoScene(int id, std::string name) {
	this->sceneID = id;
	this->name = name;
}

void EndlessInfoScene::init() {
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

	//Title text
	text.push_back(new sf::Text(std::string("Endless"), *AssetManager::getFont("title"), 75));
	//Set real position
	text[0]->setPosition(centerX - (text[0]->getGlobalBounds().width / 2), Config::applyRDY(100));
	text[0]->setFillColor(sf::Color::Black);
	text[0]->setOutlineColor(sf::Color::White);
	text[0]->setOutlineThickness(3.5f);

	//Add back button & label
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[0]->getSprite()->setPosition(centerX - (ui[0]->getSprite()->getGlobalBounds().width / 2), Config::applyRDY(580));
	text.push_back(new sf::Text(std::string("Back"), *AssetManager::getFont("title"), 34));
	text[1]->setPosition(centerX - (text[1]->getGlobalBounds().width / 2), Config::applyRDY(580) + (text[1]->getGlobalBounds().height / 2));
	text[1]->setFillColor(sf::Color::Black);

	//Continue button
	ui.push_back(new UIButton(new float[2]{ 0.f, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[1]->getSprite()->setPosition(ui[0]->getSprite()->getPosition());
	ui[1]->getSprite()->move(0.f, -(ui[1]->getSprite()->getGlobalBounds().height) - Config::applyRDY(25));
	//Continue button label
	text.push_back(new sf::Text(std::string("Continue"), *AssetManager::getFont("title"), 34));
	text[2]->setPosition(ui[1]->getSprite()->getPosition());
	text[2]->move((ui[1]->getSprite()->getGlobalBounds().width / 2) - text[2]->getGlobalBounds().width / 2, (ui[1]->getSprite()->getGlobalBounds().height / 2) - (text[2]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[2]->setFillColor(sf::Color::Black);

	text.push_back(new sf::Text(std::string("'Bouncing Balls' but forever!"), *AssetManager::getFont("title"), 34));
	text[3]->setPosition(centerX - (text[3]->getGlobalBounds().width / 2), text[0]->getPosition().y + text[0]->getGlobalBounds().height + (text[3]->getGlobalBounds().height) + Config::applyRDY(10));
	text[3]->setFillColor(sf::Color::Black);

	text.push_back(new sf::Text(std::string("Try for your new highscore!"), *AssetManager::getFont("title"), 34));
	text[4]->setPosition(centerX - (text[4]->getGlobalBounds().width / 2), text[3]->getPosition().y + text[3]->getGlobalBounds().height + (text[4]->getGlobalBounds().height) - Config::applyRDY(5));
	text[4]->setFillColor(sf::Color::Black);

	text.push_back(new sf::Text(std::string("More balls & up to 17 colours!"), *AssetManager::getFont("title"), 34));
	text[5]->setPosition(centerX - (text[5]->getGlobalBounds().width / 2), text[4]->getPosition().y + text[4]->getGlobalBounds().height + (text[5]->getGlobalBounds().height) - Config::applyRDY(5));
	text[5]->setFillColor(sf::Color::Black);

	text.push_back(new sf::Text(std::string("Progressively gets faster."), *AssetManager::getFont("title"), 34));
	text[6]->setPosition(centerX - (text[6]->getGlobalBounds().width / 2), text[5]->getPosition().y + text[5]->getGlobalBounds().height + (text[6]->getGlobalBounds().height) - Config::applyRDY(5));
	text[6]->setFillColor(sf::Color::Black);

	text.push_back(new sf::Text(std::string("Too easy? Try Challenge mode!"), *AssetManager::getFont("title"), 34));
	text[7]->setPosition(centerX - (text[7]->getGlobalBounds().width / 2), text[6]->getPosition().y + text[6]->getGlobalBounds().height + (text[7]->getGlobalBounds().height) - Config::applyRDY(5));
	text[7]->setFillColor(sf::Color::Black);

	ui[0]->lock();
	ui[1]->lock();
	//Unlock scene and get ready to function as complete scene
	unlock();
}

void EndlessInfoScene::update(sf::RenderWindow* w) {
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

	//If back button pressed
	if (*ui[0]->getState() == UIState::CLICK) {
		clock.restart();
		ui[0]->lock();
		this->lock();
		SceneManager::setNext(3);
		SceneManager::next();
	}
	else if (*ui[0]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[0]->unlock();
		}
	}

	//If continue button pressed
	if (*ui[1]->getState() == UIState::CLICK) {
		clock.restart();
		ui[1]->lock();
		this->lock();
		SceneManager::setNext(12);
		SceneManager::next();
	}
	else if (*ui[1]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[1]->unlock();
		}
	}
}

void EndlessInfoScene::input(sf::Event* e) {

}