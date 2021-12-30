#include "UIRadioButton.h"

/*
Default (manual) constructor for a radio button
*/
UIRadioButton::UIRadioButton(float x, float y, std::string tex[4], std::string tex_alt[4], float sx, float sy) {
	element = sf::Sprite();
	element.setPosition(x, y);
	element.setScale(sx, sy);
	states.at(UIState::ACTIVE) = tex[0];
	currentState = UIState::ACTIVE;
	element.setTexture(*AssetManager::getTexture(tex[0]));
	setStateTex(UIState::INACTIVE, tex[1]);
	setStateTex(UIState::HOVER, tex[2]);
	setStateTex(UIState::CLICK, tex[3]);
	//Initalise alt texture map
	states_alt = std::map<UIState, std::string>();
	states_alt.insert({ UIState::ACTIVE, ""});
	states_alt.insert({ UIState::INACTIVE, "" });
	states_alt.insert({ UIState::HOVER, "" });
	states_alt.insert({ UIState::CLICK, "" });
	//Update alt (val) textures
	setAltStateTex(UIState::ACTIVE, tex_alt[0]);
	setAltStateTex(UIState::INACTIVE, tex_alt[1]);
	setAltStateTex(UIState::HOVER, tex_alt[2]);
	setAltStateTex(UIState::CLICK, tex_alt[3]);
}

UIRadioButton::UIRadioButton(float pos[2], std::string tex[4], std::string tex_alt[4], float scale[2]) {
	element = sf::Sprite();
	element.setPosition(pos[0], pos[1]);
	element.setScale(scale[0], scale[1]);
	states.at(UIState::ACTIVE) = tex[0];
	currentState = UIState::ACTIVE;
	element.setTexture(*AssetManager::getTexture(tex[0]));
	setStateTex(UIState::INACTIVE, tex[1]);
	setStateTex(UIState::HOVER, tex[2]);
	setStateTex(UIState::CLICK, tex[3]);
	//Initalise alt texture map
	states_alt = std::map<UIState, std::string>();
	//Update alt (val) textures
	setAltStateTex(UIState::ACTIVE, tex_alt[0]);
	setAltStateTex(UIState::INACTIVE, tex_alt[1]);
	setAltStateTex(UIState::HOVER, tex_alt[2]);
	setAltStateTex(UIState::CLICK, tex_alt[3]);
}

void UIRadioButton::update(sf::RenderWindow* w) {
	//Update bounds
	minX = element.getPosition().x;
	maxX = minX + element.getGlobalBounds().width;
	minY = element.getPosition().y;
	maxY = minY + element.getGlobalBounds().height;

	//Get mouse position
	float mouse_x = sf::Mouse::getPosition(*w).x;
	float mouse_y = sf::Mouse::getPosition(*w).y;

	//Update button value & state
	if (this->locked == false) {
		setState(UIState::ACTIVE);
		//Check if mouse is within element bounds
		if (((mouse_x >= minX) && (mouse_x <= maxX)) && ((mouse_y >= minY) && (mouse_y <= maxY))) {
			//If so set to hover
			setState(UIState::HOVER);
			//If user ALSO clicks on button
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				//Update tex to click
				setState(UIState::CLICK);
				//Invert state
				state = !state;
			}
		}
	}
	else {
		setState(UIState::INACTIVE);
	}

	//Switch texture based on state & value of radio button
	switch (state) {
	case true:
		switch (currentState) {
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
	case false:
		switch (currentState) {
		case UIState::HOVER:
			element.setTexture(*AssetManager::getTexture(states_alt.at(UIState::HOVER)));
			break;
		case UIState::INACTIVE:
			element.setTexture(*AssetManager::getTexture(states_alt.at(UIState::INACTIVE)));
			break;
		case UIState::CLICK:
			element.setTexture(*AssetManager::getTexture(states_alt.at(UIState::CLICK)));
			break;
		case UIState::ACTIVE:
			element.setTexture(*AssetManager::getTexture(states_alt.at(UIState::ACTIVE)));
			break;
		default:
			element.setTexture(*AssetManager::getTexture(states_alt.at(UIState::ACTIVE)));
			break;
		}
	default:
		break;
	}
}

//Simple method to dynamically allocate state and texture binding for alternative map
void UIRadioButton::setAltStateTex(UIState s, std::string ref) {
	switch (s) {
	case UIState::ACTIVE:
		states_alt.at(s) = ref;
		break;
	case UIState::HOVER:
		states_alt.at(s) = ref;
		break;
	case UIState::LOCK:
		states_alt.at(s) = ref;
		break;
	case UIState::CLICK:
		states_alt.at(s) = ref;
		break;
	default:
		break;
	}
}

//Simple get for state
bool UIRadioButton::get() {
	return state;
}

//Simple set for state (override)
void UIRadioButton::set(bool to) {
	state = to;
}