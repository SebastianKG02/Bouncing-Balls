#include "SceneManager.h"

//Default constructor for all scenes 
Scene::Scene(uint8_t id, std::string name) {
	this->sceneID = id;
	this->name = name;

	std::vector<sf::Sprite*> sprties = std::vector<sf::Sprite*>();
	std::vector<sf::Sound*> sounds = std::vector<sf::Sound*>();
	std::vector<sf::Text*> text = std::vector <sf::Text*>();
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
	sceneID = NULL;
	name.clear();
}

Scene::Scene() {

}

void Scene::init() { 

}

void Scene::draw(sf::RenderWindow* w) { 
	for (auto sprite : sprites) {
		w->draw(*sprite);
	}

	for (auto textm : text) {
		w->draw(*textm);
	}
}

void Scene::update() { }
void Scene::input() { }

//Simple get for scene's ID
int Scene::getID() {
	return sceneID;
}

//Set internal identifier (override)
void Scene::setID(int id) {
	this->sceneID = id;
}

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

}

//Set next scene (override)
void SceneManager::setNext(uint8_t id) {

}

//Set previous scene (override)
void SceneManager::setPrev(uint8_t id) {

}

//Switch to previous scene
void SceneManager::previous() {

}

//Draw current scene
void SceneManager::draw(sf::RenderWindow* w) {
	scenes.at(currScene)->draw(w);
}