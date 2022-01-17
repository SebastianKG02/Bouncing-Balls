#include "PlayL1Scene.h"

PlayL1Scene::PlayL1Scene(int id, std::string name) {
	this->name = name;
	this->sceneID = id;
}

void PlayL1Scene::init() {
	//Lock scene during initalisaton (unavailable for input / drawing)
	lock();
	clock = sf::Clock();
	sprites = std::vector<sf::Sprite*>();
	sounds = std::vector<sf::Sound*>();
	text = std::vector<sf::Text*>();
	ui = std::vector<UIElement*>();
	balls = std::vector<Ball*>();
	resize = false;
	fired = true;
	sprites_map_begin = 0;
	sprites_map_end = 0;
	sprites_opp_begin = 0;
	sprites_opp_end = 0;
	sprites_pball_begin = 0;
	sprites_pball_end = 0;
	//Set up easy centering variable
	centerX = Config::user_resolution.X() / 2;

	num_balls = 0;
	maxCols = 2;
	map = BallMap(24, 12, 2, 2, Config::user_resolution.X()*0.25f, 0.f, -1);
	//opp = BallMap(24, 16, 2, 2, Config::user_resolution.X() * 0.25f, map.getStart().y, 1);

	//Background image
	sprites.push_back(new sf::Sprite(*AssetManager::getTexture("background")));

	//Add back button & label
	ui.push_back(new UIButton(new float[2]{ centerX, 0.f }, std::string("long"), new float[2]{ 0.1f, 0.1f }));
	ui[0]->getSprite()->setPosition(centerX - (ui[0]->getSprite()->getGlobalBounds().width / 2), Config::applyRDY(20)+(ui[0]->getSprite()->getGlobalBounds().height/2));
	text.push_back(new sf::Text(std::string("Back"), *AssetManager::getFont("title"), 34));
	text[0]->setPosition(centerX - (text[0]->getGlobalBounds().width / 2), Config::applyRDY(20) + (text[0]->getGlobalBounds().height / 2));
	text[0]->setFillColor(sf::Color::Black);

	p1Cannon = Cannon(centerX,Config::user_resolution.Y()-Config::applyRDY(32), true);
	p1Cannon.getBase()->move(0, -(p1Cannon.getBase()->getGlobalBounds().height*4));
	sprites.push_back(p1Cannon.getBase());
	sprites.push_back(p1Cannon.getBody());
	sprites.push_back(p1Cannon.getAxel());

	tracer = sf::RectangleShape(sf::Vector2f(2, 1000));
	shapes.push_back(&tracer);

	left_bound = sf::RectangleShape(sf::Vector2f(8, Config::user_resolution.Y()*16));
	left_bound.setPosition(-5 + (Config::user_resolution.X() * 0.25f) - 8, 0);
	left_bound.setFillColor(sf::Color::Transparent);
	left_bound.setOutlineThickness(3.f);
	left_bound.setOutlineColor(sf::Color::Red);
	shapes.push_back(&left_bound);

	right_bound = sf::RectangleShape(sf::Vector2f(8, Config::user_resolution.Y()*16));
	right_bound.setPosition(map.getEnd().x+5, 0);
	right_bound.setFillColor(sf::Color::Transparent);
	right_bound.setOutlineThickness(3.f);
	right_bound.setOutlineColor(sf::Color::Red);
	shapes.push_back(&right_bound);

	death_bound = sf::RectangleShape(sf::Vector2f(right_bound.getPosition().x - left_bound.getPosition().x+8, 8));
	death_bound.setPosition(left_bound.getPosition().x, Config::user_resolution.Y()- p1Cannon.getBase()->getPosition().y - Config::applyRDY(275));
	death_bound.setFillColor(sf::Color::Transparent);
	death_bound.setOutlineThickness(3.f);
	death_bound.setOutlineColor(sf::Color::Red);
	shapes.push_back(&death_bound);

	ball_eraser = sf::RectangleShape(sf::Vector2f(right_bound.getPosition().x - left_bound.getPosition().x + 8, 8));
	ball_eraser.setPosition(map.getStart().x, map.getEnd().y);
	ball_eraser.setFillColor(sf::Color::Transparent);
	ball_eraser.setOutlineThickness(3.f);
	ball_eraser.setOutlineColor(sf::Color::Red);
	shapes.push_back(&death_bound);
	
	sprites_map_begin = sprites.size();

	//Register map balls as rendered sprites
	for (auto& row : map.getMap()) {
		for (auto& ball : row->row) {
			sprites.push_back(ball->spr);
			ball->spr_id = sprites.size() - 1;
		}
	}

	sprites_map_end = sprites.size();
	sprites_opp_begin = sprites.size();

	for (auto& row : opp.getMap()) {
		for (auto& ball : row->row) {
			ball->spr->setColor(sf::Color::Transparent);
			sprites.push_back(ball->spr);
		}
	}

	sprites_opp_end = sprites.size();
	sprites_pball_begin = sprites.size();
	sprites_pball_end = sprites.size();
	//Unlock scene and get ready to function as complete scene
	clock.restart();
	unlock();
}

void PlayL1Scene::update(sf::RenderWindow* w) {
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
	tracer.setRotation(p1Cannon.getBody()->getRotation()+180);

	//Shooting mechanism
	if ((sf::Keyboard::isKeyPressed(Config::user_key_shoot) && fired == false) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && fired == false) {
		clock.restart();
		BallColour bc;
		bc = static_cast<BallColour>((rand() % (2)));
		balls.push_back(new Ball(p1Cannon.getBody()->getPosition().x, p1Cannon.getBody()->getPosition().y, bc));

		if (p1Cannon.getBody()->getRotation() == 0) {
			balls[num_balls]->path_plot(Config::user_resolution.Y(), p1Cannon.getBody()->getRotation() + 0.00001f, sf::Vector2f(p1Cannon.getBody()->getPosition().x, p1Cannon.getBody()->getPosition().y-p1Cannon.getBody()->getGlobalBounds().height));
		}
		else {
			balls[num_balls]->path_plot(Config::user_resolution.Y(), p1Cannon.getBody()->getRotation(), p1Cannon.getBody()->getPosition());
		}
		std::cout << "TX: " << balls[balls.size()-1]->getTarget()->x << " TY: " << balls[balls.size()-1]->getTarget()->y << std::endl;
		*balls[balls.size() - 1]->alive() = true;
		sprites.push_back(balls[balls.size()-1]->getSprite());
		sprites_pball_end++;
		fired = true;
		num_balls++;
	}
	else {
		if (clock.getElapsedTime().asMilliseconds() >= 1000 * GM_BALL_SHOOT_DELAY) {
			fired = false;
		}
	}

	//Update player balls
	if (num_balls > 0) {
		int it = 0;
		for (auto& ball : balls) {
			it++;

			if (ball == nullptr || it == 0) {
				continue;
			}

			if (*ball->alive() == true) {
				ball->update();

				//If ball collides with nonblank columns
				if (ball->getSprite()->getPosition().y <= map.getMap()[map.getBlanks()]->row[0]->spr->getPosition().y + ball->getSprite()->getGlobalBounds().height) {
					std::cout << "valid" << std::endl;
					float mapWidth = map.getEnd().x - map.getStart().x;
					float temp = 0;

					bool ypos_check = false;
					bool xpos_check = false;
					bool valid_check = false;
					bool col_check = false;

					int found = 1;

					std::vector<sf::Vector2f*> kill_list = std::vector<sf::Vector2f*>();

					for (int i = 0; i < map.getRows() - 1; i++) {
						ypos_check = false;
						xpos_check = false;
						valid_check = false;
						col_check = false;
						float ball_y = ball->getSprite()->getPosition().y;
						float ball_x = ball->getSprite()->getPosition().x;
						float bwidth = ball->getSprite()->getGlobalBounds().width;
						float bheight = ball->getSprite()->getGlobalBounds().height;
						float approx_col = round(((ball_x - map.getStart().x) / bwidth));
						float approx_row = round(((ball_y - map.getStart().y) / bheight));
						if (approx_col > map.getMap()[i]->row.size() - 1) {
							approx_col -= 2;
						}
						if (i > -1) {
							if (ball_y >= map.getMap()[i]->row[approx_col]->spr->getPosition().y && map.getMap()[i]->row[approx_col]->alive == true && map.getMap()[i]->row[approx_col]->spr->getColor() != sf::Color::Transparent) {
								std::cout << "YPOS Check PASS" << std::endl;
								ypos_check = true;
								int rowsize = map.getMap()[i]->row.size();
								for (int j = 0; j < rowsize; j++) {
									if (approx_row > -1) {
										if (map.getBall(i, j)->alive == true && map.getBall(i, j)->spr->getColor() != sf::Color::Transparent) {
											std::cout << "is alive & isn't transparent" << std::endl;
											if (map.getBall(i, j)->col == *ball->getColour()) {
												found++;
												kill_list.push_back(new sf::Vector2f(i, j));
												std::cout << "hit!" << std::endl;
											}
										}
										std::cout << "CHK " << i << " : " << j << std::endl;
										std::cout << "Ball Pos " << ball->getSprite()->getPosition().x << " , " << ball_y << std::endl;
										std::cout << "BallMap[" << i << "," << j << "] Pos: " << map.getBall(i, j)->spr->getPosition().x << " , " << map.getBall(i, j)->spr->getPosition().y << std::endl;
									}
								}
							}
						}
					}
									/*
									if ((ball_x >= map.getMap()[i]->row[j]->spr->getPosition().x || ball_x + bwidth >= map.getMap()[i]->row[j]->spr->getPosition().x) && ball_x < map.getMap()[i]->row[j]->spr->getPosition().x + bwidth) {
										std::cout << "Sucess!" << std::endl;
										xpos_check = true;
										col_check = false;
										valid_check = false;
										if (map.getBall(i, j)->alive == true && map.getBall(i, j)->col == *ball->getColour() && map.getBall(i, j)->spr->getColor() != sf::Color::Transparent) {
											found++;
											valid_check = true;
											kill_list.push_back(new sf::Vector2f(i, j));
											if (col_check != true) {
												col_check = true;
											}
										}
										if (j > 0){
											if (map.getBall(i, j - 1)->alive == true
												&& map.getBall(i, j - 1)->col == *ball->getColour()
												&& map.getBall(i, j - 1)->spr->getColor() != sf::Color::Transparent) {
												found++;
												valid_check = true;
												kill_list.push_back(new sf::Vector2f(i, j - 1));
												if (col_check != true) {
													col_check = true;
												}
											}
										}
										if (i > 0) {
											if (map.getBall(i - 1, j)->alive == true
												&& map.getBall(i - 1, j)->col == *ball->getColour()
												&& map.getBall(i - 1, j)->spr->getColor() != sf::Color::Transparent) {
												found++;
												valid_check = true;
												kill_list.push_back(new sf::Vector2f(i - 1, j));
												if (col_check != true) {
													col_check = true;
												}
											}
										}
										if (j < map.getMap()[i]->row.size() - 1) {
											if (map.getBall(i, j + 1)->alive == true
												&& map.getBall(i, j + 1)->col == *ball->getColour()
												&& map.getBall(i, j + 1)->spr->getColor() != sf::Color::Transparent) {
												found++;
												valid_check = true;
												kill_list.push_back(new sf::Vector2f(i, j + 1));
												if (col_check != true) {
													col_check = true;
												}
											}
										}
										if (i < map.getRows() - 1) {
											if (map.getBall(i+1, j)->alive == true
												&& map.getBall(i+1, j)->col == *ball->getColour()
												&& map.getBall(i+1, j)->spr->getColor() != sf::Color::Transparent) {
												found++;
												valid_check = true;
												kill_list.push_back(new sf::Vector2f(i, j + 1));
												if (col_check != true) {
													col_check = true;
												}
											}
										}
									}else {
										if (valid_check == true) {
											if (col_check == true) {
												continue;
											}
											else {
												valid_check = false;
											}
										}
										else if (col_check == false && valid_check == false) {
											break;
											}
										}

									}
								}
							}
							else {
								continue;
							}
						}*/
						/*
						else {
							for (int j = 0; j < map.getMap()[i]->row.size()-1; j++) {
								int step = 0;

								if (j % 2 == 0) {
									step = -1;
								}
								else {
									step = -2;
								}

								if (map.getBall(i, j)->col == *ball->getColour() && map.getBall(i, j)->spr->getColor() != sf::Color::Transparent && map.getBall(i, j)->alive == true) {
									if (j < map.getMap()[i]->row.size()-1) {
										if (j > 0) {
											if (map.getBall(i - 1, j)->col == *ball->getColour()) {
												
												for (int k = ((map.getMap()[i]->row.size()+step))-1; k > 0; k--) {
													if (map.getBall(k, j)->spr->getColor() == sf::Color::Transparent) {
														if (map.getBall(k, j)->col == *ball->getColour()) {
															if (k == 1) {
																map.getBall(i, j)->alive == false;
																map.getBall(i, j)->spr->setColor(sf::Color::Transparent);
																kill_list.push_back(new sf::Vector2f(i - 1, j));
																found++;
															}
															continue;
														}
													}
												}
											}
										}
										if (j < map.getMap()[i]->row.size()-1 && map.getBall(i, j)->alive == true) {
											if (map.getBall(i - 1, j + 1 + step)->col == *ball->getColour()) {
												
												for (int k = (map.getMap()[i]->row.size()+step)-1; k > 0; k--) {
													if (map.getBall(k, j)->spr->getColor() == sf::Color::Transparent) {
														if (map.getBall(k, j)->col == *ball->getColour()) {
															if (k == 1) {
																map.getBall(i, j)->alive == false;
																map.getBall(i, j)->spr->setColor(sf::Color::Transparent);
																kill_list.push_back(new sf::Vector2f(i - 1, j + 1 + step));
																found++;
															}
															continue;
														}
													}
													if (map.getBall(i, j)->alive == false) {
														break;
													}
												}
											}
										}
									}

									if (j < map.getMap()[i]->row.size() - 1 && map.getBall(i, j)->alive == true) {
										if (map.getBall(i, j+1 + step)->col == *ball->getColour()) {
											
											for (int k = (map.getMap()[i]->row.size()+step)-1; k > 0; k--) {
												if (map.getBall(k, j)->spr->getColor() == sf::Color::Transparent) {
													if (map.getBall(k, j)->col == *ball->getColour()) {
														if (k == 1) {
															map.getBall(i, j)->alive == false;
															map.getBall(i, j)->spr->setColor(sf::Color::Transparent);
															kill_list.push_back(new sf::Vector2f(i, j + 1 + step));
															found++;
														}
														continue;
													}
												}
											}
										}
									}
										
									if (j > 0 && map.getBall(i, j)->alive == true) {
										if (map.getBall(i, j - 1)->col == *ball->getColour()) {
											
											for (int k = (map.getMap()[i]->row.size()+step)-1; k > 0; k--) {
												if (map.getBall(k, j)->spr->getColor() == sf::Color::Transparent) {
													if (map.getBall(k, j)->col == *ball->getColour()) {
														if (k == 1) {
															map.getBall(i, j)->alive == false;
															map.getBall(i, j)->spr->setColor(sf::Color::Transparent);
															kill_list.push_back(new sf::Vector2f(i, j - 1));
															found++;
														}
														continue;
													}
												}
											}
										}
									}
									
									if (i < map.getMap().size()-map.getBlanks() && map.getBall(i, j)->alive == true) {
										if (map.getBall(i + 1, j)->col == *ball->getColour()) {
											
											for (int k = (map.getMap()[i]->row.size()+step)-1; k > 0; k--) {
												if (map.getBall(k, j)->spr->getColor() == sf::Color::Transparent) {
													if (map.getBall(k, j)->col == *ball->getColour()) {
														if (k == 1) {
															map.getBall(i, j)->alive == false;
															map.getBall(i, j)->spr->setColor(sf::Color::Transparent);
															kill_list.push_back(new sf::Vector2f(i + 1, j));
															found++;
														}
														continue;
													}
												}
											}
										}
										if (map.getBall(i + 1, j + 1 + step)->col == *ball->getColour() ) {
											
											for (int k = (map.getMap()[i]->row.size()+step)-1; k > 0; k--) {
												if (map.getBall(k, j)->spr->getColor() == sf::Color::Transparent) {
													if (map.getBall(k, j)->col == *ball->getColour()) {
														if (k == 1) {
															map.getBall(i, j)->alive == false;
															map.getBall(i, j)->spr->setColor(sf::Color::Transparent);
															kill_list.push_back(new sf::Vector2f(i + 1, j + 1 + step));
															found++;
														}
														continue;
													}
												}
											}
										}
									}
									else {
										if (map.getBall(i, j)->alive == false) {
											map.getBall(i, j)->spr->setColor(sf::Color::Transparent);
										}
									}
								}
							}
						}*/
					
					std::cout << "Hit <" << found << "> balls in total" << std::endl;

					//If more than 3 balls found by algorithim
					if (found > 2) {
						//Loop through each position
						for (auto& kpos : kill_list) {
							//Kill each member
							map.getMap()[kpos->x]->row[kpos->y]->alive = false;
							map.getMap()[kpos->x]->row[kpos->y]->spr->setColor(sf::Color::Transparent);
							//sprites.erase(sprites.begin() + map.getMap()[kpos->x]->row[kpos->y]->spr_id - num_sprites_killed);
							num_sprites_killed++;
							std::cout << "Killing [" << kpos->x << "," << kpos->y << std::endl;
							delete kpos;
						}
						//Kill player ball
						num_balls--;
						ball->kill();
						*ball->alive() = false;
						balls.erase(balls.begin() + (it - 1));
						sprites.erase(sprites.begin() + sprites_pball_begin + (it - 1));
						num_sprites_killed++;
						std::cout << "Ball chain hit! Size: " << found << std::endl;
						break;
					}
					else {
						//If < 2 colours found, kill ball
						//NOTE! have to add ball ADD on unsucessful hit
						num_balls--;
						ball->kill();
						*ball->alive() = false;
						balls.erase(balls.begin() + (it - 1));
						sprites.erase(sprites.begin() + sprites_pball_begin + (it - 1));
						num_sprites_killed++;
						std::cout << "No balls hit. " << std::endl;
						break;
					}

				}

				//Kill ball if hit left bound
				if (ball->getSprite()->getGlobalBounds().intersects(left_bound.getGlobalBounds()) && *ball->alive() == true) {
					num_balls--;
					ball->kill();
					*ball->alive() = false;
					balls.erase(balls.begin() + (it - 1));
					sprites.erase(sprites.begin() + sprites_pball_begin + (it - 1));
					num_sprites_killed++;
					break;
				}

				//Kill ball if hit right bound
				if (ball->getSprite()->getGlobalBounds().intersects(right_bound.getGlobalBounds()) && *ball->alive() == true) {
					num_balls--;
					ball->kill();
					*ball->alive() = false;
					balls.erase(balls.begin() + (it - 1));
					sprites.erase(sprites.begin() + sprites_pball_begin + (it - 1));
					num_sprites_killed++;
					break;
				}

				//Kill ball is hit eraser
				if (ball->getSprite()->getGlobalBounds().intersects(ball_eraser.getGlobalBounds()) && *ball->alive() == true) {
					num_balls--;
					ball->kill();
					*ball->alive() = false;
					balls.erase(balls.begin() + (it - 1));
					sprites.erase(sprites.begin() + sprites_pball_begin + (it - 1));
					num_sprites_killed++;
					break;
				}
			}
			else {

				if (ball != nullptr && *ball->alive() == false) {
					ball->kill();
					balls.erase(balls.begin() + (it-1));
					sprites.erase(sprites.begin() + sprites_pball_begin + (num_balls - 1));
					num_sprites_killed++;
					sprites_pball_end--;
					num_balls--;
					break;
				}
			}
		}
		/*for (int i = 0; i < (balls.size()); i++) {
			if (balls[i] != nullptr) {
				if (balls[i]->alive()) {
					balls[i]->update();

					for (int j = 6; j < map.getMap().size(); j++) {
						for (int k = 6; k < map.getMap()[j]->row.size(); k++) {
							if (i > 0) {
								//std::cout << j << "," << k << std::endl;
								*balls[i]->alive() = true;
								//Dont process transparents & dead
								if (map.getMap()[j]->row[k]->spr->getColor() == sf::Color::Transparent || map.getMap()[j]->row[k]->alive == false) {
									continue;
								}
								else {
									//If a ball in the map collides with the player ball
									if (map.getMap()[j]->row[k]->spr->getGlobalBounds().intersects(balls[i]->getSprite()->getGlobalBounds())) {
										BallSimple* hit_ball = map.getMap()[j]->row[k];
										sf::Sprite* pball = balls[i]->getSprite();
										//If same colour
										if (hit_ball->col == *balls[i]->getColour()) {
											std::cout << "HIT ! SAME COLOUR : " << j << "," << k << ": " << Ball::ballColourToStr(hit_ball->col) << std::endl;
											hit_ball->alive = false;
											hit_ball->spr->setColor(sf::Color::Transparent);
											*balls[i]->alive() = false;
											balls[i]->kill();
											balls.erase(balls.begin() + i);
											sprites.erase(sprites.begin() + sprites_pball_begin + i);
											sprites_pball_end;
											break;
										}
										else {
											std::cout << "HIT ! OTHER COLOUR : " << j << "," << k << ": " << Ball::ballColourToStr(hit_ball->col) << std::endl;
											hit_ball->alive = false;
											hit_ball->spr->setColor(sf::Color::Transparent);
											*balls[i]->alive() = false;
											break;
										}
									}
								}
							}
							else {
								break;
							}
						}
						if (i > 0){
							if (balls[i] == nullptr || *balls[i]->alive() == false) {
								balls[i]->kill();
								break;
							}
						}
					}

					if (i > 0) {
						if (balls[i - 1] != nullptr) {
							if (balls[i]->getSprite()->getGlobalBounds().intersects(left_bound.getGlobalBounds())) {
								*balls[i]->alive() = false;
								continue;
							}

							if (balls[i]->getSprite()->getGlobalBounds().intersects(right_bound.getGlobalBounds())) {
								*balls[i]->alive() = false;
								continue;
							}

							if (balls[i]->getSprite()->getGlobalBounds().intersects(ball_eraser.getGlobalBounds())) {
								*balls[i]->alive() = false;
								continue;
							}
						}
						else {
							continue;
						}
					}
					else {
						break;
					}
				}
				else {
					balls[i]->kill();
					balls.erase(balls.begin() + i);
					sprites.erase(sprites.begin() + sprites_pball_begin + i);
					sprites_pball_end;
				}
			}
		}*/
	}
	map.update(0, GM_BALL_MOVE_SPEED * TICK_SPEED.asMilliseconds());

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
}

void PlayL1Scene::input(sf::Event* e) {
	p1Cannon.input(e);
}