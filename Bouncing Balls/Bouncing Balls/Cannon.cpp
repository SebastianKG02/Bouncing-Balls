#include "Cannon.h"
#include "AssetManager.h"
#include "Config.h"

Cannon::Cannon() {

}

//Default constructor
Cannon::Cannon(int x, int y, bool centerX, bool centerY) {
	this->x = x;
	this->y = y;
	base = sf::Sprite(*AssetManager::getTexture("cannon_base"));
	axel = sf::Sprite(*AssetManager::getTexture("cannon_axel"));
	body = sf::Sprite(*AssetManager::getTexture("cannon_body"));

	minRot = GM_MIN_CANNON_ROT;
	maxRot = GM_MAX_CANNON_ROT;
	rot = 0;
	mouseMove = false;

	/*
	if (centerX && centerY == false) {
		base.setOrigin(base.getGlobalBounds().width / 2, 0);
		axel.setOrigin(axel.getGlobalBounds().width / 2, 0);
	}
	else if (centerX == false && centerY) {
		base.setOrigin(0, base.getGlobalBounds().height / 2);
		axel.setOrigin(0, axel.getGlobalBounds().height / 2);
	}
	else if (centerX && centerY) {
		base.setOrigin(base.getGlobalBounds().width / 2, base.getGlobalBounds().height / 2);
		axel.setOrigin(axel.getGlobalBounds().width / 2, axel.getGlobalBounds().height / 2);
	}
	*/
	base.setScale(0.25f, 0.25f);
	axel.setScale(0.75f, 0.75f);
	body.setScale(0.25f, 0.25f);
}

void Cannon::update(sf::RenderWindow* w) {
	base.setPosition(x-(base.getGlobalBounds().width/2), y);
	axel.setPosition(base.getPosition());
	axel.move((base.getGlobalBounds().width / 2) - (axel.getGlobalBounds().width/2), -(axel.getGlobalBounds().height/2));
	body.setOrigin(body.getLocalBounds().width/2, body.getLocalBounds().height);
	body.setPosition(axel.getPosition().x+(axel.getGlobalBounds().width/2), axel.getPosition().y+(axel.getGlobalBounds().height/2));
	
	float dx = 0;
	float dy = 0;

	//If mouse to the right
	if (sf::Mouse::getPosition(*w).x > body.getPosition().x || sf::Keyboard::isKeyPressed(Config::user_key_right)) {
		if (sf::Keyboard::isKeyPressed(Config::user_key_right)) {
			rot -= GM_KEYBOARD_MOVE_SPEED;
			sf::Mouse::setPosition(sf::Vector2i(Config::user_resolution.X() / 2, Config::user_resolution.Y() / 2), *w);
		}
		else if(mouseMove){
			dx = sf::Mouse::getPosition(*w).x - body.getPosition().x;
			dy = sf::Mouse::getPosition(*w).y - body.getPosition().y;
			rot = atan(dx / dy) * (180 / 3.14f);
		}
	}
	
	if (sf::Mouse::getPosition(*w).x < body.getPosition().x || sf::Keyboard::isKeyPressed(Config::user_key_left)) {
		if (sf::Keyboard::isKeyPressed(Config::user_key_left)) {
			rot += GM_KEYBOARD_MOVE_SPEED;
			sf::Mouse::setPosition(sf::Vector2i(Config::user_resolution.X() / 2, Config::user_resolution.Y() / 2), *w);
		}else if(mouseMove){
			dx = body.getPosition().x - sf::Mouse::getPosition(*w).x;
			dy = body.getPosition().y - sf::Mouse::getPosition(*w).y;
			rot = atan(dx / dy) * (180 / 3.14f);
		}
	}

	if (rot > minRot && rot < maxRot) {
		body.setRotation(-rot);
	}
}

void Cannon::input(sf::Event* e) {
	if (e->type == sf::Event::MouseMoved) {
		mouseMove = true;
	}
	else {
		mouseMove = false;
	}
}

sf::Sprite* Cannon::getBase() {
	return &base;
}

sf::Sprite* Cannon::getAxel() {
	return &axel;
}

sf::Sprite* Cannon::getBody() {
	return &body;
}

float* Cannon::X() {
	return &x;
}

float* Cannon::Y() {
	return &y;
}