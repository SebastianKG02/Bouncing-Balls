#include "SettingsScene.h"

#define BTN_SCALE 0.025f

SettingsScene::SettingsScene(int id, std::string name) {
	this->sceneID = id;
	this->name = name;
	this->active = false;
}

void SettingsScene::init() {
	//Lock scene during initalisaton (unavailable for input / drawing)
	lock();
	clock = sf::Clock();
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	resize = false;
	//Set up easy centering variable
	centerX = Config::user_resolution.X() / 2;

	//Background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));

	//Title text
	text.push_back(new sf::Text(std::string("Settings"), *AssetManager::getFont("title"), 75));
	//Set real position
	text[0]->setPosition(centerX - (text[0]->getGlobalBounds().width / 2), Config::applyRDY(100));
	text[0]->setFillColor(sf::Color::Black);
	text[0]->setOutlineColor(sf::Color::White);
	text[0]->setOutlineThickness(3.5f);

	//Add back button & label
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"),new float[2]{ 0.1f, 0.1f }));
	ui[0]->getSprite()->setPosition(centerX - (ui[0]->getSprite()->getGlobalBounds().width / 2), (580));
	text.push_back(new sf::Text(std::string("Back"), *AssetManager::getFont("title"), 34));
	text[1]->setPosition(centerX - (text[1]->getGlobalBounds().width / 2), (580) + (text[1]->getGlobalBounds().height / 2));
	text[1]->setFillColor(sf::Color::Black);
	
	//Vsync enabled-------------------------------------------------------------------------------------------------------------------------------------------------
	text.push_back(new sf::Text(std::string("Vsync enabled: "), *AssetManager::getFont("title"),25));
	text[2]->setPosition(centerX - (text[2]->getGlobalBounds().width / 2) - (centerX / 2), text[0]->getPosition().y+text[0]->getGlobalBounds().height+Config::applyRDY(25));
	text[2]->setFillColor(sf::Color::Black);

	//Left button - 1
	ui.push_back(new UIButton(new float[2]{ text[2]->getPosition().x+text[2]->getGlobalBounds().width+25, text[2]->getPosition().y }, std::string("larrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[1]->getSprite()->move(0.f, ui[1]->getSprite()->getGlobalBounds().width / 2);

	//Vsync enabled value label
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 25));
	//Update text label
	if (Config::user_do_vsync == true) {
		text[3]->setString("Yes");
	}
	else {
		text[3]->setString("No");
	}
	text[3]->setPosition(centerX - (text[3]->getGlobalBounds().width / 2), text[2]->getPosition().y);
	text[3]->setFillColor(sf::Color::Black);

	//Right button - 2
	ui.push_back(new UIButton(new float[2]{ centerX + text[3]->getPosition().x-ui[1]->getSprite()->getPosition().x, text[2]->getPosition().y }, std::string("rarrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[2]->getSprite()->move(0.f, ui[2]->getSprite()->getGlobalBounds().width / 2);

	//Frame limit enabled-------------------------------------------------------------------------------------------------------------------------------------------
	text.push_back(new sf::Text(std::string("Frame limit enabled: "), *AssetManager::getFont("title"), 25));
	text[4]->setPosition(centerX - (text[4]->getGlobalBounds().width / 2) - (centerX / 2)-30, text[2]->getPosition().y+text[4]->getGlobalBounds().height+20);
	text[4]->setFillColor(sf::Color::Black);

	//Left button - 3
	ui.push_back(new UIButton(new float[2]{ text[4]->getPosition().x + text[4]->getGlobalBounds().width + 25, text[4]->getPosition().y }, std::string("larrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[3]->getSprite()->move(0.f, ui[3]->getSprite()->getGlobalBounds().width / 2);

	//Frame limit enabled value label
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 25));
	//Update text label
	if (Config::user_do_frame_limit == true) {
		text[5]->setString("Yes");
	}
	else {
		text[5]->setString("No");
	}
	text[5]->setPosition(centerX - (text[5]->getGlobalBounds().width / 2), text[4]->getPosition().y);
	text[5]->setFillColor(sf::Color::Black);

	//Right button - 4
	ui.push_back(new UIButton(new float[2]{ ui[2]->getSprite()->getPosition().x, text[4]->getPosition().y }, std::string("rarrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[4]->getSprite()->move(0.f, ui[4]->getSprite()->getGlobalBounds().width / 2);

	//Frame limit value---------------------------------------------------------------------------------------------------------------------------------------------
	text.push_back(new sf::Text(std::string("Frame limit value: "), *AssetManager::getFont("title"), 25));
	text[6]->setPosition(centerX - (text[6]->getGlobalBounds().width / 2) - (centerX / 2) - 17, text[4]->getPosition().y + text[6]->getGlobalBounds().height + 20);
	text[6]->setFillColor(sf::Color::Black);

	//Left button - 5
	ui.push_back(new UIButton(new float[2]{ text[6]->getPosition().x + text[6]->getGlobalBounds().width + 25, text[6]->getPosition().y }, std::string("larrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[5]->getSprite()->move(0.f, ui[5]->getSprite()->getGlobalBounds().width / 2);

	//Frame limit value label
	text.push_back(new sf::Text(std::string(std::to_string(Config::user_frame_limit_val)), *AssetManager::getFont("title"), 25));
	text[7]->setPosition(centerX - (text[7]->getGlobalBounds().width / 2), text[6]->getPosition().y);
	text[7]->setFillColor(sf::Color::Black);

	//Right button - 6
	ui.push_back(new UIButton(new float[2]{ ui[4]->getSprite()->getPosition().x, text[6]->getPosition().y }, std::string("rarrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[6]->getSprite()->move(0.f, ui[6]->getSprite()->getGlobalBounds().width / 2);

	//Fullscreen enabled--------------------------------------------------------------------------------------------------------------------------------------------
	text.push_back(new sf::Text(std::string("Fullscreen enabled: "), *AssetManager::getFont("title"), 25));
	text[8]->setPosition(centerX - (text[8]->getGlobalBounds().width / 2) - (centerX / 2) - 28, text[6]->getPosition().y + text[8]->getGlobalBounds().height + 20);
	text[8]->setFillColor(sf::Color::Black);

	//Left button - 7
	ui.push_back(new UIButton(new float[2]{ text[8]->getPosition().x + text[8]->getGlobalBounds().width + 25, text[8]->getPosition().y }, std::string("larrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[7]->getSprite()->move(0.f, ui[7]->getSprite()->getGlobalBounds().width / 2);

	//Fullscreen enabled value label
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 25));
	//Update text label
	if (Config::user_do_fullscreen == true) {
		text[9]->setString("Yes");
	}
	else {
		text[9]->setString("No");
	}
	text[9]->setPosition(centerX - (text[9]->getGlobalBounds().width / 2), text[8]->getPosition().y);
	text[9]->setFillColor(sf::Color::Black);

	//Right button - 8
	ui.push_back(new UIButton(new float[2]{ ui[6]->getSprite()->getPosition().x, text[8]->getPosition().y }, std::string("rarrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[8]->getSprite()->move(0.f, ui[8]->getSprite()->getGlobalBounds().width / 2);

	//RESOLUTION----------------------------------------------------------------------------------------------------------------------------------------------------
	text.push_back(new sf::Text(std::string("Resolution: "), *AssetManager::getFont("title"), 25));
	text[10]->setPosition(centerX - (text[10]->getGlobalBounds().width / 2) - (centerX / 2) + 22, text[8]->getPosition().y + text[10]->getGlobalBounds().height + 20);
	text[10]->setFillColor(sf::Color::Black);

	//Left button - 9
	ui.push_back(new UIButton(new float[2]{ text[10]->getPosition().x + text[10]->getGlobalBounds().width + 25, text[10]->getPosition().y }, std::string("larrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[9]->getSprite()->move(0.f, ui[9]->getSprite()->getGlobalBounds().width / 2);

	//Resolution size label
	text.push_back(new sf::Text(std::string(Config::user_resolution.name()), *AssetManager::getFont("title"), 25));
	text[11]->setPosition(centerX - (text[11]->getGlobalBounds().width / 2), text[10]->getPosition().y);
	text[11]->setFillColor(sf::Color::Black);

	//Right button - 10
	ui.push_back(new UIButton(new float[2]{ ui[8]->getSprite()->getPosition().x, text[10]->getPosition().y }, std::string("rarrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[10]->getSprite()->move(0.f, ui[10]->getSprite()->getGlobalBounds().width / 2);

	//Controls
	ui.push_back(new UIButton(new float[2]{ centerX, text[10]->getPosition().y+120 }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[11]->getSprite()->move(-2*(ui[11]->getSprite()->getGlobalBounds().width), 0);
	text.push_back(new sf::Text(std::string("Controls"), *AssetManager::getFont("title"), 34));
	text[12]->setPosition(ui[11]->getSprite()->getPosition());
	text[12]->move(text[12]->getGlobalBounds().width / 2 - 20, text[12]->getGlobalBounds().height / 2);
	text[12]->setFillColor(sf::Color::Black);

	//Save button
	ui.push_back(new UIButton(new float[2]{ centerX, ui[11]->getSprite()->getPosition().y }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[12]->getSprite()->move(-ui[11]->getSprite()->getGlobalBounds().width + 20, 0);
	text.push_back(new sf::Text(std::string("Save"), *AssetManager::getFont("title"), 34));
	text[13]->setPosition(ui[12]->getSprite()->getPosition());
	text[13]->move((ui[12]->getSprite()->getGlobalBounds().width / 2) - (text[13]->getGlobalBounds().width / 2), text[13]->getGlobalBounds().height / 2);
	text[13]->setFillColor(sf::Color::Black);

	//Reset settings
	ui.push_back(new UIButton(new float[2]{ ui[12]->getSprite()->getPosition().x, ui[11]->getSprite()->getPosition().y }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[13]->getSprite()->move(ui[13]->getSprite()->getGlobalBounds().width + 20, 0);
	text.push_back(new sf::Text(std::string("Reset"), *AssetManager::getFont("title"), 34));
	text[14]->setPosition(ui[13]->getSprite()->getPosition());
	text[14]->move((ui[13]->getSprite()->getGlobalBounds().width / 2) - (text[14]->getGlobalBounds().width / 2), text[14]->getGlobalBounds().height / 2);
	text[14]->setFillColor(sf::Color::Black);

	//Refresh settings
	ui.push_back(new UIButton(new float[2]{ ui[13]->getSprite()->getPosition().x, ui[13]->getSprite()->getPosition().y }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[14]->getSprite()->move(ui[14]->getSprite()->getGlobalBounds().width + 20, 0);
	text.push_back(new sf::Text(std::string("Refresh"), *AssetManager::getFont("title"), 34));
	text[15]->setPosition(ui[14]->getSprite()->getPosition());
	text[15]->move((ui[14]->getSprite()->getGlobalBounds().width / 2) - (text[15]->getGlobalBounds().width / 2), text[15]->getGlobalBounds().height / 2);
	text[15]->setFillColor(sf::Color::Black);

	//SFX VOLUME----------------------------------------------------------------------------------------------------------------------------------------------------
	text.push_back(new sf::Text(std::string("SFX Volume: "), *AssetManager::getFont("title"), 25));
	text[16]->setPosition(centerX - (text[16]->getGlobalBounds().width / 2) - (centerX / 2) + 20, text[11]->getPosition().y + text[16]->getGlobalBounds().height + 20);
	text[16]->setFillColor(sf::Color::Black);

	//Left button - 15
	ui.push_back(new UIButton(new float[2]{ text[16]->getPosition().x + text[16]->getGlobalBounds().width + 25, text[16]->getPosition().y }, std::string("larrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[15]->getSprite()->move(0.f, ui[15]->getSprite()->getGlobalBounds().width / 2);

	//SFX volume label
	text.push_back(new sf::Text(std::string(std::to_string(Config::user_sfx_volume)), *AssetManager::getFont("title"), 25));
	text[17]->setPosition(centerX - (text[17]->getGlobalBounds().width / 2), text[16]->getPosition().y);
	text[17]->setFillColor(sf::Color::Black);

	//Right button - 16
	ui.push_back(new UIButton(new float[2]{ ui[10]->getSprite()->getPosition().x, text[16]->getPosition().y }, std::string("rarrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[16]->getSprite()->move(0.f, ui[16]->getSprite()->getGlobalBounds().width / 2);

	//MUSIC VOLUME----------------------------------------------------------------------------------------------------------------------------------------------------
	text.push_back(new sf::Text(std::string("Music Volume: "), *AssetManager::getFont("title"), 25));
	text[18]->setPosition(centerX - (text[18]->getGlobalBounds().width / 2) - (centerX / 2)+5, text[16]->getPosition().y + text[18]->getGlobalBounds().height + 20);
	text[18]->setFillColor(sf::Color::Black);

	//Left button - 17 
	ui.push_back(new UIButton(new float[2]{ text[18]->getPosition().x + text[18]->getGlobalBounds().width + 25, text[18]->getPosition().y }, std::string("larrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[17]->getSprite()->move(0.f, ui[17]->getSprite()->getGlobalBounds().width / 2);

	//Music volume label
	text.push_back(new sf::Text(std::string(std::to_string(Config::user_music_volume)), *AssetManager::getFont("title"), 25));
	text[19]->setPosition(centerX - (text[19]->getGlobalBounds().width / 2), text[18]->getPosition().y);
	text[19]->setFillColor(sf::Color::Black);

	//Right button - 18
	ui.push_back(new UIButton(new float[2]{ ui[16]->getSprite()->getPosition().x, text[18]->getPosition().y }, std::string("rarrow"), new float[2]{ BTN_SCALE, BTN_SCALE }));
	ui[18]->getSprite()->move(0.f, ui[18]->getSprite()->getGlobalBounds().width / 2);

	for (int i = 0; i < 19; i++) {
		ui[i]->lock();
	}
	clock.restart();
	//Initalisation finished, unlock scene
	unlock();
}

void SettingsScene::update(sf::RenderWindow* w) {
	//Check for screen res update
	if (resize == false) {
		sprites[0]->setPosition(0, 0);
		sprites[0]->setScale(Config::user_resolution.X() / sprites[0]->getGlobalBounds().width, Config::user_resolution.Y() / sprites[0]->getGlobalBounds().height);
		resize = true;
	}

	//Update UI
	for (auto ui_e : ui) {
		ui_e->update(w);
	}

	//Check if "back" button is pressed
	if (*ui[0]->getState() == UIState::CLICK) {
		SceneManager::setNext(0);
		SceneManager::next();
	}
	else if (clock.getElapsedTime().asMilliseconds() >= PAUSE_DELAY) {
		ui[0]->unlock();
	}

	//Vsync enabled buttons
	if (*ui[1]->getState() == UIState::CLICK || *ui[2]->getState() == UIState::CLICK) {
		if (Config::user_do_vsync == true) {
			text[3]->setString("No");
			Config::user_do_vsync = false;
			ui[1]->lock();
			ui[2]->lock();
		}
		else {
			text[3]->setString("Yes");
			Config::user_do_vsync = true;
			ui[1]->lock();
			ui[2]->lock();
		}
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[1]->unlock();
			ui[2]->unlock();
		}
	}

	//Frame limit enabled buttons
	if (*ui[3]->getState() == UIState::CLICK || *ui[4]->getState() == UIState::CLICK) {
		if (Config::user_do_frame_limit == true) {
			text[5]->setString("No");
			Config::user_do_frame_limit = false;
			ui[3]->lock();
			ui[4]->lock();
		}
		else {
			text[5]->setString("Yes");
			Config::user_do_frame_limit = true;
			ui[3]->lock();
			ui[4]->lock();
		}
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[3]->unlock();
			ui[4]->unlock();
		}
	}

	//Frame limit value LEFT button (decrement)
	if (*ui[5]->getState() == UIState::CLICK) {
		if (Config::user_frame_limit_val == 20) {
			text[7]->setString("240");
			Config::user_frame_limit_val = 240;
			ui[5]->lock();
		}
		else if (Config::user_frame_limit_val == 30) {
			text[7]->setString("20");
			Config::user_frame_limit_val = 20;
			ui[5]->lock();
		}
		else if (Config::user_frame_limit_val == 60) {
			text[7]->setString("30");
			Config::user_frame_limit_val = 30;
			ui[5]->lock();
		}
		else if (Config::user_frame_limit_val == 75) {
			text[7]->setString("60");
			Config::user_frame_limit_val = 60;
			ui[5]->lock();
		}
		else if (Config::user_frame_limit_val == 90) {
			text[7]->setString("75");
			Config::user_frame_limit_val = 75;
			ui[5]->lock();
		}
		else if (Config::user_frame_limit_val == 120) {
			text[7]->setString("90");
			Config::user_frame_limit_val = 90;
			ui[5]->lock();
		}
		else if (Config::user_frame_limit_val == 240) {
			text[7]->setString("120");
			Config::user_frame_limit_val = 120;
			ui[5]->lock();
		}
		else {
			text[7]->setString("20");
			Config::user_frame_limit_val = 20;
			ui[5]->lock();
		}
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[5]->unlock();
		}
	}

	//Frame limit value RIGHT button (increment)
	if (*ui[6]->getState() == UIState::CLICK) {
		if (Config::user_frame_limit_val == 20) {
			text[7]->setString("30");
			Config::user_frame_limit_val = 30;
			ui[6]->lock();
		}
		else if (Config::user_frame_limit_val == 30) {
			text[7]->setString("60");
			Config::user_frame_limit_val = 60;
			ui[6]->lock();
		}
		else if (Config::user_frame_limit_val == 60) {
			text[7]->setString("75");
			Config::user_frame_limit_val = 75;
			ui[6]->lock();
		}
		else if (Config::user_frame_limit_val == 75) {
			text[7]->setString("90");
			Config::user_frame_limit_val = 90;
			ui[6]->lock();
		}
		else if (Config::user_frame_limit_val == 90) {
			text[7]->setString("120");
			Config::user_frame_limit_val = 120;
			ui[6]->lock();
		}
		else if (Config::user_frame_limit_val == 120) {
			text[7]->setString("240");
			Config::user_frame_limit_val = 240;
			ui[6]->lock();
		}
		else if (Config::user_frame_limit_val == 240) {
			text[7]->setString("20");
			Config::user_frame_limit_val = 20;
			ui[6]->lock();
		}
		else {
			text[7]->setString("20");
			Config::user_frame_limit_val = 20;
			ui[6]->lock();
		}
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[6]->unlock();
		}
	}

	//Fullscreen enabled buttons
	if (*ui[7]->getState() == UIState::CLICK || *ui[8]->getState() == UIState::CLICK) {
		if (Config::user_do_fullscreen == true) {
			text[9]->setString("No");
			Config::user_do_fullscreen = false;
			ui[7]->lock();
			ui[8]->lock();
		}
		else {
			text[9]->setString("Yes");
			Config::user_do_fullscreen = true;
			ui[7]->lock();
			ui[8]->lock();
		}
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[7]->unlock();
			ui[8]->unlock();
		}
	}

	//Resolution setting LEFT button (decrement)
	if (*ui[9]->getState() == UIState::CLICK) {
		if (Config::user_resolution.X() == 640) {
			Config::user_resolution = Resolution(3840, 2160);
			text[11]->setString(Config::user_resolution.name());
			ui[9]->lock();
		}
		else if (Config::user_resolution.X() == 1280) {
			Config::user_resolution = Resolution(640, 480);
			text[11]->setString(Config::user_resolution.name());
			ui[9]->lock();
		}
		else if (Config::user_resolution.X() == 1920) {
			Config::user_resolution = Resolution(1280, 720);
			text[11]->setString(Config::user_resolution.name());
			ui[9]->lock();
		}
		else if (Config::user_resolution.X() == 2560) {
			Config::user_resolution = Resolution(1920, 1080);
			text[11]->setString(Config::user_resolution.name());
			ui[9]->lock();
		}
		else if (Config::user_resolution.X() == 3840) {
			Config::user_resolution = Resolution(2560, 1440);
			text[11]->setString(Config::user_resolution.name());
			ui[9]->lock();
		}
		else {
			Config::user_resolution = Resolution(640, 480);
			text[11]->setString(Config::user_resolution.name());
			ui[9]->lock();
		}
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[9]->unlock();
		}
	}
	
	//Resolution setting RIGHT button (increment)
	if (*ui[10]->getState() == UIState::CLICK) {
		if (Config::user_resolution.X() == 640) {
			Config::user_resolution = Resolution(1280, 720);
			text[11]->setString(Config::user_resolution.name());
			ui[10]->lock();
		}
		else if (Config::user_resolution.X() == 1280) {
			Config::user_resolution = Resolution(1920, 1080);
			text[11]->setString(Config::user_resolution.name());
			ui[10]->lock();
		}
		else if (Config::user_resolution.X() == 1920) {
			Config::user_resolution = Resolution(2560, 1440);
			text[11]->setString(Config::user_resolution.name());
			ui[10]->lock();
		}
		else if (Config::user_resolution.X() == 2560) {
			Config::user_resolution = Resolution(3840, 2160);
			text[11]->setString(Config::user_resolution.name());
			ui[10]->lock();
		}
		else if (Config::user_resolution.X() == 3840) {
			Config::user_resolution = Resolution(640, 480);
			text[11]->setString(Config::user_resolution.name());
			ui[10]->lock();
		}
		else {
			Config::user_resolution = Resolution(640, 480);
			text[11]->setString(Config::user_resolution.name());
			ui[10]->lock();
		}
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[10]->unlock();
		}
	}

	//Controls button
	if (*ui[11]->getState() == UIState::CLICK) {
		SceneManager::setNext(2);
		SceneManager::next();
	}
	else if ((*ui[11]->getState() == UIState::LOCK && clock.getElapsedTime().asMilliseconds() > 300)) {
		ui[11]->unlock();
	}

	//Save button
	if (*ui[12]->getState() == UIState::CLICK) {
		//Update inernal config values
		Config::getSettings()->setValue("vsync", Utils::boolToString(Config::user_do_vsync), true);
		Config::getSettings()->setValue("frame_limit", Utils::boolToString(Config::user_do_frame_limit), true);
		Config::getSettings()->setValue("fullscreen", Utils::boolToString(Config::user_do_fullscreen), true);
		Config::getSettings()->setValue("frame_limit_value", std::to_string(Config::user_frame_limit_val), true);
		Config::getSettings()->setValue("resolution", Config::user_resolution.name(), true);
		//Save to file
		Config::saveCurrentConfig("config.cfg");
		resize = false;
		Config::applySettings(w);
	}
	else if ((*ui[12]->getState() == UIState::LOCK && clock.getElapsedTime().asMilliseconds() > 300)) {
		ui[12]->unlock();
	}

	//Reset button
	if (*ui[13]->getState() == UIState::CLICK) {
		//Re-load config
		Config::loadFromConfig("config.cfg");

		//Update vsync label
		if (Config::user_do_vsync == 1) {
			text[3]->setString("Yes");
		}
		else {
			text[3]->setString("No");
		}

		//Update frame limit label
		if (Config::user_do_frame_limit == 1) {
			text[5]->setString("Yes");
		}
		else {
			text[5]->setString("No");
		}

		//update frame limit value label
		text[7]->setString(std::to_string(Config::user_frame_limit_val));

		//Update fullscreen enable label
		if (Config::user_do_fullscreen == 1) {
			std::cout << Config::user_do_fullscreen << std::endl;
			text[9]->setString("Yes");
		}
		else {
			text[9]->setString("No");
		}

		//Update resolution label
		text[11]->setString(Config::user_resolution.name());
	}
	else if ((*ui[13]->getState() == UIState::LOCK && clock.getElapsedTime().asMilliseconds() > 300)) {
		ui[13]->unlock();
	}

	//Refresh button
	if (*ui[14]->getState() == UIState::CLICK) {
		//Update vsync label
		if (Config::user_do_vsync == 1) {
			text[3]->setString("Yes");
		}
		else {
			text[3]->setString("No");
		}

		//Update frame limit label
		if (Config::user_do_frame_limit == 1) {
			text[5]->setString("Yes");
		}
		else {
			text[5]->setString("No");
		}

		//update frame limit value label
		text[7]->setString(std::to_string(Config::user_frame_limit_val));

		//Update fullscreen enable label
		if (Config::user_do_fullscreen == 1) {
			text[9]->setString("Yes");
		}
		else {
			text[9]->setString("No");
		}

		//Update resolution label
		text[11]->setString(Config::user_resolution.name());
	}
	else if ((*ui[14]->getState() == UIState::LOCK && clock.getElapsedTime().asMilliseconds() > 300)) {
		ui[14]->unlock();
	}

	//SFX volume LEFT button (decrement)
	if (*ui[15]->getState() == UIState::CLICK) {
		switch (Config::user_sfx_volume) {
		case 0:
			Config::user_sfx_volume = 10;
			break;
		case 1:
			Config::user_sfx_volume--;
			break;
		case 2:
			Config::user_sfx_volume--;
			break;
		case 3:
			Config::user_sfx_volume--;
			break;
		case 4:
			Config::user_sfx_volume--;
			break;
		case 5:
			Config::user_sfx_volume--;
			break;
		case 6:
			Config::user_sfx_volume--;
			break;
		case 7:
			Config::user_sfx_volume--;
			break;
		case 8:
			Config::user_sfx_volume--;
			break;
		case 9:
			Config::user_sfx_volume--;
			break;
		case 10:
			Config::user_sfx_volume--;
			break;
		default:
			Config::user_sfx_volume = 0;
			break;
		}
		text[17]->setString(std::to_string(Config::user_sfx_volume));
		ui[15]->lock();
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[15]->unlock();
		}
	}

	//SFX volume RIGHT button (increment)
	if (*ui[16]->getState() == UIState::CLICK) {
		switch (Config::user_sfx_volume) {
		case 0:
			Config::user_sfx_volume++;
			break;
		case 1:
			Config::user_sfx_volume++;
			break;
		case 2:
			Config::user_sfx_volume++;
			break;
		case 3:
			Config::user_sfx_volume++;
			break;
		case 4:
			Config::user_sfx_volume++;
			break;
		case 5:
			Config::user_sfx_volume++;
			break;
		case 6:
			Config::user_sfx_volume++;
			break;
		case 7:
			Config::user_sfx_volume++;
			break;
		case 8:
			Config::user_sfx_volume++;
			break;
		case 9:
			Config::user_sfx_volume++;
			break;
		case 10:
			Config::user_sfx_volume = 0;
			break;
		default:
			Config::user_sfx_volume = 0;
			break;
		}
		text[17]->setString(std::to_string(Config::user_sfx_volume));
		ui[16]->lock();
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[16]->unlock();
		}
	}

	//Music volume LEFT button (decrement)
	if (*ui[17]->getState() == UIState::CLICK) {
		switch (Config::user_music_volume) {
		case 0:
			Config::user_music_volume = 10;
			break;
		case 1:
			Config::user_music_volume--;
			break;
		case 2:
			Config::user_music_volume--;
			break;
		case 3:
			Config::user_music_volume--;
			break;
		case 4:
			Config::user_music_volume--;
			break;
		case 5:
			Config::user_music_volume--;
			break;
		case 6:
			Config::user_music_volume--;
			break;
		case 7:
			Config::user_music_volume--;
			break;
		case 8:
			Config::user_music_volume--;
			break;
		case 9:
			Config::user_music_volume--;
			break;
		case 10:
			Config::user_music_volume--;
			break;
		default:
			Config::user_music_volume = 0;
			break;
		}
		text[19]->setString(std::to_string(Config::user_music_volume));
		ui[17]->lock();
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[17]->unlock();
		}
	}

	//SFX volume RIGHT button (increment)
	if (*ui[18]->getState() == UIState::CLICK) {
		switch (Config::user_music_volume) {
		case 0:
			Config::user_music_volume++;
			break;
		case 1:
			Config::user_music_volume++;
			break;
		case 2:
			Config::user_music_volume++;
			break;
		case 3:
			Config::user_music_volume++;
			break;
		case 4:
			Config::user_music_volume++;
			break;
		case 5:
			Config::user_music_volume++;
			break;
		case 6:
			Config::user_music_volume++;
			break;
		case 7:
			Config::user_music_volume++;
			break;
		case 8:
			Config::user_music_volume++;
			break;
		case 9:
			Config::user_music_volume++;
			break;
		case 10:
			Config::user_music_volume = 0;
			break;
		default:
			Config::user_music_volume = 0;
			break;
		}
		text[19]->setString(std::to_string(Config::user_music_volume));
		ui[18]->lock();
		clock.restart();
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() > PAUSE_DELAY) {
			ui[18]->unlock();
		}
	}
}

void SettingsScene::input(sf::Event* e) {

}