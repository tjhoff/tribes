#include "GameState.h"

static float game_time_elapsed = 0;

float GameState::getGameTime(){
	return game_time_elapsed;
}

void GameState::addGameTime(float add){
	game_time_elapsed += add;
}
