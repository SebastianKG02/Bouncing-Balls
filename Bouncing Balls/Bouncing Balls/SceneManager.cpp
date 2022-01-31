#include "SceneManager.h"
#include "SettingsScene.h"
#include "ControlsScene.h"
#include "PlaySelectScene.h"
#include "StatsScene.h"
#include "CampaignInfoScene.h"
#include "EndlessInfoScene.h"
#include "ChallengeInfoScene.h"
#include "CampaignSelectScene.h"
#include "PlayCampaignScene.h"
#include "CampVictoryScene.h"
#include "CampLoseScene.h"
#include "EndlessPreGameScene.h"
#include "EndlessPlayScene.h"
#include "EndlessVictoryScene.h"
#include "EndlessLoseScene.h"
#include "ChallengePreScene.h"
#include "ChallengePlayScene.h"
#include "ChallengeVictoryScene.h"
#include "ChallengeLoseScene.h"

/*
SCENE DEFINITION
*/

//Default constructor for all scenes 
Scene::Scene(int id, std::string name) {
	this->sceneID = id;
	this->name = name;
	this->active = false;
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
}

void Scene::lock() {
	if (active == true) {
		active = false;
	}
}

void Scene::unlock() {
	if (active == false) {
		active = true;
	}
}

bool Scene::isActive() {
	return active;
}

//Get localised name
std::string Scene::getFriendlyName() {
	return name;
}

//Simple cleanup method (deletes all maps, clears both ID vars)
void Scene::cleanup() {
	sprites.clear();
	text.clear();
	sounds.clear();
	ui.clear();

	
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
		//ui_e->cleanup();
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
//Added functionality to draw hitboxes
void Scene::draw(sf::RenderWindow* w) { 
	//debug only - set up ghost hitbox
#if DB_SHOW_HITBOXES == true
	sf::RectangleShape rect = sf::RectangleShape();
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(3.f);
#endif

	//Draw sprites
	for (auto sprite : sprites) {
		w->draw(*sprite);
#if DB_SHOW_HITBOXES == true
		//Draw sprite hitbox
		rect.setPosition(sprite->getPosition());
		rect.setSize(sf::Vector2f(sprite->getGlobalBounds().width, sprite->getGlobalBounds().height));
		w->draw(rect);
#endif
	}

	//Draw UI elements
	for (auto ui_e : ui) {
		w->draw(*ui_e->getSprite());
#if DB_SHOW_HITBOXES == true
		//Draw UIE hitbox
		rect.setPosition(ui_e->getSprite()->getPosition());
		rect.setSize(sf::Vector2f(ui_e->getSprite()->getGlobalBounds().width, ui_e->getSprite()->getGlobalBounds().height));
		w->draw(rect);
#endif
	}

	//Draw text
	for (auto textm : text) {
		w->draw(*textm);
#if DB_SHOW_HITBOXES == true
		//Draw text hitboxes
		rect.setPosition(textm->getPosition());
		rect.setSize(sf::Vector2f(textm->getGlobalBounds().width, textm->getGlobalBounds().height));
		w->draw(rect);
#endif
	}

	//Draw any primitive shapes
	for (auto shape : shapes) {
		w->draw(*shape);
	}
}

void Scene::update(sf::RenderWindow* w) { }
void Scene::input(sf::Event* e) { 
}

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
int SceneManager::nextScene = 0;
int SceneManager::prevScene = -1;
int SceneManager::currScene = -1;
std::map<int, Scene*> SceneManager::scenes = std::map<int, Scene*>();
bool SceneManager::initComplete = false;
bool SceneManager::cleanupComplete = false;
std::vector<GameSettings*> SceneManager::camp_settings = std::vector<GameSettings*>();

//Initalise SceneManager, register all scenes
void SceneManager::init() {
	scenes = std::map<int, Scene*>();
	DefaultScene *title = new DefaultScene(0, "Title");
	scenes.insert({ title->getID(), title });

	SettingsScene* settings = new SettingsScene(1, "Settings");
	scenes.insert({ settings->getID(), settings });

	ControlsScene* controls = new ControlsScene(2, "Controls");
	scenes.insert({ controls->getID(), controls });

	PlaySelectScene* play_select = new PlaySelectScene(3, "Level Select");
	scenes.insert({ play_select->getID(), play_select});

	StatsScene* stats = new StatsScene(4, "Stats");
	scenes.insert({ stats->getID(), stats });

	CampaignInfoScene* camp_info = new CampaignInfoScene(5, "Campaign Info");
	scenes.insert({ camp_info->getID(), camp_info });

	EndlessInfoScene* endless_info = new EndlessInfoScene(6, "Endless Info");
	scenes.insert({endless_info->getID(), endless_info});

	ChallengeInfoScene* challenge_info = new ChallengeInfoScene(7, "Challenge Info");
	scenes.insert({challenge_info->getID(), challenge_info});

	CampaignSelectScene* camp_select = new CampaignSelectScene(8, "Campaign Select");
	scenes.insert({ camp_select->getID(), camp_select });

	for (int i = 0; i < 6; i++) {
		camp_settings.push_back(new GameSettings);
		camp_settings[i]->level = i;
		camp_settings[i]->rows = camp_settings[i]->rows + (camp_settings[i]->level * GM_LEVEL_DIFF_MOD_ROWS);
		if (i == 0){
			camp_settings[i]->maxColours = 2;
		}
		else {
			camp_settings[i]->maxColours = 2 + ((camp_settings[i]->level * GM_LEVEL_DIFF_MOD_COLOUR/2));
		}
		camp_settings[i]->time_l2 = camp_settings[i]->time_l2 + (camp_settings[i]->time_l2 * (camp_settings[i]->level * GM_LEVEL_DIFF_MOD_L2));
		camp_settings[i]->time_l3 = camp_settings[i]->time_l3 + (camp_settings[i]->time_l3 * (camp_settings[i]->level * GM_LEVEL_DIFF_MOD_L3));
	}
	
	PlayCampScene* camp_l1 = new PlayCampScene(9, "Campaign Level");
	scenes.insert({camp_l1->getID(), camp_l1});

	CampVictoryScene* camp_win = new CampVictoryScene(10, "Winner");
	scenes.insert({ camp_win->getID(), camp_win });

	CampLoseScene* camp_lose = new CampLoseScene(11, "Loser");
	scenes.insert({ camp_lose->getID(), camp_lose });

	EndlessPreGameScene* endless_pre = new EndlessPreGameScene(12, "Endless Pregame");
	scenes.insert({ endless_pre->getID(), endless_pre });

	EndlessPlayScene* endless_play = new EndlessPlayScene(13, "Endless Play");
	scenes.insert({endless_play->getID(), endless_play});

	EndlessVictoryScene* endless_win = new EndlessVictoryScene(14, "Endless Victory");
	scenes.insert({endless_win->getID(), endless_win});

	EndlessLoseScene* endless_lose = new EndlessLoseScene(15, "Endless Lose");
	scenes.insert({endless_lose->getID(), endless_lose});

	ChallengePreScene* chal_pre = new ChallengePreScene(16, "Challenge Pregame");
	scenes.insert({ chal_pre->getID(), chal_pre });

	ChallengePlayScene* chal_play = new ChallengePlayScene(17, "Challenge Play");
	scenes.insert({ chal_play->getID(), chal_play});

	ChallengeVictoryScene* chal_win = new ChallengeVictoryScene(18, "Challenge Win");
	scenes.insert({ chal_win->getID(), chal_win });

	ChallengeLoseScene* chal_lose = new ChallengeLoseScene(19, "Challenge Lose");
	scenes.insert({ chal_lose->getID(), chal_lose});
}

GameSettings* SceneManager::getCampSettings(int num) {
	return camp_settings[num];
}

//Clean up all resources used by scenes by calling their member cleanup function
void SceneManager::cleanup() {
	if (initComplete == true && cleanupComplete == false) {
		for (auto& scene : scenes) {
			scene.second->cleanup();
			delete &scene;
		}
	}

	for (auto& gs : camp_settings) {
		delete gs;
	}
}

//Get current scene ID
int SceneManager::getCurrentSceneID() {
	return SceneManager::currScene;
}

//Get current scene
Scene* SceneManager::getCurrentScene() {
	return scenes.at(currScene);
}

//Get any scene by ID
Scene* SceneManager::getScene(int id) {
	try {
		return scenes.at(id);
	}
	catch (std::exception e) {
		return nullptr;
	}
}

//Switch to next scene
void SceneManager::next() {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_MINERR
	std::cout << "[SM]Moving to scene [" << nextScene << "]<'" << getScene(nextScene)->getFriendlyName() << "'>" << std::endl;
	#endif
#endif
	if (prevScene > -1 && (getScene(prevScene) != nullptr)) {
		getScene(prevScene)->cleanup();
	}

	if (currScene > -1) {
		getScene(currScene)->lock();
	}

	prevScene = currScene;
	currScene = nextScene;

	for (;;) {
		if (prevScene > -1 && getScene(prevScene)->isActive() == false) {
			getScene(prevScene)->lock();
			getScene(currScene)->unlock();
			break;
		}
		else if (prevScene < 0) {
			getScene(currScene)->unlock();
			break;
		}
	}

	getScene(currScene)->init();
	getScene(currScene)->unlock();
}

//Set scene (override of system)
void SceneManager::set(int id) {
	prevScene = currScene;
	currScene = id;
}

//Set next scene (override)
void SceneManager::setNext(int id) {
	nextScene = id;
}

//Set previous scene (override)
void SceneManager::setPrev(int id) {
	prevScene = id;
}

Scene* SceneManager::getNext(int id) {
	return scenes.at(id);
}

Scene* SceneManager::getPrev(int id) {
	return scenes.at(id);
}

void SceneManager::delScene(int id) {
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
	int prev = prevScene;
	prevScene = currScene;
	currScene = prev;
}

//Draw current scene
void SceneManager::draw(sf::RenderWindow* w) {
	scenes.at(currScene)->draw(w);
}

//Get input for current scene
void SceneManager::input(sf::Event* e) {
	//std::cout << currScene << std::endl;
	//std::cout << scenes.at(currScene) << std::endl;
	scenes.at(currScene)->input(e);
}

//Update current scene
void SceneManager::update(sf::RenderWindow* w) {
	//std::cout << currScene << std::endl;
	//std::cout << scenes.at(currScene) << std::endl;
	if (scenes.at(currScene)->isActive() == true) {
		scenes.at(currScene)->update(w);
	}
}