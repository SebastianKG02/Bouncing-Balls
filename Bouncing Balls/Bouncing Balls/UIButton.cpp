#include "UIButton.h"

//Default contructor
UIButton::UIButton(float x, float y, std::string defTex, float scale_x, float scale_y) {
	element = sf::Sprite();
	element.setPosition(x, y);
	element.setScale(scale_x, scale_y);
	states.at(UIState::ACTIVE) = defTex;
	element.setTexture(*AssetManager::getTexture(defTex));
	currentState = UIState::ACTIVE;

	minX = x;
	maxX = minX + element.getGlobalBounds().width;
	minY = y;
	maxY = minY + element.getGlobalBounds().height;
}

//Update state based on mouse position
void UIButton::update(sf::RenderWindow* w) {
	//Poll relative mouse position in window
	float mouse_x = sf::Mouse::getPosition(*w).x;
	float mouse_y = sf::Mouse::getPosition(*w).y;

	//Only update if not locked
	if (locked == false) {
		currentState = UIState::ACTIVE;
		//Check if mouse is within texture rect
		if (((mouse_x >= minX) && (mouse_x <= maxX)) && ((mouse_y >= minY) && (mouse_y <= maxY))) {
			//If true set to hover state
			currentState = UIState::HOVER;
			//If user is hoveing over button AND presses lclick
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				//Report state as being clicked
				currentState = UIState::CLICK;
			}
		}
	}
	else {
		//If locked, do not process any input
		currentState = UIState::LOCK;
	}

	//Switch texture based on state
	switch (currentState)
	{
	case UIState::HOVER:
		element.setTexture(*AssetManager::getTexture(states.at(UIState::HOVER)));
		break;
	case UIState::INACTIVE:
		element.setTexture(*AssetManager::getTexture(states.at(UIState::INACTIVE)));
		break;
	case UIState::CLICK:
		element.setTexture(*AssetManager::getTexture(states.at(UIState::CLICK)));
		break;
	case UIState::ACTIVE:
		element.setTexture(*AssetManager::getTexture(states.at(UIState::ACTIVE)));
		break;
	default:
		element.setTexture(*AssetManager::getTexture(states.at(UIState::ACTIVE)));
		break;
	}
}