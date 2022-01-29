#include "StatsScene.h"
#include "PlayerData.h"

StatsScene::StatsScene(int id, std::string name) {
	this->sceneID = id;
	this->name = name;
}

void StatsScene::init() {
	//Lock scene during initalisaton (unavailable for input / drawing)
	lock();
	clock = sf::Clock();
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	resize = false;
	//Set up easy centering variable
	centerX = (Config::user_resolution.X() / 2);

	//Background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));

	//Title text
	text.push_back(new sf::Text(std::string("Player Statistics"), *AssetManager::getFont("title"), 75));
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

	//Campaign compelte identifier
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 34));
	text[2]->setPosition(centerX - (text[2]->getGlobalBounds().width / 2), text[0]->getPosition().y + text[0]->getGlobalBounds().height + Config::applyRDY(25));
	std::cout << text[2]->getPosition().x << "," << text[2]->getPosition().y << std::endl;
	text[2]->setFillColor(sf::Color::Black);
	text[2]->setString("Campaign Complete: " + Utils::boolToString(Player::getData()->campaign_finished));

	//Add up stars from campaign
	int total_stars = 0;
	for (int i = 0; i < 6; i++) {
		total_stars += Player::getData()->campaign_stars[i];
	}

	//Display total campaign stars
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 34));
	text[3]->setPosition(centerX - (text[3]->getGlobalBounds().width / 2), text[2]->getPosition().y + text[2]->getGlobalBounds().height + Config::applyRDY(25));
	text[3]->setFillColor(sf::Color::Black);
	text[3]->setString("Total Campaign Stars (/Max): " + std::to_string(total_stars) + " / 18");

	//Display high-score for endless mode
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 34));
	text[4]->setPosition(centerX - (text[4]->getGlobalBounds().width / 2), text[3]->getPosition().y + text[3]->getGlobalBounds().height + Config::applyRDY(25));
	text[4]->setFillColor(sf::Color::Black);
	text[4]->setString("High-score (Endless Mode): " + std::to_string(Player::getData()->endless_hs));

	//Display high-score for challenge mode
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 34));
	text[5]->setPosition(centerX - (text[5]->getGlobalBounds().width / 2), text[4]->getPosition().y + text[4]->getGlobalBounds().height + Config::applyRDY(25));
	text[5]->setFillColor(sf::Color::Black);
	text[5]->setString("High-score (Challenge Mode): " + std::to_string(Player::getData()->challenge_hs));

	//Display total coins
	text.push_back(new sf::Text(std::string(" "), *AssetManager::getFont("title"), 34));
	text[6]->setPosition(centerX - (text[6]->getGlobalBounds().width / 2), text[5]->getPosition().y + text[5]->getGlobalBounds().height + Config::applyRDY(25));
	text[6]->setFillColor(sf::Color::Black);
	text[6]->setString("Total Coins: " + std::to_string(Player::getData()->coins));

	//Reset button
	ui.push_back(new UIButton(new float[2]{ 0.f, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[1]->getSprite()->setPosition(ui[0]->getSprite()->getPosition());
	ui[1]->getSprite()->move(0.f, -(ui[1]->getSprite()->getGlobalBounds().height) - Config::applyRDY(25));
	//Reset button label
	text.push_back(new sf::Text(std::string("Reset"), *AssetManager::getFont("title"), 34));
	text[7]->setPosition(ui[1]->getSprite()->getPosition());
	text[7]->move((ui[1]->getSprite()->getGlobalBounds().width / 2) - text[7]->getGlobalBounds().width / 2, (ui[1]->getSprite()->getGlobalBounds().height / 2) - (text[7]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[7]->setFillColor(sf::Color::Black);

	text[2]->setPosition(centerX - (text[2]->getGlobalBounds().width / 2), text[0]->getPosition().y + text[0]->getGlobalBounds().height + Config::applyRDY(25));
	text[3]->setPosition(centerX - (text[3]->getGlobalBounds().width / 2), text[2]->getPosition().y + text[2]->getGlobalBounds().height + Config::applyRDY(25));
	text[4]->setPosition(centerX - (text[4]->getGlobalBounds().width / 2), text[3]->getPosition().y + text[3]->getGlobalBounds().height + Config::applyRDY(25));
	text[5]->setPosition(centerX - (text[5]->getGlobalBounds().width / 2), text[4]->getPosition().y + text[4]->getGlobalBounds().height + Config::applyRDY(25));
	text[6]->setPosition(centerX - (text[6]->getGlobalBounds().width / 2), text[5]->getPosition().y + text[5]->getGlobalBounds().height + Config::applyRDY(25));
}

void StatsScene::update(sf::RenderWindow* w) {
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

	//If reset button is pressed
	if (*ui[1]->getState() == UIState::CLICK) {
		clock.restart();
		ui[1]->lock();
		Player::save();
		Player::load();
		int total_stars = 0;
		for (int i = 0; i < 6; i++) {
			total_stars += Player::getData()->campaign_stars[i];
		}
		text[2]->setString("Campaign Complete: " + Utils::boolToString(Player::getData()->campaign_finished));
		text[3]->setString("Total Campaign Stars (/Max): " + std::to_string(total_stars) + " / 18");
		text[4]->setString("High-score (Endless Mode): " + std::to_string(Player::getData()->endless_hs));
		text[5]->setString("High-score (Challenge Mode): " + std::to_string(Player::getData()->challenge_hs));
		text[6]->setString("Total Coins: " + std::to_string(Player::getData()->coins));
		text[2]->setPosition(centerX - (text[2]->getGlobalBounds().width / 2), text[0]->getPosition().y + text[0]->getGlobalBounds().height + Config::applyRDY(25));
		text[3]->setPosition(centerX - (text[3]->getGlobalBounds().width / 2), text[2]->getPosition().y + text[2]->getGlobalBounds().height + Config::applyRDY(25));
		text[4]->setPosition(centerX - (text[4]->getGlobalBounds().width / 2), text[3]->getPosition().y + text[3]->getGlobalBounds().height + Config::applyRDY(25));
		text[5]->setPosition(centerX - (text[5]->getGlobalBounds().width / 2), text[4]->getPosition().y + text[4]->getGlobalBounds().height + Config::applyRDY(25));
		text[6]->setPosition(centerX - (text[6]->getGlobalBounds().width / 2), text[5]->getPosition().y + text[5]->getGlobalBounds().height + Config::applyRDY(25));
	}
	else if (*ui[1]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[1]->unlock();
		}
	}
}

void StatsScene::input(sf::Event* e) {

}