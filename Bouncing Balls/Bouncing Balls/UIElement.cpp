#include "UIElement.h"

//Simple initaliser
UIElement::UIElement() {
	element = sf::Sprite();
	currentState = UIState::INACTIVE;
	states = std::map<UIState, std::string>();
	states.insert({ UIState::ACTIVE, ""});
	states.insert({ UIState::HOVER, "" });
	states.insert({ UIState::LOCK, "" });
	states.insert({ UIState::CLICK, ""});
}

//Empty reference array
void UIElement::cleanup() {
	states.empty();
}

//Simple method to dynamically allocate state and texture binding
void UIElement::setStateTex(UIState s, std::string ref) {
	switch (s) {
	case UIState::ACTIVE:
		states.at(s) = ref;
		break;
	case UIState::HOVER:
		states.at(s) = ref;
		break;
	case UIState::LOCK:
		states.at(s) = ref;
		break;
	case UIState::CLICK:
		states.at(s) = ref;
		break;
	default:
		break;
	}
}

//Empty initaliser - UIElement child classes will override this method
void UIElement::init() {

}

//Empty update method - UIElement child classes will override this method
void UIElement::update(sf::RenderWindow *win) {

}

//Simple method to get reference to current state of this element
UIState* UIElement::getState() {
	return &currentState;
}

//Simple method to override current state of this element
void UIElement::setState(UIState s) {
	currentState = s;
}

//Method for external classes to reference the sprite associated with this element
sf::Sprite* UIElement::getSprite() {
	return &element;
}

//Set UIE state to locked (if unlocked)
void UIElement::lock() {
	if (locked == false) {
		locked = true;
	}
}

//Set UIE state to unlocked (if locked)
void UIElement::unlock() {
	if (locked == true) {
		locked = false;
	}
}