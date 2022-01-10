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
#include "UIElement.h"
#include "UIButton.h"
#include "Config.h"

/*
V0.1a - Scene
The Scene class is a template class for all future scenes to be loaded in the SceneManager
*/
class Scene
{
public:
	Scene(int id, std::string name);
	Scene();
	virtual void init();
	virtual void cleanup();
	virtual void draw(sf::RenderWindow* w);
	virtual void update(sf::RenderWindow* w);
	virtual void input(sf::Event* e);
	void lock();
	void unlock();
	bool isActive();
	std::string getFriendlyName();
	int getID();
	void setID(int id);
protected:
	std::vector<sf::Sprite*> sprites;
	std::vector<sf::Sound*> sounds;
	std::vector<sf::Text*> text;
	std::vector<UIElement*> ui;
	std::vector<sf::Shape*> shapes;
	int sceneID;
	bool active = false;
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
	static void input(sf::Event* e);
	static void update(sf::RenderWindow* w);
	static int getCurrentSceneID();
	static Scene* getCurrentScene();
	static Scene* getScene(int id);
	static Scene* getNext(int id);
	static Scene* getPrev(int id);
	static void next();
	static void previous();
	static void set(int id);
	static void setNext(int id);
	static void setPrev(int id);
	static void addScene(Scene* scene);
	static void delScene(int id);
private:
	static std::map<int, Scene*> scenes;
	static int nextScene;
	static int prevScene;
	static int currScene;
	static bool initComplete;
	static bool cleanupComplete;
};
#endif