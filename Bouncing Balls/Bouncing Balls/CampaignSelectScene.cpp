#include "CampaignSelectScene.h"
#include "PlayerData.h"

CampaignSelectScene::CampaignSelectScene(int id, std::string) {
	this->sceneID = id;
	this->name = name;
}

void CampaignSelectScene::init() {
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
	text.push_back(new sf::Text(std::string("Campaign Level Select"), *AssetManager::getFont("title"), 75));
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

	//Level 1 button
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[1]->getSprite()->setPosition(centerX - (1.5f * ui[1]->getSprite()->getGlobalBounds().width) - Config::applyRDX(25), text[0]->getPosition().y + text[0]->getGlobalBounds().height+Config::applyRDY(100));
	//Level 1 button label
	text.push_back(new sf::Text(std::string("Level 1"), *AssetManager::getFont("title"), 34));
	text[2]->setPosition(ui[1]->getSprite()->getPosition());
	text[2]->move((ui[1]->getSprite()->getGlobalBounds().width / 2)-(text[2]->getGlobalBounds().width/2), (ui[1]->getSprite()->getGlobalBounds().height / 2) - (text[2]->getGlobalBounds().height / 2)-Config::applyRDY(5));
	text[2]->setFillColor(sf::Color::Black);
	//Level 1 curent stars
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture(std::to_string(Player::getData()->campaign_stars[0]) + "_stars")));
	sprites[1]->setPosition(ui[1]->getSprite()->getPosition());
	sprites[1]->setScale(0.4f, 0.4f);
	sprites[1]->move((ui[1]->getSprite()->getGlobalBounds().width/2)-(sprites[1]->getGlobalBounds().width/2), -sprites[1]->getGlobalBounds().height);

	//Level 2 button
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[2]->getSprite()->setPosition(centerX - (ui[1]->getSprite()->getGlobalBounds().width/2) - Config::applyRDX(5), text[0]->getPosition().y + text[0]->getGlobalBounds().height + Config::applyRDY(100));
	//Level 2 button label
	text.push_back(new sf::Text(std::string("Level 2"), *AssetManager::getFont("title"), 34));
	text[3]->setPosition(ui[2]->getSprite()->getPosition());
	text[3]->move((ui[2]->getSprite()->getGlobalBounds().width / 2) - (text[3]->getGlobalBounds().width / 2), (ui[2]->getSprite()->getGlobalBounds().height / 2) - (text[3]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[3]->setFillColor(sf::Color::Black);
	//Level 2 curent stars
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture(std::to_string(Player::getData()->campaign_stars[1]) + "_stars")));
	sprites[2]->setPosition(ui[2]->getSprite()->getPosition());
	sprites[2]->setScale(0.4f, 0.4f);
	sprites[2]->move((ui[2]->getSprite()->getGlobalBounds().width / 2) - (sprites[2]->getGlobalBounds().width / 2), -sprites[2]->getGlobalBounds().height);

	//Level 3 button
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[3]->getSprite()->setPosition(ui[2]->getSprite()->getPosition().x+(1.0f*ui[3]->getSprite()->getGlobalBounds().width)+Config::applyRDX(20), text[0]->getPosition().y + text[0]->getGlobalBounds().height + Config::applyRDY(100));
	//Level 3 button label
	text.push_back(new sf::Text(std::string("Level 3"), *AssetManager::getFont("title"), 34));
	text[4]->setPosition(ui[3]->getSprite()->getPosition());
	text[4]->move((ui[3]->getSprite()->getGlobalBounds().width / 2) - (text[4]->getGlobalBounds().width / 2), (ui[3]->getSprite()->getGlobalBounds().height / 2) - (text[4]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[4]->setFillColor(sf::Color::Black);
	//Level 3 curent stars
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture(std::to_string(Player::getData()->campaign_stars[2]) + "_stars")));
	sprites[3]->setPosition(ui[3]->getSprite()->getPosition());
	sprites[3]->setScale(0.4f, 0.4f);
	sprites[3]->move((ui[3]->getSprite()->getGlobalBounds().width / 2) - (sprites[3]->getGlobalBounds().width / 2), -sprites[3]->getGlobalBounds().height);

	//Level 4 button
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[4]->getSprite()->setPosition(centerX - (1.5f * ui[4]->getSprite()->getGlobalBounds().width) - Config::applyRDX(25), text[2]->getPosition().y + text[2]->getGlobalBounds().height + Config::applyRDY(125));
	//Level 4 button label
	text.push_back(new sf::Text(std::string("Level 4"), *AssetManager::getFont("title"), 34));
	text[5]->setPosition(ui[4]->getSprite()->getPosition());
	text[5]->move((ui[4]->getSprite()->getGlobalBounds().width / 2) - (text[5]->getGlobalBounds().width / 2), (ui[4]->getSprite()->getGlobalBounds().height / 2) - (text[5]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[5]->setFillColor(sf::Color::Black);
	//Level 4 curent stars
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture(std::to_string(Player::getData()->campaign_stars[3]) + "_stars")));
	sprites[4]->setPosition(ui[4]->getSprite()->getPosition());
	sprites[4]->setScale(0.4f, 0.4f);
	sprites[4]->move((ui[4]->getSprite()->getGlobalBounds().width / 2) - (sprites[4]->getGlobalBounds().width / 2), -sprites[4]->getGlobalBounds().height);

	//Level 5 button
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[5]->getSprite()->setPosition(centerX - (ui[4]->getSprite()->getGlobalBounds().width / 2) - Config::applyRDX(5), text[2]->getPosition().y + text[2]->getGlobalBounds().height + Config::applyRDY(125));
	//Level 5 button label
	text.push_back(new sf::Text(std::string("Level 5"), *AssetManager::getFont("title"), 34));
	text[6]->setPosition(ui[5]->getSprite()->getPosition());
	text[6]->move((ui[5]->getSprite()->getGlobalBounds().width / 2) - (text[6]->getGlobalBounds().width / 2), (ui[5]->getSprite()->getGlobalBounds().height / 2) - (text[6]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[6]->setFillColor(sf::Color::Black);
	//Level 5 curent stars
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture(std::to_string(Player::getData()->campaign_stars[4]) + "_stars")));
	sprites[5]->setPosition(ui[5]->getSprite()->getPosition());
	sprites[5]->setScale(0.4f, 0.4f);
	sprites[5]->move((ui[5]->getSprite()->getGlobalBounds().width / 2) - (sprites[5]->getGlobalBounds().width / 2), -sprites[5]->getGlobalBounds().height);

	//Level 6 button
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[6]->getSprite()->setPosition(ui[5]->getSprite()->getPosition().x + (1.0f * ui[6]->getSprite()->getGlobalBounds().width) + Config::applyRDX(20), text[2]->getPosition().y + text[2]->getGlobalBounds().height + Config::applyRDY(125));
	//Level 6 button label
	text.push_back(new sf::Text(std::string("Level 6"), *AssetManager::getFont("title"), 34));
	text[7]->setPosition(ui[6]->getSprite()->getPosition());
	text[7]->move((ui[6]->getSprite()->getGlobalBounds().width / 2) - (text[7]->getGlobalBounds().width / 2), (ui[6]->getSprite()->getGlobalBounds().height / 2) - (text[7]->getGlobalBounds().height / 2) - Config::applyRDY(5));
	text[7]->setFillColor(sf::Color::Black);
	//Level 3 curent stars
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture(std::to_string(Player::getData()->campaign_stars[5]) + "_stars")));
	sprites[6]->setPosition(ui[6]->getSprite()->getPosition());
	sprites[6]->setScale(0.4f, 0.4f);
	sprites[6]->move((ui[6]->getSprite()->getGlobalBounds().width / 2) - (sprites[6]->getGlobalBounds().width / 2), -sprites[6]->getGlobalBounds().height);

	Player::save();
	Player::load();
	//Loop through each campaign completion marker in player data and disable levels that have not been unlocked yet
	for (int i = 2; i < 7; i++) {
		if (Player::getData()->campaign_comp[i-2] == false) {
			ui[i]->lock();
		}
		else {
			ui[i]->unlock();
		}
	}

	//Level 1 should always be available
	ui[1]->unlock();

	text.push_back(new sf::Text(std::string("Unlock each level by getting at least one star in the previous level!"), *AssetManager::getFont("title"), 28));
	text[8]->setPosition(centerX-(text[8]->getGlobalBounds().width/2), ui[0]->getSprite()->getPosition().y-text[8]->getGlobalBounds().height-Config::applyRDY(25));
	text[8]->setFillColor(sf::Color::Black);

	//Unlock scene and get ready to function as complete scene
	unlock();
}

void CampaignSelectScene::update(sf::RenderWindow* w) {
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

	//If play level (i) button pressed
	for (int i = 1; i < 7; i++) {
		//Specific handling for level 1 as always available
		if (i == 1) {
			if(*ui[1]->getState() == UIState::CLICK) {
				clock.restart();
				ui[1]->lock();
				this->lock();
				SceneManager::getCampSettings(0)->level = 0;
				Player::getData()->last_level = 0;
				std::cout << "[SM]Sending to Campaign Level " << SceneManager::getCampSettings(0)->level << std::endl;
				SceneManager::setNext(9);
				SceneManager::next();
			}
			else if (*ui[1]->getState() == UIState::LOCK) {
				if (clock.getElapsedTime().asMilliseconds() > 200) {
					ui[1]->unlock();
				}
			}
		}
		else {
			if (Player::getData()->campaign_comp[i - 2] == true && *ui[i]->getState() == UIState::CLICK) {
				clock.restart();
				ui[i]->lock();
				this->lock();
				Player::getData()->last_level = (i - 1);
				SceneManager::setNext(9);
				SceneManager::next();
			}
			else if (Player::getData()->campaign_comp[i - 2] == true && *ui[i]->getState() == UIState::LOCK) {
				if (clock.getElapsedTime().asMilliseconds() > 200) {
					ui[i]->unlock();
				}
			}
			else if (Player::getData()->campaign_comp[i - 2] == false) {
				ui[i]->lock();
			}
		}
	}
}

void CampaignSelectScene::input(sf::Event* e) {

}