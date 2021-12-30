#pragma once

#ifndef UI_RADIO_BUTTON
#define UI_RADIO_BUTTON
#include "UIElement.h"

class UIRadioButton : public UIElement
{
public:
    UIRadioButton(float x, float y, std::string tex[4], std::string tex_alt[4], float sx, float sy);
    UIRadioButton(float pos[2], std::string tex[4], std::string tex_alt[4], float scale[2]);
	virtual void update(sf::RenderWindow* w);
    void setAltStateTex(UIState s, std::string ref);
    bool get();
    void set(bool to);
private:
    std::map<UIState, std::string> states_alt;
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;
    bool state = false;
};

#endif

