#include "EndlessPreGameScene.h"
#include "PlayerData.h"

EndlessPreGameScene::EndlessPreGameScene(int id, std::string name) {
	this->sceneID = id;
	this->name = name;
}

void EndlessPreGameScene::init() {
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
	text.push_back(new sf::Text(std::string("Endless Menu"), *AssetManager::getFont("title"), 75));
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

	//Current endless level label
	text.push_back(new sf::Text(std::string("Current Endless Level: " + std::to_string(Player::getData()->endless_level)), *AssetManager::getFont("title"), 40));
	text[2]->setPosition(text[0]->getPosition());
	text[2]->move((text[0]->getGlobalBounds().width / 2) - (text[2]->getGlobalBounds().width / 2), text[0]->getGlobalBounds().height + Config::applyRDY(25));
	text[2]->setFillColor(sf::Color::Black);

	//Current endless level label
	text.push_back(new sf::Text(std::string("Next Endless Level: " + std::to_string(Player::getData()->endless_level + 1)), *AssetManager::getFont("title"), 40));
	text[3]->setPosition(text[2]->getPosition());
	text[3]->move((text[2]->getGlobalBounds().width / 2) - (text[3]->getGlobalBounds().width / 2), text[2]->getGlobalBounds().height + Config::applyRDY(5));
	text[3]->setFillColor(sf::Color::Black);

	//All-time endless high-score label
	text.push_back(new sf::Text(std::string("Your high-score: " + std::to_string(Player::getData()->endless_hs)), *AssetManager::getFont("title"), 40));
	text[4]->setPosition(text[3]->getPosition());
	text[4]->move((text[3]->getGlobalBounds().width / 2) - (text[4]->getGlobalBounds().width / 2), text[3]->getGlobalBounds().height + Config::applyRDY(5));
	text[4]->setFillColor(sf::Color::Black);

	//Current modofiers label
	text.push_back(new sf::Text(std::string("Current modifiers:"), *AssetManager::getFont("title"), 40));
	text[5]->setPosition(text[4]->getPosition());
	text[5]->move((text[4]->getGlobalBounds().width / 2) - (text[5]->getGlobalBounds().width / 2), text[4]->getGlobalBounds().height + Config::applyRDY(5));
	text[5]->setFillColor(sf::Color::Black);
	//Current endless level modifiers
	text.push_back(new sf::Text(std::string("Points: +" + std::to_string(GM_ENDLESS_PTS_MOD_LVL * Player::getData()->endless_level) + "%, Game speed: +" + std::to_string(GM_ENDLESS_SPD_MOD_LVL * Player::getData()->endless_level) + "%"), *AssetManager::getFont("title"), 40));
	text[6]->setPosition(text[5]->getPosition());
	text[6]->move((text[5]->getGlobalBounds().width / 2) - (text[6]->getGlobalBounds().width / 2), text[5]->getGlobalBounds().height + Config::applyRDY(5));
	text[6]->setFillColor(sf::Color::Black);

	//Next modofiers label
	text.push_back(new sf::Text(std::string("Next modifiers:"), *AssetManager::getFont("title"), 40));
	text[7]->setPosition(text[6]->getPosition());
	text[7]->move((text[6]->getGlobalBounds().width / 2) - (text[7]->getGlobalBounds().width / 2), text[6]->getGlobalBounds().height + Config::applyRDY(5));
	text[7]->setFillColor(sf::Color::Black);
	//Next endless level modifiers
	text.push_back(new sf::Text(std::string("Points: +" + std::to_string(GM_ENDLESS_PTS_MOD_LVL * (Player::getData()->endless_level+1)) + "%, Game speed: +" + std::to_string(GM_ENDLESS_SPD_MOD_LVL * (Player::getData()->endless_level+1)) + "%"), *AssetManager::getFont("title"), 40));
	text[8]->setPosition(text[7]->getPosition());
	text[8]->move((text[7]->getGlobalBounds().width / 2) - (text[8]->getGlobalBounds().width / 2), text[7]->getGlobalBounds().height + Config::applyRDY(5));
	text[8]->setFillColor(sf::Color::Black);

	//Add next level button
	ui.push_back(new UIButton(new float[2]{ ui[0]->getSprite()->getPosition().x, ui[0]->getSprite()->getPosition().y }, "long", new float[2]{ 0.1f, 0.1f }));
	ui[1]->getSprite()->move(0, -ui[1]->getSprite()->getGlobalBounds().height - Config::applyRDY(5));
	text.push_back(new sf::Text(std::string("Play Next Level"), *AssetManager::getFont("title"), 30));
	text[9]->setPosition(ui[1]->getSprite()->getPosition());
	text[9]->move((ui[1]->getSprite()->getGlobalBounds().width/2)-(text[9]->getGlobalBounds().width/2), (ui[1]->getSprite()->getGlobalBounds().height/2)-(text[9]->getGlobalBounds().height/2)-Config::applyRDY(2));
	text[9]->setFillColor(sf::Color::Black);

	ui[0]->lock();
	ui[1]->lock();
	//Unlock scene and get ready to function as complete scene
	unlock();
}

void EndlessPreGameScene::update(sf::RenderWindow* w) {
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
		//Go back to endless info scene
		SceneManager::setNext(6);
		SceneManager::next();
	}
	else if (*ui[0]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[0]->unlock();
		}
	}

	//If next level button pressed
	if (*ui[1]->getState() == UIState::CLICK) {
		clock.restart();
		ui[1]->lock();
		this->lock();
		//Go back to endless info scene
		SceneManager::setNext(13);
		SceneManager::next();
	}
	else if (*ui[1]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[1]->unlock();
		}
	}
}

void EndlessPreGameScene::input(sf::Event* e) {

}