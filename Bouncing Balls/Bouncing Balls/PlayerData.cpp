#include "PlayerData.h"

//Initalise static variable
PlayerData Player::data;

/*
Simple initaliser for Player
Fetches data (if file exists)
*/
void Player::init() {
	if (Player::load() != 0) {
		data = PlayerData();
		for (int i = 0; i < 6; i++) {
			data.campaign_comp[i] = false;
			data.campaign_stars[i] = 0;
		}
		data.challenge_hs = 0;
		data.campaign_finished = false;
		data.coins = 0;
		data.endless_hs = 0;
		Player::save();
	}
}

//Simple get for player data struct
PlayerData* Player::getData() {
	return &data;
}

//Loads p.dat (player data binary file) from disk and uploads into local variable
int Player::load() {
	//Open read stream
	std::ifstream ifstr("p.dat", std::ios::out | std::ios::binary);

	//Check if stream successfully open
	if (!ifstr) {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ERRO
		//Debug only! notify console of issue with loading
		std::cout << "[Player]Could not open PlayerData<p.dat>, using empty!" << std::endl;
	#endif
#endif
		//did not successfully complete
		return -1;
	}
	//Read player data from file
	ifstr.read((char*)&data, sizeof(PlayerData));
	//Close stream
	ifstr.close();
	//Check if stream closed properly
	if (!ifstr.good()) {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ERRO
		//Debug only! notify console of issue with reading
		std::cout << "[Player]Could not load PlayerData<p.dat>!" << std::endl;
	#endif
#endif
		//did not successfully complete
		return -1;
	}
	//sucessful completion 
	return 0;
}

//Saves local player data variable to disk under p.dat
int Player::save() {
	//Open write stream
	std::ofstream ostr("p.dat", std::ios::out | std::ios::binary);

	//Check if stream sucessfully open
	if (!ostr) {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ERRO
		//Debug only! notify console of issue with opening
		std::cout << "[Player]Could not open PlayerData<p.dat>!" << std::endl;
	#endif
#endif
		//did not sucessfuly complete
		return -1;
	}
	//Write local variable playerdata to file
	ostr.write((char*)&data, sizeof(PlayerData));
	//Close stream
	ostr.close();
	//Check if write sucessfuly complete
	if (!ostr.good()) {
#ifdef DEBUG_ENABLED
	#if DEBUG_LEVEL >= DB_LEVEL_ERRO
		//Debug only! notify console of issue with writing
		std::cout << "[Player]Could not save PlayerData<p.dat>!" << std::endl;
	#endif
#endif
		//did not sucessfully complete
		return -1;
	}
	//sucessfully completed
	return 0;
}