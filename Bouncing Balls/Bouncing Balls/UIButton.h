#pragma once

#ifndef UIELEMENT_BUTTON_SHORT
#define UIELEMENT_BUTTON_SHORT

#include "UIElement.h"
class UIButton : public UIElement
{
public:
    UIButton(float x, float y, std::string defTex, float scale_x = 1.0f, float scale_y = 1.0f);
    UIButton(float pos[2], std::string type, float scale[2]);
    virtual void update(sf::RenderWindow* w);
private:
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;
};

#endif

