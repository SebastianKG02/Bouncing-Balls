#pragma once
#include "SceneManager.h"

#ifndef SCN_TITLE_SCREEN
#define SCN_TITLE_SCREEN

/*
TITLE SCREEN (or default scene)
This scene will be the first screen the user interacts with, expanding upon the default Scene
It will mainly be a navigation menu between other screens within the game.
*/
class DefaultScene : public Scene
{
public:
    DefaultScene(uint8_t id, std::string name);
    void init();
    void update(sf::RenderWindow* w);
    void input(sf::Event* e);
private:
    bool resize = false;
    float maxBound = 107.5f;
    float minBound = 72.5f;
    float angleDelta = 0.009f;
    bool minMax = false;
    float centerX = 0;
    float counter = 0;
    //std::vector<sf::Vector2f*> balls;
};

#endif