#pragma once

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <vector>
#include <map>
#include <iostream>
#include "Utils.h"
#include "Defines.h"
#include "AssetManager.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>

/*
V0.1a - Scene
The Scene class is a template class for all future scenes to be loaded in the SceneManager
*/
class Scene
{
public:
	Scene(uint8_t id, std::string name);
	Scene();
	virtual void init();
	virtual void cleanup();
	virtual void draw(sf::RenderWindow* w);
	virtual void update();
	virtual void input();
	std::string getFriendlyName();
	int getID();
	void setID(int id);
protected:
	std::vector<sf::Sprite*> sprites;
	std::vector<sf::Sound*> sounds;
	std::vector<sf::Text*> text;
	int sceneID;
	std::string name;
};

#include "DefaultScene.h"
/*
V0.1a - Scene Manager
The SceneManager is responsible for switching between scenes (or screens),
initalising them and loading all the appropriate resources
*/
class SceneManager
{
public:
	static void init();
	static void cleanup();
	static void draw(sf::RenderWindow* w);
	static void input();
	static void update();
	static uint8_t getCurrentSceneID();
	static Scene* getCurrentScene();
	static Scene* getScene(int id);
	static Scene* getNext(int id);
	static Scene* getPrev(int id);
	static void next();
	static void previous();
	static void set(uint8_t id);
	static void setNext(uint8_t id);
	static void setPrev(uint8_t id);
	static void addScene(Scene* scene);
	static void delScene(uint8_t id);
private:
	static std::map<uint8_t, Scene*> scenes;
	static uint8_t nextScene;
	static uint8_t prevScene;
	static uint8_t currScene;
	static bool initComplete;
	static bool cleanupComplete;
};
#endif