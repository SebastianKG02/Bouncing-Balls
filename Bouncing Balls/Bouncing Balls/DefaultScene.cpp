#include "DefaultScene.h"
#include "UIButton.h"

void DefaultScene::init() {
	text.push_back(new sf::Text(std::string("Bouncing Balls"), *AssetManager::getFont("title"), 48));
	text[0]->setPosition(520, 300);
	ui.push_back(new UIButton(200, 200, "btn_short_active", 0.05f, 0.05f));
	ui[0]->setStateTex(UIState::ACTIVE, "btn_short_active");
	ui[0]->setStateTex(UIState::HOVER, "btn_short_hover");
	ui[0]->setStateTex(UIState::CLICK, "btn_short_click");
	ui[0]->setStateTex(UIState::INACTIVE, "btn_short_lock");

}

void DefaultScene::update(sf::RenderWindow* w) {
	for (auto ui_e : ui) {
		ui_e->update(w);
	}

	if (*ui[0]->getState() == UIState::CLICK) {
		w->close();
	}
}

void DefaultScene::input() {

}

DefaultScene::DefaultScene(uint8_t id, std::string name) {
	this->sceneID = id;
	this->name = name;
}