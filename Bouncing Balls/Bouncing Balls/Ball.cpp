#include "Ball.h"

//Conerts between custom colour enum to string based on switch/case
std::string Ball::ballColourToStr(BallColour b) {
	switch (b) {
	case BallColour::RED:
		return "red";//done
	case BallColour::AMBER:
		return "amber";//done
	case BallColour::ORANGE:
		return "orange";//done
	case BallColour::YELLOW:
		return "yellow";//done
	case BallColour::LIME:
		return "lime";//done
	case BallColour::ROYAL_BLUE:
		return "royal_blue";//done
	case BallColour::DARK_BLUE:
		return "dark_blue";//done
	case BallColour::GREEN:
		return "green";//done
	case BallColour::EM_GREEN:
		return "em_green";//done
	case BallColour::TURQOUISE:
		return "turq";//done
	case BallColour::CYAN:
		return "cyan";//done
	case BallColour::WINE:
		return "wine";
	case BallColour::VIOLET:
		return "violet";
	case BallColour::PURPLE:
		return "purple";
	case BallColour::DARK_PURPLE:
		return "dark_purple";//done
	case BallColour::BRONZE:
		return "bronze";
	case BallColour::COBALT:
		return "cobalt";
	}
}

//Converts between string and custom colour enum
BallColour Ball::ballColourFromStr(std::string s) {
	s = *Utils::strToLower(&s);
	switch (s.c_str()[0]) {
	case 'r':
		switch (s.c_str()[1]) {
		case 'e':
			return BallColour::RED;
		case 'o':
			return BallColour::ROYAL_BLUE;
		}
	case 'a':
		return BallColour::AMBER;
	case 'o':
		return BallColour::ORANGE;
	case 'y':
		return BallColour::YELLOW;
	case 'l':
		return BallColour::LIME;
	case 'd':
		switch (s.c_str()[5]) {
		case 'b':
			return BallColour::DARK_BLUE;
		case 'p':
			return BallColour::DARK_PURPLE;
		}
	case 'g':
		return BallColour::GREEN;
	case 'e':
		return BallColour::EM_GREEN;
	case 'c':
		switch (s.c_str()[1]) {
		case 'y':
			return BallColour::CYAN;
		case 'o':
			return BallColour::COBALT;
		}
	case 't':
		return BallColour::TURQOUISE;
	case 'w':
		return BallColour::WINE;
	case 'v':
		return BallColour::VIOLET;
	case 'p':
		return BallColour::PURPLE;
	case 'b':
		return BallColour::BRONZE;
	}
}

Ball::Ball(int x, int y, BallColour b) {
	spr = new sf::Sprite(*AssetManager::getTexture("ball_" + ballColourToStr(b)));
	spr->setOrigin(spr->getLocalBounds().width / 2, spr->getLocalBounds().height / 2);
	spr->setScale(0.1f, 0.1f);
	spr->setPosition(x, y);
	target = sf::Vector2f();
	motion = sf::Vector2f();
	col = b;
	if (isAlive == nullptr || *isAlive != true) {
		isAlive = new bool(true);
	}
}

sf::Vector2f* Ball::getTarget() {
	return &target;
}

sf::Sprite* Ball::getSprite() {
	return spr;
}

bool* Ball::alive() {
	return isAlive;
}

void Ball::path_plot(float y, float cannon_rot, sf::Vector2f spos) {
	sf::Vector2f start = spos;
	spr->setPosition(start);
	float rot = cannon_rot;
	bool left = false;

	//Adjust angle to be from -180 to +180
	if (rot > 300) {
		rot -= 360;
		rot = -rot;
		left = true;
	}
	//Get hypotenuse
	float hyp = y / (cos(rot * 3.14f / 180));
	//Get length of x
	float fx = sqrt(pow(hyp, 2) - pow(y, 2));
	//float fx = sin(rot*3.14f/180) * hyp;
	std::cout << "Rot: " << rot << " Hyp: " << hyp << " Fx: " << fx << std::endl;

	//Change x direction based on  angle
	if (left) {
		target.x = start.x - fx;

	}
	else {
		target.x = start.x + fx;
	}

	//Target y
	target.y = start.y - y;

	//Amount of iterations at regular speed to facilitate all of the x boundary
	float iterx = fx / GM_BALL_SHOOT_SPEED;
	//Amount of iterations at regular speed to facilitate all of the y boundary
	float itery = y / GM_BALL_SHOOT_SPEED;

	float diffx = 0;
	if (iterx > 0) {
		diffx = itery / iterx;
	}

	float shoot_speed = GM_BALL_SHOOT_SPEED * TICK_SPEED.asMilliseconds();

	motion.y = diffx * shoot_speed;
	if (motion.y > shoot_speed) {
		float xadj = motion.y / shoot_speed;
		motion.y = -(shoot_speed);

		if (left) {
			motion.x = -shoot_speed;
		}
		else {
			motion.x = shoot_speed;
		}

		motion.x /= xadj;
		std::cout << diffx << " @ " << motion.y << " XADJ: " << xadj << std::endl;
	}
	else {
		if (left) {
			motion.x = -shoot_speed;
		}
		else {
			motion.x = shoot_speed;
		}
		motion.y = -(shoot_speed);
	}

}

void Ball::update() {
	spr->move(motion);
	//spr->setPosition(spr->getPosition().x + motion.x, spr->getPosition().y + motion.y);
	
}

void Ball::kill() {

}

float* Ball::getID() {
	return idRef;
}

void Ball::setID(float id) {
	*idRef = id;
}

BallColour* Ball::getColour() {
	return &col;
}

BallMap::BallMap() {

}

float BallMap::getBlanks() {
	return blanks_start;
}

BallMap::BallMap(int rows, int columns, int min_colours, int max_colours, int startx, int starty, int dir) {
	std::cout << "[BallMap]Creating new map..." << std::endl;
	std::cout << "[BallMap]Rows: " << rows << ", Columns: " << columns << std::endl;
	std::cout << "[BallMap]Minimum colours: " << min_colours << ", Max colours: " << max_colours << std::endl;
	//Initalise local reference variables
	this->map_rows = rows;
	this->map_cols = columns;
	this->min_colours = min_colours;
	this->max_colours = max_colours;
	this->start_x = startx;
	this->start_y = starty;
	this->dir = dir;

	ref = sf::Sprite(*AssetManager::getTexture("ball_red"));
	ref.setScale(0.1f, 0.1f);
	this->end_x = startx + (columns * ref.getGlobalBounds().width);
	this->end_y = starty - (rows * ref.getGlobalBounds().height);

	//Initalise map
	map = std::vector<BallRow*>();

	int cur_cols = 0;
	blanks_start = 0;

	//Generate map
	for (int i = 0; i < map_rows; i++) {
		//Local determinator variable
		cur_cols = 0;

		//Check if current row is odd or even (even rows have <columns> amount of columns, odd rows have <columns>-1 columns
		if (i % 2 == 0) {
			cur_cols = map_cols-1;
		}
		else {
			cur_cols = map_cols - 2;
		}

		//Allocate memory for new row
		map.push_back(new BallRow);
		map[i]->id = i;
		//Loop through row per column and allocate ball
		for (int j = 0; j < cur_cols+1;j++) {
			//Initalise sprite in memory
			map[i]->row.push_back(new BallSimple);
		}
	}
	for(int i=0;i<map_rows; i++){
		//Local determinator variable
		cur_cols = 0;

		//Check if current row is odd or even (even rows have <columns> amount of columns, odd rows have <columns>-1 columns
		if (i % 2 == 0) {
			cur_cols = map_cols - 1;
		}
		else {
			cur_cols = map_cols - 2;
		}

		for (int j = 0; j < cur_cols+1;j++) {
			map[i]->row[j]->col = BallColour();
			if (max_colours - min_colours == 0) {
				map[i]->row[j]->col = static_cast<BallColour>((rand() % (2)));
			}
			else {
				//srand(time(NULL));
				map[i]->row[j]->col = static_cast<BallColour>(rand() % (max_colours));
				std::cout << "[BallMap][" << i << "," << j << "]Ball Colour: " << Ball::ballColourToStr(map[i]->row[j]->col) << std::endl;
			}
			//Set up sprite
			map[i]->id = map_cols + 1 - i;
			map[i]->row[j]->spr = new sf::Sprite(*AssetManager::getTexture("ball_" + Ball::ballColourToStr(map[i]->row[j]->col)));
			map[i]->row[j]->spr->setScale(0.1f, 0.1f);
			map[i]->row[j]->alive = true;
			map[i]->row[j]->id = j;

			//Set up sprite position
			if (i % 2 == 0) {
				map[i]->row[j]->spr->setPosition(startx + (j * ref.getGlobalBounds().width), starty + (dir * (i* ref.getGlobalBounds().height)));
			}
			else {
				map[i]->row[j]->spr->setPosition(startx + (j * ref.getGlobalBounds().width) + (ref.getGlobalBounds().width / 2), starty + (dir * (i * ref.getGlobalBounds().height)));
			}

		}
	}
}

float BallMap::addRow(bool isBlank) {
	

	long cols = 0;
	if ((map[1]->row.size()) == map_cols) {
		cols = map_cols;
	}
	else if ((map[1]->row.size()) == map_cols-1) {
		cols = map_cols-1;
	}

	map.insert(map.begin(), new BallRow);
	map[0]->id = map[1]->id + 1;

	ref.move(0, -(map_rows * ref.getGlobalBounds().height));
	for (int i = 0; i < cols; i++) {
		map[0]->row.push_back(new BallSimple);
		map[0]->row[i]->spr = new sf::Sprite(*AssetManager::getTexture("ball_" + Ball::ballColourToStr(BallColour::RED)));
		if (isBlank) {
			map[0]->row[i]->spr->setColor(sf::Color::Transparent);
		}
		map[0]->row[i]->id = i;
		map[0]->row[i]->col = BallColour::RED;
		map[0]->row[i]->spr->setScale(0.1f, 0.1f);
		map[0]->row[i]->id = i;
		map[0]->row[i]->alive = false;
		
		if (cols == (map_cols+1)) {
			map[0]->row[i]->spr->setPosition(start_x + (i * ref.getGlobalBounds().width), map[1]->row[0]->spr->getPosition().y + (ref.getGlobalBounds().height));
		}
		else {
			map[0]->row[i]->spr->setPosition(start_x + (i * ref.getGlobalBounds().width) + (ref.getGlobalBounds().width / 2), map[1]->row[0]->spr->getPosition().y + (ref.getGlobalBounds().height));
		}
		
		start_y = map[0]->row[i]->spr->getPosition().y;

		std::cout << "New Ball @ " << map_rows << "/" << i << ": x-" << map[0]->row[i]->spr->getPosition().x << " ~ y-" << map[0]->row[i]->spr->getPosition().y << std::endl;
	}
	return ++map_rows;
}

float BallMap::getCols() {
	return map_cols;
}

float BallMap::getRows() {
	return map_rows;
}

void BallMap::update(float mx, float my) {
	for (auto& row : map) {
		for (auto& ball : row->row) {
			ball->spr->move(mx, my);
			if (ball->alive == false) {
				if (ball->spr->getColor() != sf::Color::Transparent) {
					ball->spr->setColor(sf::Color::Transparent);
				}
			}
		}
	}
}

std::vector<BallRow*> BallMap::getMap() {
	return map;
}

BallSimple* BallMap::getBall(int rownum, int colnum) {
	return map[rownum]->row[colnum];
}

BallRow* BallMap::getRow(int rownum) {
	return map[rownum];
}

sf::Vector2f BallMap::getEnd() {
	return sf::Vector2f(end_x, end_y);
}

sf::Vector2f BallMap::getStart() {
	return sf::Vector2f(start_x, start_y);
}