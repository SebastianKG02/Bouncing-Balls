#pragma once

#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

/*
V0.3a - UI Element
This class will serve as the base component in building a fluid, reactive UI system
The objective of this class is to use one sprite to represent the physical element,
as well as provide virtual methods for proper UI elements to implement on their own.

There are four states a UI element can be in: INACTIVE (LOCK), HOVER (SELECT), ACTIVE and CLICK:
INACTIVE - this UI element is not being interacted with (this could also be LOCK)
HOVER - the user is "hovering" over the element (this could also be SELECT)
ACTIVE - the user is able to interact with this element, but has not yet done so
CLICK - the user has interacted with this element, and now something must be done.

V0.5a - UIState changes
Two more UIStates have been added for pseudo-radio button functionality (more texture switching)
These are:
- TRUE
- FALSE

These two new enum features will allow for TRUE/FALSE radio buttons
by allowing them to represent boolean values
*/

#include <SFML/Graphics.hpp>
#include <functional>
#include "AssetManager.h"

enum class UIState
{
	INACTIVE,
	LOCK = INACTIVE,
	HOVER,
	SELECT = HOVER,
	ACTIVE,
	CLICK,
	TRUE,
	FALSE
};

class UIElement
{
public:
	UIElement();
	virtual void init();
	virtual void update(sf::RenderWindow *win);
	void lock();
	void unlock();
	void cleanup();
	UIState* getState();
	void setState(UIState s);
	void setStateTex(UIState s, std::string ref);
	sf::Sprite* getSprite();
protected:
	sf::Sprite element;
	std::map<UIState, std::string> states;
	UIState currentState;
	bool locked = false;
};

#endif

