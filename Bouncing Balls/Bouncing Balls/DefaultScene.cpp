#include "DefaultScene.h"

void DefaultScene::init() {
	text.push_back(new sf::Text(std::string("Bouncing Balls"), *AssetManager::getFont("title"), 48));
	text[0]->setPosition(520, 300);
}

void DefaultScene::update() {

}

void DefaultScene::input() {

}

DefaultScene::DefaultScene(uint8_t id, std::string name) {
	this->sceneID = id;
	this->name = name;
}