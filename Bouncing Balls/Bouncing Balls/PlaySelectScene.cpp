#include "PlaySelectScene.h"

//Default simple initaliser
PlaySelectScene::PlaySelectScene(int id, std::string name) {
	this->sceneID = id;
	this->name = name;
}

void PlaySelectScene::init() {
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
	text.push_back(new sf::Text(std::string("Level Select"), *AssetManager::getFont("title"), 75));
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

	//Campaign mode button & label
	ui.push_back(new UIButton(new float[2]{ centerX / 2, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[1]->getSprite()->move(-(ui[1]->getSprite()->getGlobalBounds().width / 2), text[0]->getPosition().y + ui[1]->getSprite()->getGlobalBounds().height + text[0]->getGlobalBounds().height);
	text.push_back(new sf::Text(std::string("Campaign"), *AssetManager::getFont("title"), 34));
	text[2]->setPosition(ui[1]->getSprite()->getPosition().x + (ui[1]->getSprite()->getGlobalBounds().width / 2) - (text[2]->getGlobalBounds().width / 2), ui[1]->getSprite()->getPosition().y +(ui[1]->getSprite()->getGlobalBounds().height/2) - (text[2]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[2]->setFillColor(sf::Color::Black);

	//Campaign descriptor
	text.push_back(new sf::Text(std::string("Play the classic campaign!"), *AssetManager::getFont("title"), 30));
	text[3]->setPosition(ui[1]->getSprite()->getPosition());
	text[3]->move(ui[1]->getSprite()->getGlobalBounds().width + Config::applyRDX(50), +(ui[1]->getSprite()->getGlobalBounds().height/2)-(text[3]->getGlobalBounds().height/2));
	text[3]->setFillColor(sf::Color::Black);
	text[3]->setStyle(sf::Text::Italic);

	//Endless mode button & label
	ui.push_back(new UIButton(new float[2]{ centerX / 2, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[2]->getSprite()->move(-(ui[1]->getSprite()->getGlobalBounds().width / 2), text[2]->getPosition().y + ui[2]->getSprite()->getGlobalBounds().height + text[2]->getGlobalBounds().height + Config::applyRDY(25));
	text.push_back(new sf::Text(std::string("Endless"), *AssetManager::getFont("title"), 34));
	text[4]->setPosition(ui[2]->getSprite()->getPosition().x + (ui[2]->getSprite()->getGlobalBounds().width / 2) - (text[4]->getGlobalBounds().width / 2), ui[2]->getSprite()->getPosition().y + (ui[2]->getSprite()->getGlobalBounds().height / 2) - (text[4]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[4]->setFillColor(sf::Color::Black);

	//Endless mode descriptor
	text.push_back(new sf::Text(std::string("Try for your new high score in endless mode!"), *AssetManager::getFont("title"), 30));
	text[5]->setPosition(ui[2]->getSprite()->getPosition());
	text[5]->move(ui[2]->getSprite()->getGlobalBounds().width + Config::applyRDX(50), +(ui[2]->getSprite()->getGlobalBounds().height / 2) - (text[5]->getGlobalBounds().height / 2));
	text[5]->setFillColor(sf::Color::Black);
	text[5]->setStyle(sf::Text::Italic);

	//Endless mode button & label
	ui.push_back(new UIButton(new float[2]{ centerX / 2, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[3]->getSprite()->move(-(ui[2]->getSprite()->getGlobalBounds().width / 2), text[4]->getPosition().y + ui[3]->getSprite()->getGlobalBounds().height + text[4]->getGlobalBounds().height + Config::applyRDY(25));
	text.push_back(new sf::Text(std::string("Challenge"), *AssetManager::getFont("title"), 34));
	text[6]->setPosition(ui[3]->getSprite()->getPosition().x + (ui[3]->getSprite()->getGlobalBounds().width / 2) - (text[6]->getGlobalBounds().width / 2), ui[3]->getSprite()->getPosition().y + (ui[3]->getSprite()->getGlobalBounds().height / 2) - (text[6]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[6]->setFillColor(sf::Color::Black);

	//Endless mode descriptor
	text.push_back(new sf::Text(std::string("Test your skill in challenge mode!"), *AssetManager::getFont("title"), 30));
	text[7]->setPosition(ui[3]->getSprite()->getPosition());
	text[7]->move(ui[3]->getSprite()->getGlobalBounds().width + Config::applyRDX(50), +(ui[3]->getSprite()->getGlobalBounds().height / 2) - (text[7]->getGlobalBounds().height / 2));
	text[7]->setFillColor(sf::Color::Black);
	text[7]->setStyle(sf::Text::Italic);

	ui[0]->lock();
	clock.restart();
	//Unlock scene and get ready to function as complete scene
	unlock();
}

void PlaySelectScene::update(sf::RenderWindow* w) {
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
		SceneManager::setNext(0);
		SceneManager::next();
	}
	else if (*ui[0]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[0]->unlock();
		}
	}

	//If campaign button pressed
	if (*ui[1]->getState() == UIState::CLICK) {
		clock.restart();
		ui[1]->lock();
		this->lock();
		SceneManager::setNext(5);
		SceneManager::next();
	}
	else if (*ui[1]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[1]->unlock();
		}
	}

	//If endless button pressed
	if (*ui[2]->getState() == UIState::CLICK) {
		clock.restart();
		ui[2]->lock();
		this->lock();
		SceneManager::setNext(6);
		SceneManager::next();
	}
	else if (*ui[2]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[2]->unlock();
		}
	}

	//If campaign button pressed
	if (*ui[3]->getState() == UIState::CLICK) {
		clock.restart();
		ui[3]->lock();
		this->lock();
		SceneManager::setNext(7);
		SceneManager::next();
	}
	else if (*ui[3]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[3]->unlock();
		}
	}
}

void PlaySelectScene::input(sf::Event* e) {

}