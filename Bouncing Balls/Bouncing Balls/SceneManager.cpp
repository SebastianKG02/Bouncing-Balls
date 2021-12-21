#include "SceneManager.h"

/*
SCENE DEFINITION
*/

//Default constructor for all scenes 
Scene::Scene(uint8_t id, std::string name) {
	this->sceneID = id;
	this->name = name;

	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
}

//Get localised name
std::string Scene::getFriendlyName() {
	return name;
}

//Simple cleanup method (deletes all maps, clears both ID vars)
void Scene::cleanup() {
	for (auto& sprite : sprites) {
		delete sprite;
	}

	for (auto& textm : text) {
		delete textm;
	}

	for (auto& sound : sounds) {
		delete sound;
	}

	for (auto& ui_e : ui) {
		ui_e->cleanup();
		delete ui_e;
	}
	sceneID = NULL;
	name.clear();
}

Scene::Scene() {
	this->sceneID = NULL;
	this->name = "";
}

void Scene::init() { 

}

//Simple drawing method, have to pass reference to current window for UI functionality
void Scene::draw(sf::RenderWindow* w) { 
	for (auto sprite : sprites) {
		w->draw(*sprite);
	}

	for (auto textm : text) {
		w->draw(*textm);
	}

	for (auto ui_e : ui) {
		w->draw(*ui_e->getSprite());
	}
}

void Scene::update(sf::RenderWindow* w) { }
void Scene::input() { }

//Simple get for scene's ID
int Scene::getID() {
	return sceneID;
}

//Set internal identifier (override)
void Scene::setID(int id) {
	this->sceneID = id;
}

/*
SCENEMANAGER DEFINITION
*/

//Set up SceneManager static variables
uint8_t SceneManager::nextScene = 0;
uint8_t SceneManager::prevScene = -1;
uint8_t SceneManager::currScene = -1;
std::map<uint8_t, Scene*> SceneManager::scenes = std::map<uint8_t, Scene*>();
bool SceneManager::initComplete = false;
bool SceneManager::cleanupComplete = false;

//Initalise SceneManager, register all scenes
void SceneManager::init() {
	DefaultScene *title = new DefaultScene(0, "Title");
	scenes.insert({ title->getID(), title });
}

//Clean up all resources used by scenes by calling their member cleanup function
void SceneManager::cleanup() {
	if (initComplete == true && cleanupComplete == false) {
		for (auto scene : scenes) {
			scene.second->cleanup();
			delete scene.second;
		}
	}
}

//Get current scene ID
uint8_t SceneManager::getCurrentSceneID() {
	return SceneManager::currScene;
}

//Get current scene
Scene* SceneManager::getCurrentScene() {
	return scenes.at(currScene);
}

//Get any scene by ID
Scene* SceneManager::getScene(int id) {
	std::cout << id << std::endl;
	return scenes.at(id);
}

//Switch to next scene
void SceneManager::next() {
	getScene(nextScene)->init();
	prevScene = currScene;
	currScene = nextScene;
	nextScene++;
}

//Set scene (override of system)
void SceneManager::set(uint8_t id) {
	prevScene = currScene;
	currScene = id;
}

//Set next scene (override)
void SceneManager::setNext(uint8_t id) {
	nextScene = id;
}

//Set previous scene (override)
void SceneManager::setPrev(uint8_t id) {
	prevScene = id;
}

Scene* SceneManager::getNext(int id) {
	return scenes.at(id);
}

Scene* SceneManager::getPrev(int id) {
	return scenes.at(id);
}

void SceneManager::delScene(uint8_t id) {
	if (scenes.find(id)->first == id) {
		scenes.erase(id);
	}
}

void SceneManager::addScene(Scene* scene) {
	try {
		scenes.insert({ scene->getID(), scene });
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
		std::cout << "[SM]Added scene " << scene->getID() << " (or '" << scene->getFriendlyName() << "')" << std::endl;
#endif
#endif
	}
	catch (std::exception e) {
#ifdef DEBUG_ENABLED
#if DEBUG_LEVEL >= DB_LEVEL_MINERR
		std::cout << "[SM]Could not scene " << scene->getID() << " (or '" << scene->getFriendlyName() << "')" << std::endl;
		std::cout << "[SM]Detail: " << e.what() << std::endl;
#endif
#endif
	}
}

//Switch to previous scene
void SceneManager::previous() {
	getScene(prevScene)->init();
	uint8_t prev = prevScene;
	prevScene = currScene;
	currScene = prev;
}

//Draw current scene
void SceneManager::draw(sf::RenderWindow* w) {
	scenes.at(currScene)->draw(w);
}

//Get input for current scene
void SceneManager::input() {
	scenes.at(currScene)->input();
}

//Update current scene
void SceneManager::update(sf::RenderWindow* w) {
	scenes.at(currScene)->update(w);
}