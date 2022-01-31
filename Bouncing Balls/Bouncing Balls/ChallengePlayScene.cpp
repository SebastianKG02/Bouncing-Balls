#include "ChallengePlayScene.h"
#include "PlayerData.h"

//Default constructor
ChallengePlayScene::ChallengePlayScene(int id, std::string name) {
	this->name = name;
	this->sceneID = id;
}

void ChallengePlayScene::init() {
	//Lock scene during initalisaton (unavailable for input / drawing)
	lock();
	//Initalise needed scene variables
	clock = sf::Clock();
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	balls = std::vector<Ball*>();
	//Initalise local variables
	resize = false;
	fired = true;
	status[0] = false;
	status[1] = false;
	started = false;
	paused = false;
	playerScore = 0;
	sprites_map_begin = 0;
	sprites_map_end = 0;
	time_elapsed = 0;
	nextBalls = std::vector<BallSimple*>();
	Config::loadFromConfig("config.cfg");
	//Set up easy centering variable
	centerX = Config::user_resolution.X() / 2;
	num_balls = 0;
	maxCols = 2;
	//Load generation rules
	this->rules = new GameSettings();
	this->rules->level = Player::getData()->challenge_level;
	this->rules->rows = 6 + (rules->level * GM_CHALLENGE_ROW_MOD_LVL);
	this->rules->maxColours = rules->minColours + ((rules->level * GM_LEVEL_DIFF_MOD_COLOUR) / 2);
	if (rules->maxColours > 17) {
		rules->maxColours = 17;
	}
	this->rules->minColours = 2;
	this->rules->time_l2 = (rules->time_l2 + (rules->time_l2 * (rules->level * GM_LEVEL_DIFF_MOD_L2))) * GM_CHALLENGE_OBJ2_TIME_MOD;
	this->rules->time_l3 = (rules->time_l3 + (rules->time_l3 * (rules->level * GM_LEVEL_DIFF_MOD_L3))) * GM_CHALLENGE_OBJ3_TIME_MOD;
	//Calculating proper start_x for ballMap
	//Create reference sprite
	sf::Sprite ref = sf::Sprite(*AssetManager::getTexture("ball_red"));
	//Set scale to be same as normal sprite
	ref.setScale(0.1f, 0.1f);
	float map_width = ref.getGlobalBounds().width * rules->cols;
	float scaled_start = Config::applyRDY(1280) / 2;
	//Ensure proper scaling of difficulty
	maxCols = rules->cols;
	map = BallMap(rules->rows, rules->cols, rules->minColours, rules->maxColours, scaled_start - (map_width / 2), Config::applyRDY(ref.getGlobalBounds().height), -1);

	//Initalise sound effects
	//Ball hit (same colour) sound effect
	sounds.push_back(new sf::Sound(*AssetManager::getSound("pop")));
	sounds[0]->setVolume(10 * Config::user_sfx_volume);

	//Background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));

	//Add back button & label
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[0]->getSprite()->setPosition((map.getStart().x / 2) - (ui[0]->getSprite()->getGlobalBounds().width / 2), Config::user_resolution.Y() - 15.f - ui[0]->getSprite()->getGlobalBounds().height);
	text.push_back(new sf::Text(std::string("Exit"), *AssetManager::getFont("title"), 34));
	text[0]->setPosition(ui[0]->getSprite()->getPosition());
	text[0]->move(ui[0]->getSprite()->getGlobalBounds().width / 2, ui[0]->getSprite()->getGlobalBounds().height / 2);
	text[0]->move(-text[0]->getGlobalBounds().width / 2, (-text[0]->getGlobalBounds().height / 2) - 2.5f);
	text[0]->setFillColor(sf::Color::Black);

	//Level identifier label
	text.push_back(new sf::Text(std::string("Challenge\nLevel " + std::to_string(rules->level)), *AssetManager::getFont("title"), 60));
	text[1]->setPosition(ui[0]->getSprite()->getPosition().x, Config::applyRDY(25));
	text[1]->move((ui[0]->getSprite()->getGlobalBounds().width / 2) - (text[1]->getGlobalBounds().width / 2), 0);
	text[1]->setFillColor(sf::Color::Black);
	text[1]->setOutlineThickness(1.5f);
	text[1]->setOutlineColor(sf::Color::White);

	//Score label
	text.push_back(new sf::Text(std::string("Score:"), *AssetManager::getFont("title"), 50));
	text[2]->setPosition(text[1]->getPosition());
	text[2]->move((+text[1]->getGlobalBounds().width / 2) - (text[2]->getGlobalBounds().width / 2), text[1]->getGlobalBounds().height + Config::applyRDY(20));
	text[2]->setFillColor(sf::Color::Black);
	text[2]->setOutlineThickness(1.5f);
	text[2]->setOutlineColor(sf::Color::White);
	//Score value label
	text.push_back(new sf::Text(std::string(std::to_string(playerScore)), *AssetManager::getFont("title"), 50));
	text[3]->setOutlineThickness(1.5f);
	text[3]->setOutlineColor(sf::Color::Black);
	updateScoreLabel();

	//Start button
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[1]->getSprite()->setPosition(ui[0]->getSprite()->getPosition());
	ui[1]->getSprite()->move(0.f, -ui[1]->getSprite()->getGlobalBounds().height - 2.5f);
	//Start button label
	text.push_back(new sf::Text(std::string("Start"), *AssetManager::getFont("title"), 34));
	text[4]->setPosition(ui[1]->getSprite()->getPosition());
	text[4]->move((ui[1]->getSprite()->getGlobalBounds().width / 2) - (text[4]->getGlobalBounds().width / 2), (+text[4]->getGlobalBounds().height / 2) - 5.f);
	text[4]->setFillColor(sf::Color::Black);

	//Pause status
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 24));
	text[5]->setFillColor(sf::Color::Black);
	updatePauseLabel();

	//Pause button
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[2]->getSprite()->setPosition(text[5]->getPosition());
	ui[2]->getSprite()->move((text[5]->getGlobalBounds().width / 2) - (ui[2]->getSprite()->getGlobalBounds().width / 2), -ui[1]->getSprite()->getGlobalBounds().height + 2.5f);
	//Pause button label
	text.push_back(new sf::Text(std::string("Pause"), *AssetManager::getFont("title"), 34));
	text[6]->setPosition(ui[2]->getSprite()->getPosition());
	text[6]->move((ui[2]->getSprite()->getGlobalBounds().width / 2) - (text[6]->getGlobalBounds().width / 2), (+text[6]->getGlobalBounds().height / 2) - 2.5f);
	text[6]->setFillColor(sf::Color::Black);

	//Starting countdown label
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 34));
	text[7]->setPosition(centerX, Config::user_resolution.Y() / 2);
	text[7]->move(0 - (text[7]->getGlobalBounds().width / 2), (-text[7]->getGlobalBounds().height / 2));
	text[7]->setFillColor(sf::Color::Black);

	//Register cannon sprites
	p1Cannon = Cannon(centerX, Config::user_resolution.Y() - Config::applyRDY(32), true);
	p1Cannon.getBase()->move(0, -(p1Cannon.getBase()->getGlobalBounds().height * 4));
	sprites.push_back(p1Cannon.getBase());
	sprites.push_back(p1Cannon.getBody());
	sprites.push_back(p1Cannon.getAxel());

	//Register next balls
	for (int i = 0; i < 3; i++) {
		nextBalls.push_back(new BallSimple());
		nextBalls[i]->alive = false;
		nextBalls[i]->col = static_cast<BallColour>((rand() % (rules->maxColours)));
		nextBalls[i]->spr = new sf::Sprite(*AssetManager::getTexture("ball_" + Ball::ballColourToStr(nextBalls[i]->col)));
		nextBalls[i]->spr->setScale(0.1f, 0.1f);
		sprites.push_back(nextBalls[i]->spr);
		nextBalls[i]->spr_id = sprites.size() - 1;
	}

	//Ensure next balls have correct positioning
	nextBalls[0]->spr->setPosition(centerX, Config::user_resolution.Y() - 2 * nextBalls[0]->spr->getGlobalBounds().height);
	nextBalls[0]->spr->move((p1Cannon.getBase()->getGlobalBounds().width / 2) + (nextBalls[0]->spr->getGlobalBounds().width * 0.75f), +(nextBalls[0]->spr->getGlobalBounds().height / 2));
	nextBalls[1]->spr->setPosition(nextBalls[0]->spr->getPosition());
	nextBalls[1]->spr->move(nextBalls[0]->spr->getGlobalBounds().width + 2.5f, 0);
	nextBalls[2]->spr->setPosition(nextBalls[1]->spr->getPosition());
	nextBalls[2]->spr->move(nextBalls[1]->spr->getGlobalBounds().width + 2.5f, 0);

	//Register tracing line for shot accuracy tracking
	tracer = sf::RectangleShape(sf::Vector2f(2, 1000));
	//shapes.push_back(&tracer);

	left_bound = sf::RectangleShape(sf::Vector2f(8, Config::user_resolution.Y() * 16));
	left_bound.setPosition(-5 + (map.getStart().x) - 8, 0);
	left_bound.setFillColor(sf::Color::Transparent);
	left_bound.setOutlineThickness(3.f);
	left_bound.setOutlineColor(sf::Color::Red);
	left_bound.setFillColor(sf::Color::Black);
	shapes.push_back(&left_bound);

	right_bound = sf::RectangleShape(sf::Vector2f(8, Config::user_resolution.Y() * 16));
	right_bound.setPosition(map.getEnd().x + 5, 0);
	right_bound.setFillColor(sf::Color::Transparent);
	right_bound.setOutlineThickness(3.f);
	right_bound.setOutlineColor(sf::Color::Red);
	right_bound.setFillColor(sf::Color::Black);
	shapes.push_back(&right_bound);

	death_bound = sf::RectangleShape(sf::Vector2f(right_bound.getPosition().x - left_bound.getPosition().x + 8, 8));
	death_bound.setPosition(left_bound.getPosition().x, Config::user_resolution.Y() - p1Cannon.getBase()->getPosition().y - Config::applyRDY(275));
	death_bound.setFillColor(sf::Color::Transparent);
	death_bound.setOutlineThickness(3.f);
	death_bound.setOutlineColor(sf::Color::Red);
	death_bound.setFillColor(sf::Color::Black);
	shapes.push_back(&death_bound);

	ball_eraser = sf::RectangleShape(sf::Vector2f(right_bound.getPosition().x - left_bound.getPosition().x - 16, 64));
	ball_eraser.setPosition(map.getStart().x - 1, map.getEnd().y - 32);
	ball_eraser.setFillColor(sf::Color(90, 90, 90));
	ball_eraser.setOutlineThickness(3.f);
	ball_eraser.setOutlineColor(sf::Color::Black);
	shapes.push_back(&ball_eraser);

	//Clock sprite
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("clock")));
	sprites[7]->setScale(0.5f, 0.5f);
	sprites[7]->setPosition(Config::user_resolution.X() * 0.75 + (sprites[7]->getGlobalBounds().width / 2), 2.5f + (sprites[7]->getGlobalBounds().height) / 2);
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 45));
	text[8]->setFillColor(sf::Color::Black);
	text[8]->setOutlineThickness(1.5f);
	text[8]->setOutlineColor(sf::Color::White);
	text[8]->setPosition(sprites[sprites.size() - 1]->getPosition());
	text[8]->move(sprites[sprites.size() - 1]->getGlobalBounds().width + 2.5f, (sprites[sprites.size() - 1]->getGlobalBounds().height / 2) - (text[8]->getGlobalBounds().height / 2));

	//Rest of the sprites vector is reserved for the map and player balls
	sprites_map_begin = sprites.size();

	//Objectives label
	text.push_back(new sf::Text(std::string("Objectives:"), *AssetManager::getFont("title"), 45));
	text[9]->setPosition(sprites[7]->getPosition().x + (sprites[7]->getGlobalBounds().width / 2) - (text[9]->getGlobalBounds().width / 2), sprites[7]->getPosition().y + (1.f * sprites[7]->getGlobalBounds().height));
	text[9]->setFillColor(sf::Color::Black);
	text[9]->setOutlineThickness(1.5f);
	text[9]->setOutlineColor(sf::Color::White);

	//OBJECTIVE 1 -----------------------------------------------------------------------------------------------------------------------------------------------------------------OBJECTIVE 1
	//Objective 1 - clear all balls (label)
	text.push_back(new sf::Text(std::string("Clear all balls"), *AssetManager::getFont("title"), 32));
	text[10]->setPosition(text[9]->getPosition());
	text[10]->move((text[9]->getGlobalBounds().width / 2) - (text[10]->getGlobalBounds().width / 2), 2.5f + text[9]->getGlobalBounds().height);
	text[10]->setFillColor(sf::Color::Black);
	text[10]->setOutlineThickness(1.5f);
	text[10]->setOutlineColor(sf::Color::White);

	//Objective 1 - clear all balls (value)
	text.push_back(new sf::Text(std::string("Balls left: " + std::to_string(num_balls)), *AssetManager::getFont("title"), 30));
	text[11]->setPosition(text[10]->getPosition());
	text[11]->move((text[10]->getGlobalBounds().width / 2) - (text[11]->getGlobalBounds().width / 2), text[10]->getGlobalBounds().height);
	text[11]->setFillColor(sf::Color::Black);
	text[11]->setOutlineThickness(1.5f);
	text[11]->setOutlineColor(sf::Color::White);

	//Objective 1 - star identifier
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("star_lock")));
	sprites[8]->setPosition(text[10]->getPosition());
	sprites[8]->setScale(0.2f, 0.2f);
	sprites[8]->move((text[10]->getGlobalBounds().width / 2) - (sprites[8]->getGlobalBounds().width / 2), text[10]->getGlobalBounds().height + text[11]->getGlobalBounds().height + 2.5f);

	//OBJECTIVE 2 -----------------------------------------------------------------------------------------------------------------------------------------------------------------OBJECTIVE 2
	//Objective 2 - complete in 120s (label)
	text.push_back(new sf::Text(std::string("Clear all in \nless than " + std::to_string(rules->time_l2) + "s"), *AssetManager::getFont("title"), 30));
	text[12]->setPosition(sprites[8]->getPosition());
	text[12]->move((sprites[8]->getGlobalBounds().width / 2) - (text[12]->getGlobalBounds().width / 2), 2.5f + sprites[8]->getGlobalBounds().height);
	text[12]->setFillColor(sf::Color::Black);
	text[12]->setOutlineThickness(1.5f);
	text[12]->setOutlineColor(sf::Color::White);

	//Objective 2 - star identifier
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("star_lock")));
	sprites[9]->setPosition(text[12]->getPosition());
	sprites[9]->setScale(0.2f, 0.2f);
	sprites[9]->move((text[12]->getGlobalBounds().width / 2) - (sprites[9]->getGlobalBounds().width / 2), text[12]->getGlobalBounds().height + 2.5f);

	//OBJECTIVE 3 -----------------------------------------------------------------------------------------------------------------------------------------------------------------OBJECTIVE 3
	//Objective 3 - complete in 100s (label)
	text.push_back(new sf::Text(std::string("Clear all in \nless than " + std::to_string(rules->time_l3) + "s"), *AssetManager::getFont("title"), 30));
	text[13]->setPosition(sprites[9]->getPosition());
	text[13]->move((sprites[9]->getGlobalBounds().width / 2) - (text[13]->getGlobalBounds().width / 2), 2.5f + sprites[9]->getGlobalBounds().height);
	text[13]->setFillColor(sf::Color::Black);
	text[13]->setOutlineThickness(1.5f);
	text[13]->setOutlineColor(sf::Color::White);


	//Objective 3 - star identifier
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("star_lock")));
	sprites[10]->setPosition(text[13]->getPosition());
	sprites[10]->setScale(0.2f, 0.2f);
	sprites[10]->move((text[13]->getGlobalBounds().width / 2) - (sprites[10]->getGlobalBounds().width / 2), text[13]->getGlobalBounds().height + 2.5f);

	//Coins held label
	text.push_back(new sf::Text(std::string("Coins:"), *AssetManager::getFont("title"), 50));
	text[14]->setPosition(text[1]->getPosition().x, text[3]->getPosition().y);
	text[14]->move((text[1]->getGlobalBounds().width / 2) - (text[14]->getGlobalBounds().width / 2), text[3]->getGlobalBounds().height * 1.5f);
	text[14]->setFillColor(sf::Color::Black);
	//Coins held value label
	text.push_back(new sf::Text(std::to_string(Player::getData()->coins), *AssetManager::getFont("title"), 50));
	text[15]->setPosition(text[14]->getPosition());
	text[15]->move((text[14]->getGlobalBounds().width / 2) - (text[15]->getGlobalBounds().width / 2), text[14]->getGlobalBounds().height * 1.1f);
	text[15]->setOutlineThickness(1.5f);
	text[15]->setOutlineColor(sf::Color::Black);

	//2X points powerups held sprite
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("power_points")));
	sprites[11]->setPosition(text[1]->getPosition().x, text[15]->getPosition().y);
	sprites[11]->setScale(0.5f, 0.5f);
	sprites[11]->move((text[1]->getGlobalBounds().width / 2) - (sprites[11]->getGlobalBounds().width), text[15]->getGlobalBounds().height + Config::applyRDY(25));
	//Value label
	text.push_back(new sf::Text(std::to_string(Player::getData()->num_powerups[0]), *AssetManager::getFont("title"), 50));
	text[16]->setPosition(sprites[11]->getPosition());
	text[16]->move(-text[16]->getGlobalBounds().width - Config::applyRDX(7.5f), (sprites[11]->getGlobalBounds().height / 2) - (text[16]->getGlobalBounds().height));
	text[16]->setFillColor(sf::Color::Black);
	text[16]->setOutlineThickness(1.5f);
	text[16]->setOutlineColor(sf::Color::White);

	//time skip powerups held sprite
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("power_time")));
	sprites[12]->setPosition(sprites[11]->getPosition());
	sprites[12]->setScale(0.5f, 0.5f);
	sprites[12]->move(sprites[11]->getGlobalBounds().width, 0);
	//Value label
	text.push_back(new sf::Text(std::to_string(Player::getData()->num_powerups[1]), *AssetManager::getFont("title"), 50));
	text[17]->setPosition(sprites[12]->getPosition());
	text[17]->move(sprites[12]->getGlobalBounds().width + (text[17]->getGlobalBounds().width / 2), (sprites[12]->getGlobalBounds().height / 2) - (text[17]->getGlobalBounds().height));
	text[17]->setFillColor(sf::Color::Black);
	text[17]->setOutlineThickness(1.5f);
	text[17]->setOutlineColor(sf::Color::White);

	//2x powerup descriptor
	text.push_back(new sf::Text(std::string("Double points\nfor 10s"), *AssetManager::getFont("title"), 18));
	text[18]->setPosition(text[16]->getPosition());
	text[18]->move(-(text[18]->getGlobalBounds().width / 2) - Config::applyRDX(5), +(text[16]->getGlobalBounds().height / 2) + text[18]->getGlobalBounds().height);
	text[18]->setFillColor(sf::Color::Black);
	text[18]->setOutlineThickness(1.5f);
	text[18]->setOutlineColor(sf::Color::White);

	//time skip powerup descriptor
	text.push_back(new sf::Text(std::string("Shave 10s off\nthe Clock!"), *AssetManager::getFont("title"), 18));
	text[19]->setPosition(sprites[12]->getPosition().x, text[17]->getPosition().y);
	text[19]->move(+(text[19]->getGlobalBounds().width / 2) - Config::applyRDX(5), +(text[17]->getGlobalBounds().height / 2) + text[19]->getGlobalBounds().height);
	text[19]->setFillColor(sf::Color::Black);
	text[19]->setOutlineThickness(1.5f);
	text[19]->setOutlineColor(sf::Color::White);

	//Buy powerups label
	text.push_back(new sf::Text(std::string("Buy Powerups!"), *AssetManager::getFont("title"), 34));
	text[20]->setPosition(sprites[10]->getPosition());
	text[20]->move((sprites[10]->getGlobalBounds().width / 2) - (text[20]->getGlobalBounds().width / 2), sprites[10]->getGlobalBounds().height + Config::applyRDY(5));
	text[20]->setFillColor(sf::Color::Black);
	text[20]->setOutlineThickness(1.5f);
	text[20]->setOutlineColor(sf::Color::White);

	//2X Points indicator
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("power_points")));
	sprites[13]->setScale(0.5f, 0.5f);
	sprites[13]->setPosition(text[20]->getPosition().x + (text[20]->getGlobalBounds().width / 4) - (sprites[13]->getGlobalBounds().width / 2.f), text[20]->getPosition().y + text[20]->getGlobalBounds().height + Config::applyRDY(5));
	ui.push_back(new UIButton(new float[2]{ sprites[13]->getPosition().x, sprites[13]->getPosition().y }, std::string("long"), new float[2]{ 0.05f, 0.05f }));
	ui[3]->getSprite()->move(-(ui[3]->getSprite()->getGlobalBounds().width / 4.f), sprites[13]->getGlobalBounds().height + Config::applyRDY(5));
	text.push_back(new sf::Text(std::string(std::to_string(GM_COST_PWP_2XPTS) + " coins"), *AssetManager::getFont("title"), 28));
	text[21]->setPosition(ui[3]->getSprite()->getPosition());
	text[21]->move((ui[3]->getSprite()->getGlobalBounds().width / 2) - (text[21]->getGlobalBounds().width / 2), (ui[3]->getSprite()->getGlobalBounds().height / 2) - (text[21]->getGlobalBounds().height / 2) - 3.f);
	text[21]->setFillColor(sf::Color::Black);

	//10s skip indicator
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("power_time")));
	sprites[14]->setScale(0.5f, 0.5f);
	sprites[14]->setPosition(sprites[13]->getPosition().x + (2 * sprites[14]->getGlobalBounds().width), text[20]->getPosition().y + text[20]->getGlobalBounds().height + Config::applyRDY(5));
	ui.push_back(new UIButton(new float[2]{ sprites[14]->getPosition().x, sprites[14]->getPosition().y }, std::string("long"), new float[2]{ 0.05f, 0.05f }));
	ui[4]->getSprite()->move(-(ui[4]->getSprite()->getGlobalBounds().width / 4.f), sprites[13]->getGlobalBounds().height + Config::applyRDY(5));
	text.push_back(new sf::Text(std::string(std::to_string(GM_COST_PWP_TMSKP) + " coins"), *AssetManager::getFont("title"), 28));
	text[22]->setPosition(ui[4]->getSprite()->getPosition());
	text[22]->move((ui[4]->getSprite()->getGlobalBounds().width / 2) - (text[22]->getGlobalBounds().width / 2), (ui[4]->getSprite()->getGlobalBounds().height / 2) - (text[22]->getGlobalBounds().height / 2) - 3.f);
	text[22]->setFillColor(sf::Color::Black);

	//Add use powerup 1 button
	ui.push_back(new UIButton(new float[2]{ text[16]->getPosition().x, text[16]->getPosition().y }, std::string("long"), new float[2]{ 0.025f, 0.025f }));
	ui[5]->getSprite()->move(-(ui[5]->getSprite()->getGlobalBounds().width / 2), -(ui[5]->getSprite()->getGlobalBounds().height));
	text.push_back(new sf::Text(std::string("Use"), *AssetManager::getFont("title"), 20));
	text[23]->setPosition(ui[5]->getSprite()->getPosition());
	text[23]->move((ui[5]->getSprite()->getGlobalBounds().width / 2) - (text[23]->getGlobalBounds().width / 2), (ui[5]->getSprite()->getGlobalBounds().height / 2) - (text[23]->getGlobalBounds().height / 2) - Config::applyRDY(2));
	text[23]->setFillColor(sf::Color::Black);

	//Add use powerup 2 button
	ui.push_back(new UIButton(new float[2]{ text[17]->getPosition().x + text[17]->getGlobalBounds().width, text[17]->getPosition().y }, std::string("long"), new float[2]{ 0.025f, 0.025f }));
	ui[6]->getSprite()->move(-(ui[6]->getSprite()->getGlobalBounds().width / 2), -(ui[6]->getSprite()->getGlobalBounds().height));
	text.push_back(new sf::Text(std::string("Use"), *AssetManager::getFont("title"), 20));
	text[24]->setPosition(ui[6]->getSprite()->getPosition());
	text[24]->move((ui[6]->getSprite()->getGlobalBounds().width / 2) - (text[24]->getGlobalBounds().width / 2), (ui[6]->getSprite()->getGlobalBounds().height / 2) - (text[24]->getGlobalBounds().height / 2) - Config::applyRDY(2));
	text[24]->setFillColor(sf::Color::Black);

	//Register map balls as rendered sprites
	for (auto& row : map.getMap()) {
		for (auto& ball : row->row) {
			ball->spr_id = sprites.size();
			sprites.push_back(ball->spr);
			//Increase ball counter
			num_balls++;
		}
	}

	sprites_map_end = sprites.size() - 1;
	//Unlock scene and get ready to function as complete scene
	clock.restart();
	unlock();

}

void ChallengePlayScene::update(sf::RenderWindow* w) {
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

	p1Cannon.update(w);
	tracer.setPosition(p1Cannon.getBody()->getPosition());
	tracer.setRotation(p1Cannon.getBody()->getRotation() + 180);

	sf::Vector2i last_alive = sf::Vector2i(0, 0);

	//Main gameplay logic
	if (status[0] == false && status[1] == false) {
		if (started == false) {
			//If start button pressed
			if (*ui[1]->getState() == UIState::CLICK || sf::Keyboard::isKeyPressed(Config::user_key_start)) {
				game_clock.restart();
				ui[1]->lock();
			}
			//If start button previously pressed
			else if (*ui[1]->getState() == UIState::LOCK) {
				//If countdown over
				if (game_clock.getElapsedTime().asMilliseconds() > 3000) {
					started = true;
					text[7]->setString("Go!");
					text[7]->setPosition(centerX, Config::user_resolution.Y() / 2);
					text[7]->move(0 - (text[7]->getGlobalBounds().width / 2), (-text[7]->getGlobalBounds().height / 2));
					game_clock.restart();
				}
				//If counting
				else {
					text[7]->setString("Starting in... " + std::to_string((game_clock.getElapsedTime().asMilliseconds() + 1000) / 1000));
					text[7]->setPosition(centerX, Config::user_resolution.Y() / 2);
					text[7]->move(0 - (text[7]->getGlobalBounds().width / 2), (-text[7]->getGlobalBounds().height / 2));
				}
			}
		}
		else {

			if (paused == true) {
				updatePauseLabel();
			}
			else {
				//Update score value 
				updatePauseLabel();
				updateScoreLabel();
				updateClockLabel();
				updateBallsLeft();

				if (time_elapsed > 1) {
					text[7]->setString("");
				}

				//Check for level 3 objective completion
				if (time_elapsed > rules->time_l3) {
					sprites[10]->setTexture(*AssetManager::getTexture("star_lock"));
					rules->obj_l3 = false;
				}
				else {
					if (rules->obj_l3 != true) {
						sprites[10]->setTexture(*AssetManager::getTexture("star_full"));
						rules->obj_l3 = true;
					}
				}

				//Check for level 2 objective completion
				if (time_elapsed > rules->time_l2) {
					sprites[9]->setTexture(*AssetManager::getTexture("star_lock"));
					rules->obj_l2 = false;
				}
				else {
					if (rules->obj_l2 != true) {
						sprites[9]->setTexture(*AssetManager::getTexture("star_full"));
						rules->obj_l2 = true;
					}
				}

				//Shooting mechanism
				if ((sf::Keyboard::isKeyPressed(Utils::charToKey(Config::getSettings()->getValue("key_shoot"))) && fired == false) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && fired == false) {
					clock.restart();
					balls.push_back(new Ball(p1Cannon.getBody()->getPosition().x, p1Cannon.getBody()->getPosition().y, getNextColour()));

					if (p1Cannon.getBody()->getRotation() == 0) {
						balls[balls.size() - 1]->path_plot(Config::user_resolution.Y(), p1Cannon.getBody()->getRotation() + 0.00001f, sf::Vector2f(p1Cannon.getBody()->getPosition().x, p1Cannon.getBody()->getPosition().y - p1Cannon.getBody()->getGlobalBounds().height));
						balls[balls.size() - 1]->setID(sprites.size());
					}
					else {
						balls[balls.size() - 1]->path_plot(Config::user_resolution.Y(), p1Cannon.getBody()->getRotation(), p1Cannon.getBody()->getPosition());
						balls[balls.size() - 1]->setID(sprites.size());
					}
					*balls[balls.size() - 1]->alive() = true;
					sprites.push_back(balls[balls.size() - 1]->getSprite());
					fired = true;
					num_balls++;
				}
				else {
					if (clock.getElapsedTime().asMilliseconds() >= 1000 * GM_BALL_SHOOT_DELAY) {
						fired = false;
					}
				}

				//Update player balls
				if (balls.size() > 0) {
					int it = -1;
					for (auto& ball : balls) {
						it++;
						bool hasHit = false;

						//Only update balls if they are alive
						if (*ball->alive() == true) {
							ball->update();

							//if ball touches left map bound, kill ball
							if (ball->getSprite()->getGlobalBounds().intersects(left_bound.getGlobalBounds())) {
								*ball->alive() = false;
								ball->getSprite()->setColor(sf::Color::Transparent);
								num_balls--;
								continue;
							}

							//if ball touches right map bound, kill ball
							if (ball->getSprite()->getGlobalBounds().intersects(right_bound.getGlobalBounds())) {
								*ball->alive() = false;
								ball->getSprite()->setColor(sf::Color::Transparent);
								num_balls--;
								continue;
							}

							//if ball touches top map bound, kill ball
							if (ball->getSprite()->getGlobalBounds().intersects(ball_eraser.getGlobalBounds())) {
								*ball->alive() = false;
								ball->getSprite()->setColor(sf::Color::Transparent);
								num_balls--;
								continue;
							}


							//Gather necessary information for collision detection
							float ball_x = ball->getSprite()->getPosition().x;
							float ball_y = ball->getSprite()->getPosition().y;
							float bwidth = ball->getSprite()->getGlobalBounds().width;
							float bheight = ball->getSprite()->getGlobalBounds().height;

							//If ball has reached ballmap
							if (ball_y < map.getBall(0, 0)->spr->getPosition().y + bheight) {

								//If ball has surpassed entire map
								if (ball_y < map.getBall(map.getMap().size() - 1, 0)->spr->getPosition().y) {
									*ball->alive() = false;
									ball->getSprite()->setColor(sf::Color::Transparent);
									num_balls--;
								}
								else {
									//If ball is between the start and end of the map
									//Loop through each row
									for (int x = 0; x < map.getMap().size(); x++) {
										//If ball has collided  with this row
										if (ball_y <= map.getBall(x, 0)->spr->getPosition().y + bheight && ball_y > map.getBall(x, 0)->spr->getPosition().y) {
											//Loop through each column 
											for (int y = 0; y < map.getRow(x)->row.size(); y++) {
												//Check if player ball is inside map ball
												if ((ball_x >= map.getBall(x, y)->spr->getPosition().x && ball_x < map.getBall(x, y)->spr->getPosition().x + bwidth)) {
													if (map.getBall(x, y)->alive == true && *ball->alive() == true) {
														//If ball is valid
														if (map.getBall(x, y)->spr->getColor() != sf::Color::Transparent && map.getBall(x, y)->spr->getColor() == sf::Color::White) {
															//If ball hit is of same colour
															if (map.getBall(x, y)->col == *ball->getColour()) {
																//initalise kill list
																std::vector<sf::Vector2i*> kill_list = std::vector<sf::Vector2i*>();

																//Add current ball to kill list
																kill_list.push_back(new sf::Vector2i(x, y));
																bool finished = false;

																//Determine larger hitbox radius
																sf::Vector2i min_pos = sf::Vector2i(ball_x - (bwidth * 1.5f), ball_y - (bheight * 2.5f));
																sf::Vector2i max_pos = sf::Vector2i(ball_x + (bwidth * 1.5f), ball_y + (bheight * 1.5f));

																short mod_x = 0;
																if (x > 0) {
																	mod_x = 1;
																}

																short max = 2;
																if (x >= map.getMap().size()) {
																	max = 0;
																}

																for (int kx = x - mod_x; kx < (x + max); kx++) {
																	short mod_y_s = 0;
																	short mod_y_e = 2;
																	if (y > 0) {
																		mod_y_s = 1;
																	}

																	if (kx >= map.getMap().size()) {
																		break;
																	}

																	if (y >= map.getRow(kx)->row.size()) {
																		mod_y_e = 1;
																	}
																	for (int ky = (y - mod_y_s); ky < map.getRow(kx)->row.size(); ky++) {
																		BallSimple* map_ball = map.getBall(kx, ky);

																		bool hasFound = false;
																		//Check if this ball collided
																		if (map_ball->spr->getPosition().x > min_pos.x - 2.5f && map_ball->spr->getPosition().x < max_pos.x + 2.5f) {
																			if (map_ball->spr->getPosition().y > min_pos.y - 2.5f && map_ball->spr->getPosition().y < max_pos.y + 2.5f) {
																				if (map_ball->alive == true) {
																					if (map_ball->col == *ball->getColour()) {
																						kill_list.push_back(new sf::Vector2i(kx, ky));
																						hasFound = true;
																					}
																					else {
																						if (hasFound == true) {
																							break;
																						}
																					}
																				}
																				else {
																					if (hasFound == true) {
																						break;
																					}
																				}
																			}
																		}
																	}
																}

																//If 3 or more balls hit
																if (kill_list.size() > 2) {
																	for (auto& kpos : kill_list) {
																		map.getBall(kpos->x, kpos->y)->spr->setColor(sf::Color::Transparent);
																		map.getBall(kpos->x, kpos->y)->alive = false;
																		playerScore += (1 + (rules->level * GM_CHALLENGE_PTS_MOD_LVL)) * pts_modifier * GM_PTS_PER_BALL;
																		sounds[0]->play();
																		num_balls--;
																		delete kpos;
																	}
																	ball->getSprite()->setColor(sf::Color::Transparent);
																	*ball->alive() = false;
																}
																else {
																	//addRowReassignSprites();
																	bool complete = false;
																	short mod = 0;
																	if (x < 1) {
																		addRowReassignSprites();
																		mod = 0;
																	}
																	else {
																		mod = 1;
																	}
																	for (int i = x - mod; i < x + 1; i++) {
																		for (int k = 0; k < map.getRow(i)->row.size(); k++) {
																			if (map.getBall(i, k)->spr->getGlobalBounds().intersects(ball->getSprite()->getGlobalBounds()) && map.getBall(i, k)->spr->getColor() == sf::Color::Transparent) {
																				map.getBall(i, k)->col = *ball->getColour();
																				map.getBall(i, k)->alive = true;
																				map.getBall(i, k)->spr->setColor(sf::Color::White);
																				map.getBall(i, k)->spr->setTexture(*AssetManager::getTexture("ball_" + Ball::ballColourToStr(map.getBall(i, k)->col)));
																				complete = true;
																				break;
																			}
																		}
																		if (complete == true) {
																			break;
																		}
																	}
																	ball->getSprite()->setColor(sf::Color::Transparent);
																	*ball->alive() = false;
																}
															}
															//If ball hit not same colour
															else {
																bool comp = false;
																short mod = 0;
																if (x == 0) {
																	addRowReassignSprites();
																	mod = 1;
																}
																for (int kx = 0; kx < (x + 1 + mod); kx++) {
																	for (int ky = 0; ky < map.getRow(kx)->row.size(); ky++) {
																		if (map.getBall(kx, ky)->spr->getGlobalBounds().intersects(ball->getSprite()->getGlobalBounds())) {
																			if (map.getBall(kx, ky)->spr->getColor() == sf::Color::Transparent) {
																				map.getBall(kx, ky)->spr->setColor(sf::Color::White);
																				map.getBall(kx, ky)->alive = true;
																				map.getBall(kx, ky)->col = *ball->getColour();
																				map.getBall(kx, ky)->spr->setTexture(*AssetManager::getTexture("ball_" + Ball::ballColourToStr(*ball->getColour())));
																				*ball->alive() = false;
																				ball->getSprite()->setColor(sf::Color::Transparent);
																				comp = true;
																				break;
																			}
																			else {
																				continue;
																			}
																		}
																	}
																	if (comp == true) {
																		break;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
						else {
							*ball->alive() = false;
							ball->getSprite()->setColor(sf::Color::Transparent);
							sprites.erase(sprites.begin() + sprites_map_end + it);
							if (it > 1) {
								balls.erase(balls.begin() + it);
							}
							else {
								balls.erase(balls.begin());
							}
							break;
						}
					}
				}
				//Update map ball location
				map.update(0, (GM_BALL_MOVE_SPEED * TICK_SPEED.asMilliseconds()) * (1 + (GM_CHALLENGE_SPD_MOD_LVL * rules->level)));
				//Update pneumatic press locatuib
				ball_eraser.move(0, (GM_BALL_MOVE_SPEED * TICK_SPEED.asMilliseconds()) * (1 + (GM_CHALLENGE_SPD_MOD_LVL * rules->level)));


				num_balls = 0;
				//Ball killer loop 
				for (int x = 0; x < map.getMap().size(); x++) {
					for (int y = 0; y < map.getRow(x)->row.size(); y++) {
						bool isAlive = map.getBall(x, y)->alive;
						bool ignore = false;
						sf::Color org_col = map.getBall(x, y)->spr->getColor();

						//If map ball has hit death barrier
						if (map.getBall(x, y)->alive == true && map.getBall(x, y)->spr->getGlobalBounds().intersects(death_bound.getGlobalBounds())) {
							status[0] = false;
							status[1] = true;
							break;
						}

						//Gravity check
						if (true) {
							if (isAlive == true && map.getBall(x, y)->spr->getColor() != sf::Color::Transparent) {
								num_balls++;
								last_alive.x = x;
								last_alive.y = y;
							}
							//If gravity has not sorted ball check left & right tethers
							if (map.getBall(x, y)->alive == true && map.getBall(x, y)->spr->getColor() != sf::Color::Transparent) {
								short checks_req = 0;
								short checks_comp = 0;
								//Always do right check if not last ball in row
								if (y < map.getRow(x)->row.size() - 1) {
									checks_req++;
									if (map.getBall(x, y + 1)->alive == false && map.getBall(x, y + 1)->spr->getColor() == sf::Color::Transparent) {
										checks_comp++;
									}
								}

								//Always do left check if not first ball in row
								if (y > 0) {
									checks_req++;
									if (map.getBall(x, y - 1)->alive == false && map.getBall(x, y - 1)->spr->getColor() == sf::Color::Transparent) {
										checks_comp++;
									}
								}

								//If at least one check required and number of checks required matches number of checks passed, kill ball
								if (checks_req > 0 && (checks_req <= checks_comp)) {
									ignore = true;
								}
								if (false) {
									if (x < map.getMap().size() - 1) {
										//if this row smaller than next row
										if (map.getRow(x)->row.size() < map.getRow(x + 1)->row.size()) {
											if (map.getBall(x + 1, y)->alive == false && map.getBall(x + 1, y)->spr->getColor() == sf::Color::Transparent) {
												if (map.getBall(x + 1, y + 1)->alive == false && map.getBall(x + 1, y + 1)->spr->getColor() == sf::Color::Transparent) {
													//If both balls above this ball are non-existent, delete this ball
													map.getBall(x, y)->alive = false;
													map.getBall(x, y)->spr->setColor(sf::Color::Transparent);
													playerScore += (1 + (rules->level * GM_CHALLENGE_PTS_MOD_LVL)) * pts_modifier * GM_PTS_PER_BALL;
													sounds[0]->play();
												}
											}
										}
										//If this row bigger than next row
										else {
											//If first ball in row
											if (y == 0) {
												if (map.getBall(x + 1, y)->alive == false && map.getBall(x + 1, y)->spr->getColor() == sf::Color::Transparent) {
													map.getBall(x, y)->alive = false;
													map.getBall(x, y)->spr->setColor(sf::Color::Transparent);
													playerScore += (1 + (rules->level * GM_CHALLENGE_PTS_MOD_LVL)) * pts_modifier * GM_PTS_PER_BALL;
													sounds[0]->play();
												}
											}

											//If middle ball
											if (y > 0 && y < map.getRow(x)->row.size() - 1) {
												if (map.getRow(x)->row.size() < map.getRow(x + 1)->row.size()) {
													if ((map.getBall(x + 1, y)->alive == false && map.getBall(x + 1, y)->spr->getColor() == sf::Color::Transparent)
														|| (map.getBall(x + 1, y + 1)->alive == false || map.getBall(x + 1, y + 1)->spr->getColor() == sf::Color::Transparent)) {
														//If both balls above this ball are non-existent, delete this ball
														map.getBall(x, y)->alive = false;
														map.getBall(x, y)->spr->setColor(sf::Color::Transparent);
														playerScore += (1 + (rules->level * GM_CHALLENGE_PTS_MOD_LVL)) * pts_modifier * GM_PTS_PER_BALL;
														sounds[0]->play();
													}
												}
											}

											//If last ball
											if (y == map.getRow(x)->row.size() - 1) {
												if (map.getBall(x + 1, y - 1)->alive == false && map.getBall(x + 1, y - 1)->spr->getColor() == sf::Color::Transparent) {
													map.getBall(x, y)->alive = false;
													map.getBall(x, y)->spr->setColor(sf::Color::Transparent);
													playerScore += (1 + (rules->level * GM_CHALLENGE_PTS_MOD_LVL)) * pts_modifier * GM_PTS_PER_BALL;
													sounds[0]->play();
												}
											}
										}
									}
								}
							}

							if (map.getBall(x, y)->alive != true) {
								map.getBall(x, y)->spr->setColor(sf::Color::Transparent);

							}
							else {
								//If not first rowd
								bool checks[8] = { false, false, false, false, false, false, false, false };
								if (x > 0) {
									if (y < map.getRow(x - 1)->row.size()) {
										if (map.getBall(x - 1, y)->alive != true || map.getBall(x - 1, y)->spr->getColor() == sf::Color::Transparent) {
											checks[0] = true;
										}
									}
									//LEFT * BELOW CHECK
									if (y > 0 && (map.getBall(x - 1, y - 1)->alive != true || map.getBall(x - 1, y - 1)->spr->getColor() == sf::Color::Transparent)) {
										checks[4] = true;
									}

									//RIGHT * BELOW CHECK
									if (y + 1 < map.getRow(x - 1)->row.size() && (map.getBall(x - 1, y + 1)->alive != true || map.getBall(x - 1, y + 1)->spr->getColor() == sf::Color::Transparent))
									{
										checks[6] = true;
									}
								}

								//If not last row
								if (x < map.getMap().size() && (y + 1) < map.getRow(x)->row.size()) {
									if (map.getBall(x, y + 1)->alive != true || map.getBall(x, y + 1)->spr->getColor() == sf::Color::Transparent) {
										checks[1] = true;
									}
								}

								//If not first ball in row
								if (y > 0) {
									if (map.getBall(x, y - 1)->alive != true || map.getBall(x, y - 1)->spr->getColor() == sf::Color::Transparent) {
										checks[2] = true;
									}
								}

								//If not last ball in row
								if (x < map.getMap().size() - 1 && y < map.getRow(x)->row.size() && y < map.getRow(x + 1)->row.size()) {
									if (map.getBall(x + 1, y)->alive != true || map.getBall(x + 1, y)->spr->getColor() == sf::Color::Transparent) {
										checks[3] = true;
									}
									//RIGHT ABOVE CHECK
									if (x < (map.getMap().size() - 1) && (y + 1) < map.getRow(x + 1)->row.size()) {
										if (map.getBall(x + 1, y + 1)->alive != true || map.getBall(x + 1, y + 1)->spr->getColor() == sf::Color::Transparent) {
											checks[5] = true;
										}
									}
								}

								//Count amount of checks complete
								short counter = 0;
								for (bool b : checks) {
									if (b) {
										counter++;
									}
								}

								//If at leasy 7/8 checks come back as true, delete ball and reward player with points
								if (counter >= 7) {
									map.getBall(x, y)->alive = false;
									map.getBall(x, y)->spr->setColor(sf::Color::Transparent);
									num_balls--;
									playerScore += (1 + (rules->level * GM_CHALLENGE_PTS_MOD_LVL)) * pts_modifier * GM_PTS_PER_BALL;
									sounds[0]->play();
								}
							}
						}
					}
					//If game lost, exit
					if (status[0] == false && status[1] == true) {
						break;
					}
				}
			}
		}
	}

	//Updare amount of balls left
	updateBallsLeft();

	//If game not won
	if (status[1] != true) {
		//If objective 1 has been met
		if (num_balls <= 0) {
			rules->obj_l1 = true;
			sprites[8]->setTexture(*AssetManager::getTexture("star_full"));
			status[0] = true;
		}
		else {
			//Security check
			if (num_balls == 1) {
				//Check if last ball is actually alive
				if (map.getBall(last_alive.x, last_alive.y)->alive != true || map.getBall(last_alive.x, last_alive.y)->spr->getColor() == sf::Color::Transparent) {
					num_balls--;
				}
			}
			if (rules->obj_l1 == true) {
				rules->obj_l1 = false;
				sprites[8]->setTexture(*AssetManager::getTexture("star_lock"));
			}
		}
	}
	//If won & not lost
	if (status[0] == true && status[1] == false) {
		//Reward player with points for level completion
		playerScore += pts_modifier * (rules->level * (1 + GM_CHALLENGE_PTS_MOD_LVL)) + GM_PTS_PER_WIN;

		//Reset stars gained for finishing level
		Player::getData()->challenge_last_stars = 0;
		//Reward player with points based on objective completion
		if (rules->obj_l1) {
			Player::getData()->challenge_last_stars++;
			playerScore += (rules->level * (1 + GM_CHALLENGE_PTS_MOD_LVL)) * GM_CHALLENGE_STAR_PTS;
		}

		if (rules->obj_l2) {
			Player::getData()->challenge_last_stars++;
			playerScore += (rules->level * (1 + GM_CHALLENGE_PTS_MOD_LVL)) * GM_CHALLENGE_STAR_PTS;
		}

		if (rules->obj_l3) {
			Player::getData()->challenge_last_stars++;
			playerScore += (rules->level * (1 + GM_CHALLENGE_PTS_MOD_LVL)) * GM_CHALLENGE_STAR_PTS;
		}

		//Reward player with coins
		Player::getData()->coins += GM_COIN_MOD * std::round(playerScore / 1000);
		rules->final_score = playerScore;
		Player::save();
		Player::getData()->challenge_level++;

		if (playerScore > Player::getData()->challenge_hs) {
			Player::getData()->challenge_hs = playerScore;
		}

		SceneManager::setNext(18);
		SceneManager::next();
	}

	//If not won & has lost
	if (status[0] == false && status[1] == true) {
		Player::save();
		SceneManager::setNext(19);
		SceneManager::next();
	}

	//Powerup 1 button logic
	if (*ui[5]->getState() == UIState::CLICK || sf::Keyboard::isKeyPressed(Config::user_key_pw_1)) {
		//If player has more than one powerup available not using 
		//Ensure only one powerup can be used at a tmie
		if (using_pw_1 != true) {
			//If player has more than one powerup available
			if (Player::getData()->num_powerups[0] >= 1) {
				//Take powerup
				Player::getData()->num_powerups[0]--;
				//Set flag
				using_pw_1 = true;
				ui[5]->lock();
				//Set expiry time for powerup (10 seconds from now)
				pw_1_time = time_elapsed + 10;
				pts_modifier = 1;
				text[16]->setString(std::to_string(Player::getData()->num_powerups[0]));
				text[16]->setPosition(sprites[11]->getPosition());
				text[16]->move(-text[16]->getGlobalBounds().width - Config::applyRDX(7.5f), (sprites[11]->getGlobalBounds().height / 2) - (text[16]->getGlobalBounds().height));
			}
			else {
				//If player does not have any powerups available BUT does have coins to buy one
				if (Player::getData()->coins >= GM_COST_PWP_2XPTS) {
					//Take coins away, function same as above
					Player::getData()->coins -= GM_COST_PWP_2XPTS;
					using_pw_1 = true;
					ui[5]->lock();
					//Set expiry time for powerup (10 seconds from now)
					pw_1_time = time_elapsed + 10;
					pts_modifier = 1;
					//Update coins held value label
					text[15]->setString(std::to_string(Player::getData()->coins));
					text[15]->setPosition(text[14]->getPosition());
					text[15]->move((text[14]->getGlobalBounds().width / 2) - (text[15]->getGlobalBounds().width / 2), text[14]->getGlobalBounds().height * 1.1f);
					//Update powerup 1 held value label
					text[16]->setString(std::to_string(Player::getData()->num_powerups[0]));
					text[16]->setPosition(sprites[11]->getPosition());
					text[16]->move(-text[16]->getGlobalBounds().width - Config::applyRDX(7.5f), (sprites[11]->getGlobalBounds().height / 2) - (text[16]->getGlobalBounds().height));
				}
			}

		}
	}
	//If button locked, unlock after powerup finished
	else if (*ui[5]->getState() == UIState::LOCK) {
		if (using_pw_1 == false) {
			ui[5]->unlock();
		}
	}

	//Time skip button
	if (*ui[6]->getState() == UIState::CLICK || sf::Keyboard::isKeyPressed(Config::user_key_pw_2)) {
		if (using_pw_2 != true) {
			//If player has powerup 2 available
			if (Player::getData()->num_powerups[1] >= 1) {
				//Take powerup
				Player::getData()->num_powerups[1]--;
				//Set flag
				using_pw_2 = true;
				//Lock button
				ui[6]->lock();
				//Deduct time
				if (time_elapsed > 10) {
					time_elapsed -= 10;
				}
				else {
					time_elapsed = 0;
				}
				//Update powerup held label
				text[17]->setString(std::to_string(Player::getData()->num_powerups[1]));
				text[17]->setPosition(sprites[12]->getPosition());
				text[17]->move(sprites[12]->getGlobalBounds().width + (text[17]->getGlobalBounds().width / 2), (sprites[12]->getGlobalBounds().height / 2) - (text[17]->getGlobalBounds().height));
				clock.restart();
				ui[6]->lock();
				text[8]->setFillColor(sf::Color::Green);
			}
			//If player has no powerups 2 available
			else if (Player::getData()->coins >= GM_COST_PWP_TMSKP) {
				Player::getData()->coins -= GM_COST_PWP_TMSKP;
				//Set flag
				using_pw_2 = true;
				//Lock button
				ui[6]->lock();
				//Deduct time
				if (time_elapsed > 10) {
					time_elapsed -= 10;
				}
				else {
					time_elapsed = 0;
				}
				//Update coin value label
				text[15]->setString(std::to_string(Player::getData()->coins));
				text[15]->setPosition(text[14]->getPosition());
				text[15]->move((text[14]->getGlobalBounds().width / 2) - (text[15]->getGlobalBounds().width / 2), text[14]->getGlobalBounds().height * 1.1f);
				//Update powerup held label
				text[17]->setString(std::to_string(Player::getData()->num_powerups[1]));
				text[17]->setPosition(sprites[12]->getPosition());
				text[17]->move(sprites[12]->getGlobalBounds().width + (text[17]->getGlobalBounds().width / 2), (sprites[12]->getGlobalBounds().height / 2) - (text[17]->getGlobalBounds().height));
				clock.restart();
				text[8]->setFillColor(sf::Color::Green);
				ui[6]->lock();
			}
		}
	}
	//If button locked, unlock after done using & 200ms has passed
	else if (*ui[6]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200 && using_pw_2 == true) {
			ui[6]->unlock();
			using_pw_2 = false;
			text[8]->setFillColor(sf::Color::Black);
		}
	}
	//Powerup 1 logic
	if (using_pw_1 == true) {
		if (time_elapsed < pw_1_time) {
			text[8]->setFillColor(sf::Color::Red);
			pts_modifier = 2;
		}
		else {
			text[8]->setFillColor(sf::Color::Black);
			pts_modifier = 1;
			using_pw_1 = false;
		}
	}

	//If back button pressed
	if (*ui[0]->getState() == UIState::CLICK) {
		clock.restart();
		ui[0]->lock();
		this->lock();
		SceneManager::setNext(16);
		SceneManager::next();
	}
	else if (*ui[0]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[0]->unlock();
		}
	}

	//Pause button & key mechanic
	//Pause key should ONLY act if the internal timer allows it to (mirroring the function of the button)
	if (*ui[2]->getState() == UIState::CLICK || (sf::Keyboard::isKeyPressed(Config::user_key_pause) && (*ui[2]->getState() != UIState::LOCK))) {
		if (paused) {
			paused = false;
			game_clock.restart();
			ui[2]->lock();
			clock.restart();
		}
		else {
			paused = true;
			ui[2]->lock();
			clock.restart();
		}
	}
	else if (*ui[2]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[2]->unlock();
		}
	}

	//Power up buying button (double points)
	if (*ui[3]->getState() == UIState::CLICK) {
		if (Player::getData()->coins >= GM_COST_PWP_2XPTS) {
			//Save new coin count
			Player::getData()->coins -= GM_COST_PWP_2XPTS;
			if (Player::getData()->num_powerups[0] < 1) {
				Player::getData()->num_powerups[0] = 0;
			}
			Player::getData()->num_powerups[0] += 1;
			Player::save();
			Player::load();
			//Update coin value label
			text[15]->setString(std::to_string(Player::getData()->coins));
			text[15]->setPosition(text[14]->getPosition());
			text[15]->move((text[14]->getGlobalBounds().width / 2) - (text[15]->getGlobalBounds().width / 2), text[14]->getGlobalBounds().height * 1.1f);
			//Update powerup 1 held value label
			text[16]->setString(std::to_string(Player::getData()->num_powerups[0]));
			text[16]->setPosition(sprites[11]->getPosition());
			text[16]->move(-text[16]->getGlobalBounds().width - Config::applyRDX(7.5f), (sprites[11]->getGlobalBounds().height / 2) - (text[16]->getGlobalBounds().height));
			clock.restart();
			ui[3]->lock();
		}
		else {
			ui[3]->lock();
		}
	}
	else if (*ui[3]->getState() == UIState::LOCK) {
		if (Player::getData()->coins >= GM_COST_PWP_2XPTS) {
			if (clock.getElapsedTime().asMilliseconds() > 200) {
				//only unlock if affordable
				ui[3]->unlock();
			}
		}
	}

	//Power up buying button (time skip)
	if (*ui[4]->getState() == UIState::CLICK) {
		if (Player::getData()->coins >= GM_COST_PWP_TMSKP) {
			//Save new coin count
			Player::getData()->coins -= GM_COST_PWP_TMSKP;
			if (Player::getData()->num_powerups[1] < 1) {
				Player::getData()->num_powerups[1] = 0;
			}
			Player::getData()->num_powerups[1]++;
			Player::save();
			Player::load();
			//Update coin value label
			text[15]->setString(std::to_string(Player::getData()->coins));
			text[15]->setPosition(text[14]->getPosition());
			text[15]->move((text[14]->getGlobalBounds().width / 2) - (text[15]->getGlobalBounds().width / 2), text[14]->getGlobalBounds().height * 1.1f);
			//Update powerup held label
			text[17]->setString(std::to_string(Player::getData()->num_powerups[1]));
			text[17]->setPosition(sprites[11]->getPosition());
			text[17]->move(-text[17]->getGlobalBounds().width - Config::applyRDX(7.5f), (sprites[11]->getGlobalBounds().height / 2) - (text[17]->getGlobalBounds().height));
			clock.restart();
			ui[4]->lock();
		}
		else {
			ui[4]->lock();
		}
	}
	else if (*ui[4]->getState() == UIState::LOCK) {
		if (Player::getData()->coins >= GM_COST_PWP_TMSKP) {
			if (clock.getElapsedTime().asMilliseconds() > 200) {
				//only unlock if affordable
				ui[4]->unlock();
			}
		}
	}
}

void ChallengePlayScene::input(sf::Event* e) {
	p1Cannon.input(e);
}

BallColour ChallengePlayScene::getNextColour() {
	srand(time(NULL));
	BallColour next = BallColour(nextBalls[0]->col);
	for (int i = 0; i < 3; i++) {
		if (i == 2) {
			nextBalls[i]->col = BallColour(static_cast<BallColour>((rand() % (rules->maxColours))));
		}
		else {
			nextBalls[i]->col = nextBalls[i + 1]->col;
		}
		sprites[nextBalls[i]->spr_id]->setTexture(*AssetManager::getTexture("ball_" + Ball::ballColourToStr(nextBalls[i]->col)));
	}
	return next;
}

void ChallengePlayScene::updatePauseLabel() {
	if (paused == true) {
		text[5]->setString("Paused: Yes");
	}
	else {
		text[5]->setString("Paused: No");
	}
	text[5]->setPosition(ui[1]->getSprite()->getPosition());
	text[5]->move((ui[1]->getSprite()->getGlobalBounds().width / 2) - (text[5]->getGlobalBounds().width / 2), -text[5]->getGlobalBounds().height - 2.5f);
}

//Simple method to update value held within score label and re-position 
void ChallengePlayScene::updateScoreLabel() {
	text[3]->setString(std::to_string(playerScore));
	text[3]->setPosition(text[2]->getPosition());
	text[3]->move((+text[2]->getGlobalBounds().width / 2) - (text[3]->getGlobalBounds().width / 2), text[2]->getGlobalBounds().height * 1.1f);
}

void ChallengePlayScene::updateClockLabel() {
	if (game_clock.getElapsedTime().asMilliseconds() > 1000) {
		time_elapsed++;
		text[8]->setString(std::to_string(time_elapsed) + "s");
		text[8]->setPosition(sprites[7]->getPosition());
		text[8]->move(sprites[7]->getGlobalBounds().width + 2.5f + (text[8]->getGlobalBounds().width / 2), (sprites[7]->getGlobalBounds().height / 2) - (text[8]->getGlobalBounds().height / 2));
		game_clock.restart();
	}
}

void ChallengePlayScene::updateBallsLeft() {
	text[11]->setString("Balls left: " + std::to_string(num_balls));
	text[11]->setPosition(text[10]->getPosition());
	text[11]->move((text[10]->getGlobalBounds().width / 2) - (text[11]->getGlobalBounds().width / 2), text[10]->getGlobalBounds().height);
}

//Add row
void ChallengePlayScene::addRowReassignSprites() {
	map.addRow(true);
	float lsize = balls.size();
	//Loop through each ball in balls vector
	for (int i = 0; i < balls.size(); i++) {
		//Delete sprite
		std::remove(sprites.begin(), sprites.end(), balls[i]->getSprite());
	}

	//Loop through each ball in new row and register sprite
	for (int i = 0; i < map.getRow(0)->row.size(); i++) {
		sprites.push_back(map.getBall(0, i)->spr);
		sprites_map_end++;
	}

	//Register player balls again
	for (int i = 0; i < balls.size() - 1; i++) {
		balls[i]->setID(sprites.size());
		sprites.push_back(balls[i]->getSprite());
	}
}

//Clean scene up
void ChallengePlayScene::cleanup() {
	map.getMap().clear();
	balls.clear();
	nextBalls.clear();
}