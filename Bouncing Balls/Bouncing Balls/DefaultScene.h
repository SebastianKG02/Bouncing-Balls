#pragma once
#include "SceneManager.h"
#include "SceneManager.h"
class DefaultScene : public Scene
{
public:
    DefaultScene(uint8_t id, std::string name);
    void init();
    void update();
    void input();
};

