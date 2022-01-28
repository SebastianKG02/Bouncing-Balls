#include "PlayCampaignScene.h"
#include "PlayerData.h"

PlayCampScene::PlayCampScene(int id, std::string name) {
	this->name = name;
	this->sceneID = id;
}

void PlayCampScene::init() {
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
	//Set up easy centering variable
	centerX = Config::user_resolution.X() / 2;
	num_balls = 0;
	maxCols = 2;
	//Initalise ball map
	//Ensure proper scaling of difficulty
	this->rules = SceneManager::getCampSettings(Player::getData()->last_level);
	std::cout << "[BallMap]Creating map for Campaign Level " << rules->level << " (loaded as " << Player::getData()->last_level << ")" << std::endl;
	map = BallMap(rules->rows, rules->cols, rules->minColours, rules->maxColours, Config::user_resolution.X()*0.3f, 0, -1);
	
	//Initalise sound effects
	//Ball hit (same colour) sound effect
	sounds.push_back(new sf::Sound(*AssetManager::getSound("pop")));
	sounds[0]->setVolume(10 * Config::user_sfx_volume);

	//Background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));

	//Add back button & label
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[0]->getSprite()->setPosition((0+(ui[0]->getSprite()->getGlobalBounds().width/4)), Config::user_resolution.Y() - 15.f - ui[0]->getSprite()->getGlobalBounds().height);
	text.push_back(new sf::Text(std::string("Exit"), *AssetManager::getFont("title"), 34));
	text[0]->setPosition(ui[0]->getSprite()->getPosition());
	text[0]->move(ui[0]->getSprite()->getGlobalBounds().width / 2, ui[0]->getSprite()->getGlobalBounds().height / 2);
	text[0]->move(-text[0]->getGlobalBounds().width / 2, (-text[0]->getGlobalBounds().height / 2)-2.5f);
	text[0]->setFillColor(sf::Color::Black);

	//Level identifier label
	text.push_back(new sf::Text(std::string("Level " + std::to_string(rules->level + 1)), *AssetManager::getFont("title"), 75));
	text[1]->setPosition(ui[0]->getSprite()->getPosition().x, text[1]->getGlobalBounds().height);
	text[1]->move((ui[0]->getSprite()->getGlobalBounds().width / 2)-(text[1]->getGlobalBounds().width/2), 0);
	text[1]->setFillColor(sf::Color::Black);
	text[1]->setOutlineThickness(1.5f);
	text[1]->setOutlineColor(sf::Color::White);

	//Score label
	text.push_back(new sf::Text(std::string("Score:"), *AssetManager::getFont("title"), 50));
	text[2]->setPosition(text[1]->getPosition());
	text[2]->move((+text[1]->getGlobalBounds().width / 2) - (text[2]->getGlobalBounds().width / 2), text[1]->getGlobalBounds().height *1.5f);
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
	ui[2]->getSprite()->move((text[5]->getGlobalBounds().width/2)-(ui[2]->getSprite()->getGlobalBounds().width/2), -ui[1]->getSprite()->getGlobalBounds().height + 2.5f);
	//Pause button label
	text.push_back(new sf::Text(std::string("Pause"), *AssetManager::getFont("title"), 34));
	text[6]->setPosition(ui[2]->getSprite()->getPosition());
	text[6]->move((ui[2]->getSprite()->getGlobalBounds().width / 2) - (text[6]->getGlobalBounds().width / 2), (+text[6]->getGlobalBounds().height / 2) - 2.5f);
	text[6]->setFillColor(sf::Color::Black);

	//Starting countdown label
	text.push_back(new sf::Text(std::string(""), *AssetManager::getFont("title"), 34));
	text[7]->setPosition(ui[2]->getSprite()->getPosition());
	text[7]->move((ui[2]->getSprite()->getGlobalBounds().width / 2) - text[7]->getGlobalBounds().width / 2, -ui[2]->getSprite()->getGlobalBounds().height-text[7]->getGlobalBounds().height - 2.5f);
	text[7]->setFillColor(sf::Color::Black);

	//Register cannon sprites
	p1Cannon = Cannon(centerX,Config::user_resolution.Y()-Config::applyRDY(32), true);
	p1Cannon.getBase()->move(0, -(p1Cannon.getBase()->getGlobalBounds().height*4));
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

	nextBalls[0]->spr->setPosition(centerX, Config::user_resolution.Y()-2*nextBalls[0]->spr->getGlobalBounds().height);
	nextBalls[0]->spr->move((p1Cannon.getBase()->getGlobalBounds().width/2) + (nextBalls[0]->spr->getGlobalBounds().width*0.75f), +(nextBalls[0]->spr->getGlobalBounds().height / 2));
	nextBalls[1]->spr->setPosition(nextBalls[0]->spr->getPosition());
	nextBalls[1]->spr->move(nextBalls[0]->spr->getGlobalBounds().width + 2.5f, 0);
	nextBalls[2]->spr->setPosition(nextBalls[1]->spr->getPosition());
	nextBalls[2]->spr->move(nextBalls[1]->spr->getGlobalBounds().width + 2.5f, 0);

	//Register tracing line for shot accuracy tracking
	tracer = sf::RectangleShape(sf::Vector2f(2, 1000));
	shapes.push_back(&tracer);

	left_bound = sf::RectangleShape(sf::Vector2f(8, Config::user_resolution.Y()*16));
	left_bound.setPosition(-5 + (Config::user_resolution.X() * 0.3f) - 8, 0);
	left_bound.setFillColor(sf::Color::Transparent);
	left_bound.setOutlineThickness(3.f);
	left_bound.setOutlineColor(sf::Color::Red);
	left_bound.setFillColor(sf::Color::Black);
	shapes.push_back(&left_bound);

	right_bound = sf::RectangleShape(sf::Vector2f(8, Config::user_resolution.Y()*16));
	right_bound.setPosition(map.getEnd().x+5, 0);
	right_bound.setFillColor(sf::Color::Transparent);
	right_bound.setOutlineThickness(3.f);
	right_bound.setOutlineColor(sf::Color::Red);
	right_bound.setFillColor(sf::Color::Black);
	shapes.push_back(&right_bound);

	death_bound = sf::RectangleShape(sf::Vector2f(right_bound.getPosition().x - left_bound.getPosition().x+8, 8));
	death_bound.setPosition(left_bound.getPosition().x, Config::user_resolution.Y()- p1Cannon.getBase()->getPosition().y - Config::applyRDY(275));
	death_bound.setFillColor(sf::Color::Transparent);
	death_bound.setOutlineThickness(3.f);
	death_bound.setOutlineColor(sf::Color::Red);
	death_bound.setFillColor(sf::Color::Black);
	shapes.push_back(&death_bound);

	ball_eraser = sf::RectangleShape(sf::Vector2f(right_bound.getPosition().x - left_bound.getPosition().x + 8, 8));
	ball_eraser.setPosition(map.getStart().x, map.getEnd().y);
	ball_eraser.setFillColor(sf::Color::Transparent);
	ball_eraser.setOutlineThickness(3.f);
	ball_eraser.setOutlineColor(sf::Color::Red);
	shapes.push_back(&ball_eraser);
	
	//Clock sprite
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("clock")));
	sprites[7]->setScale(0.5f, 0.5f);
	sprites[7]->setPosition(Config::user_resolution.X()*0.75+(sprites[7]->getGlobalBounds().width/2), 2.5f + (sprites[7]->getGlobalBounds().height)/2);
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
	text[9]->setPosition(right_bound.getPosition().x + right_bound.getGlobalBounds().width + 48.f, sprites[7]->getPosition().y+(1.f*sprites[7]->getGlobalBounds().height));
	text[9]->setFillColor(sf::Color::Black);
	text[9]->setOutlineThickness(1.5f);
	text[9]->setOutlineColor(sf::Color::White);

	//OBJECTIVE 1 -----------------------------------------------------------------------------------------------------------------------------------------------------------------OBJECTIVE 1
	//Objective 1 - clear all balls (label)
	text.push_back(new sf::Text(std::string("Clear all balls"), *AssetManager::getFont("title"), 32));
	text[10]->setPosition(text[9]->getPosition());
	text[10]->move((text[9]->getGlobalBounds().width / 2) - (text[10]->getGlobalBounds().width / 2), 2.5f + text[9]->getGlobalBounds().height);
	text[10]->setFillColor(sf::Color::Black);

	//Objective 1 - clear all balls (value)
	text.push_back(new sf::Text(std::string("Balls left: " + std::to_string(num_balls)), *AssetManager::getFont("title"), 30));
	text[11]->setPosition(text[10]->getPosition());
	text[11]->move((text[10]->getGlobalBounds().width / 2) - (text[11]->getGlobalBounds().width / 2), text[10]->getGlobalBounds().height);
	text[11]->setFillColor(sf::Color::Black);

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

	//Objective 3 - star identifier
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("star_lock")));
	sprites[10]->setPosition(text[13]->getPosition());
	sprites[10]->setScale(0.2f, 0.2f);
	sprites[10]->move((text[13]->getGlobalBounds().width / 2) - (sprites[10]->getGlobalBounds().width / 2), text[13]->getGlobalBounds().height + 2.5f);
	
	//Coins held label
	text.push_back(new sf::Text(std::string("Coins:"), *AssetManager::getFont("title"), 50));
	text[14]->setPosition(text[3]->getPosition());
	text[14]->move((text[3]->getGlobalBounds().width / 2) - (text[14]->getGlobalBounds().width / 2), text[3]->getGlobalBounds().height * 1.5f);
	text[14]->setFillColor(sf::Color::Black);
	//Coins held value label
	text.push_back(new sf::Text(std::to_string(Player::getData()->coins), *AssetManager::getFont("title"), 50));
	text[15]->setPosition(text[14]->getPosition());
	text[15]->move((text[14]->getGlobalBounds().width / 2) - (text[15]->getGlobalBounds().width / 2), text[14]->getGlobalBounds().height * 1.1f);
	text[15]->setOutlineThickness(1.5f);
	text[15]->setOutlineColor(sf::Color::Black);

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

void PlayCampScene::update(sf::RenderWindow* w) {
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
			if (*ui[1]->getState() == UIState::CLICK) {
				game_clock.restart();
				ui[1]->lock();
			}
			//If start button previously pressed
			else if (*ui[1]->getState() == UIState::LOCK) {
				//If countdown over
				if (game_clock.getElapsedTime().asMilliseconds() > 3000) {
					started = true;
					text[7]->setString("Go!");
					text[7]->setPosition(ui[2]->getSprite()->getPosition());
					text[7]->move((ui[2]->getSprite()->getGlobalBounds().width / 2) - text[7]->getGlobalBounds().width / 2, -ui[2]->getSprite()->getGlobalBounds().height - text[7]->getGlobalBounds().height - 2.5f);;
					game_clock.restart();
				}
				//If counting
				else {
					text[7]->setString("Starting in... " + std::to_string(std::round(game_clock.getElapsedTime().asSeconds())));
					text[7]->setPosition(ui[2]->getSprite()->getPosition());
					text[7]->move((ui[2]->getSprite()->getGlobalBounds().width / 2) - text[7]->getGlobalBounds().width / 2, -ui[2]->getSprite()->getGlobalBounds().height - text[7]->getGlobalBounds().height - 2.5f);
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
				if ((sf::Keyboard::isKeyPressed(Config::user_key_shoot) && fired == false) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && fired == false) {
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
					std::cout << "TX: " << balls[balls.size() - 1]->getTarget()->x << " TY: " << balls[balls.size() - 1]->getTarget()->y << std::endl;
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
														std::cout << "[b" << it << "]" << "[" << x << "," << y << "] Is alive!" << std::endl;
														//If ball is valid
														if (map.getBall(x, y)->spr->getColor() != sf::Color::Transparent && map.getBall(x, y)->spr->getColor() == sf::Color::White) {
															std::cout << "[b" << it << "]" << "[" << x << "," << y << "] Is valid!" << std::endl;
															//If ball hit is of same colour
															if (map.getBall(x, y)->col == *ball->getColour()) {
																std::cout << "[b" << it << "]" << "[" << x << "," << y << "] Is same colour: Map Ball-" << Ball::ballColourToStr(map.getBall(x, y)->col) << " # Player Ball-" << Ball::ballColourToStr(*ball->getColour()) << std::endl;
																std::cout << "[b" << it << "]" << "Starting kill list... " << std::endl;

																//initalise kill list
																std::vector<sf::Vector2i*> kill_list = std::vector<sf::Vector2i*>();

																//Add current ball to kill list
																kill_list.push_back(new sf::Vector2i(x, y));
																bool finished = false;

																//Determine larger hitbox radius
																sf::Vector2i min_pos = sf::Vector2i(ball_x - (bwidth * 1.5f), ball_y - (bheight * 2.5f));
																sf::Vector2i max_pos = sf::Vector2i(ball_x + (bwidth * 1.5f), ball_y + (bheight * 1.5f));

																//Debug info
																std::cout << "[BallMap]" << "[b" << it << "] Min: [" << min_pos.x << "," << min_pos.y << "]" << std::endl;
																std::cout << "[BallMap]" << "[b" << it << "] Max: [" << max_pos.x << "," << max_pos.y << "]" << std::endl;

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
																		//Check if this ball 
																		std::cout << "[" << kx << "," << ky << "] x: " << map_ball->spr->getPosition().x << " : y: " << map_ball->spr->getPosition().y << std::endl;
																		if (map_ball->spr->getPosition().x > min_pos.x - 2.5f && map_ball->spr->getPosition().x < max_pos.x + 2.5f) {
																			std::cout << "[" << kx << "," << ky << "]XPOS check passed" << std::endl;
																			if (map_ball->spr->getPosition().y > min_pos.y - 2.5f && map_ball->spr->getPosition().y < max_pos.y + 2.5f) {
																				std::cout << "[" << kx << "," << ky << "]YPOS check passed" << std::endl;
																				if (map_ball->alive == true) {
																					std::cout << "[" << kx << "," << ky << "]Ball alive check passed" << std::endl;
																					std::cout << "[" << kx << "," << ky << "]Map ball colour: " << Ball::ballColourToStr(map_ball->col) << std::endl;
																					std::cout << "[" << kx << "," << ky << "]Player ball colour: " << Ball::ballColourToStr(*ball->getColour()) << std::endl;
																					if (map_ball->col == *ball->getColour()) {
																						std::cout << "[" << kx << "," << ky << "]Ball colour check passed! Adding to kill list " << std::endl;
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
																		playerScore += GM_PTS_PER_BALL;
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
																std::cout << "[b" << it << "]" << "[" << x << "," << y << "] Is not same colour: Map Ball-" << Ball::ballColourToStr(map.getBall(x, y)->col) << " # Player Ball-" << Ball::ballColourToStr(*ball->getColour()) << std::endl;
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
																				std::cout << "[b" << it << "]" << "[" << x << "," << y << "] Now is " << kx << "," << ky << std::endl;
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
				map.update(0, GM_BALL_MOVE_SPEED * TICK_SPEED.asMilliseconds());

				num_balls = 0;
				//Ball killer loop 
				for (int x = 0; x < map.getMap().size(); x++) {
					for (int y = 0; y < map.getRow(x)->row.size(); y++) {
						bool isAlive = map.getBall(x, y)->alive;
						bool ignore = false;
						sf::Color org_col = map.getBall(x, y)->spr->getColor();

						//If map ball has hit death barrier
						if (map.getBall(x,y)->alive == true && map.getBall(x, y)->spr->getGlobalBounds().intersects(death_bound.getGlobalBounds())) {
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
								if (checks_req > 0 && (checks_req == checks_comp)) {
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
													std::cout << "[" << x << "," << y << "] Lost due to above (l/r) check" << std::endl;
													playerScore += GM_PTS_PER_BALL;
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
													std::cout << "[" << x << "," << y << "] Lost due to above first ball in row check" << std::endl;
													playerScore += GM_PTS_PER_BALL;
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
														std::cout << "[" << x << "," << y << "] Lost due to above (l/r) check" << std::endl;
														playerScore += GM_PTS_PER_BALL;
														sounds[0]->play();
													}
												}
											}

											//If last ball
											if (y == map.getRow(x)->row.size() - 1) {
												if (map.getBall(x + 1, y - 1)->alive == false && map.getBall(x + 1, y - 1)->spr->getColor() == sf::Color::Transparent) {
													map.getBall(x, y)->alive = false;
													map.getBall(x, y)->spr->setColor(sf::Color::Transparent);
													std::cout << "[" << x << "," << y << "] Lost due to above last ball in row check" << std::endl;
													playerScore += GM_PTS_PER_BALL;
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
								//If not first row
								bool checks[4] = { false, false, false, false };
								if (x > 0) {
									if (y < map.getRow(x - 1)->row.size()) {
										if (map.getBall(x - 1, y)->alive != true || map.getBall(x - 1, y)->spr->getColor() == sf::Color::Transparent) {
											checks[0] = true;
										}
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
								}

								short counter = 0;
								for (bool b : checks) {
									if (b) {
										counter++;
									}
								}

								if (counter >= 4) {
									map.getBall(x, y)->alive = false;
									map.getBall(x, y)->spr->setColor(sf::Color::Transparent);
									playerScore += GM_PTS_PER_BALL;
									sounds[0]->play();
								}
							}

							if (map.getBall(x, y)->alive != isAlive && map.getBall(x, y)->spr->getColor() != org_col) {
								std::cout << "[GM][" << x << "," << y << "]Lost to gravity!" << std::endl;
							}
						}
					}
					if(status[0] == false && status[1] == true){
						break;
					}
				}
			}
		}
	}

	updateBallsLeft();

	if (status[1] != true) {
		//If objectove 1 has been met
		if (num_balls <= 0) {
			rules->obj_l1 = true;
			sprites[8]->setTexture(*AssetManager::getTexture("star_full"));
			status[0] = true;
		}
		else {
			//Security check
			if (num_balls == 1) {
				std::cout << "One ball left!! [" << last_alive.x << "," << last_alive.y << "]Checking..." << std::endl;
				if (map.getBall(last_alive.x, last_alive.y)->alive != true || map.getBall(last_alive.x, last_alive.y)->spr->getColor() == sf::Color::Transparent) {
					num_balls--;
				}
				else {
					map.getBall(last_alive.x, last_alive.y)->alive = false;
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
		std::cout << "[GM]Win!" << std::endl;
		//Reward player with points for level completion
		playerScore += (rules->level * GM_PTS_PW_LEVEL_MOD) + GM_PTS_PER_WIN;
		//Set completion flag for this level to be true
		Player::getData()->campaign_comp[rules->level] = true;
		//Reward player with stars as completed
		if (rules->obj_l1 == true) {
			Player::getData()->campaign_stars[rules->level] += 1;
		}
		if (rules->obj_l2 == true) {
			Player::getData()->campaign_stars[rules->level] += 1;
		}
		if (rules->obj_l3 == true) {
			Player::getData()->campaign_stars[rules->level] += 1;
		}
		if (Player::getData()->campaign_stars[rules->level] > 3) {
			Player::getData()->campaign_stars[rules->level] = 3;
		}
		//Reward player with coins
		Player::getData()->coins += std::round(playerScore / 1000);
		rules->final_score = playerScore;
		Player::save();
		std::cout << "[GM]Game finished & player data saved, switching to win scene..." << std::endl;
		Player::getData()->last_level = rules->level;
		SceneManager::setNext(10);
		SceneManager::next();
	}

	//If not won & has lost
	if (status[0] == false && status[1] == true) {
		std::cout << "[GM]Loss." << std::endl;
		Player::save();
		Player::getData()->last_level = rules->level;
		SceneManager::setNext(11);
		SceneManager::next();
		std::cout << "[GM]Game finished & player data saved, switching to lose scene..." << std::endl;
	}

	//If back button pressed
	if (*ui[0]->getState() == UIState::CLICK) {
		clock.restart();
		ui[0]->lock();
		this->lock();
		SceneManager::setNext(8);
		SceneManager::next();
	}
	else if (*ui[0]->getState() == UIState::LOCK) {
		if (clock.getElapsedTime().asMilliseconds() > 200) {
			ui[0]->unlock();
		}
	}

	//Pause button
	if (*ui[2]->getState() == UIState::CLICK) {
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
}

void PlayCampScene::input(sf::Event* e) {
	p1Cannon.input(e);
}

BallColour PlayCampScene::getNextColour(){
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

void PlayCampScene::updatePauseLabel() {
	if (paused == true) {
		text[5]->setString("Paused: Yes");
	}
	else {
		text[5]->setString("Paused: No");
	}
	text[5]->setPosition(ui[1]->getSprite()->getPosition());
	text[5]->move((ui[1]->getSprite()->getGlobalBounds().width / 2) - (text[5]->getGlobalBounds().width / 2), -text[5]->getGlobalBounds().height - 2.5f);
}

void PlayCampScene::updateScoreLabel() {
	text[3]->setString(std::to_string(playerScore));
	text[3]->setPosition(text[2]->getPosition());
	text[3]->move((+text[2]->getGlobalBounds().width / 2) - (text[3]->getGlobalBounds().width / 2), text[2]->getGlobalBounds().height *1.1f);
}

void PlayCampScene::updateClockLabel() {
	if (game_clock.getElapsedTime().asMilliseconds() > 1000) {
		time_elapsed++;
		text[8]->setString(std::to_string(time_elapsed) + "s");
		text[8]->setPosition(sprites[7]->getPosition());
		text[8]->move(sprites[7]->getGlobalBounds().width + 2.5f + (text[8]->getGlobalBounds().width / 2), (sprites[7]->getGlobalBounds().height / 2) - (text[8]->getGlobalBounds().height / 2));
		game_clock.restart();
	}
}

void PlayCampScene::updateBallsLeft() {
	text[11]->setString("Balls left: " + std::to_string(num_balls));
	text[11]->setPosition(text[10]->getPosition());
	text[11]->move((text[10]->getGlobalBounds().width / 2) - (text[11]->getGlobalBounds().width / 2), text[10]->getGlobalBounds().height);
}

void PlayCampScene::addRowReassignSprites() {
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

void PlayCampScene::cleanup() {
	map.getMap().clear();
	balls.clear();
	nextBalls.clear();
}