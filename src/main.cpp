/*
 * main.cpp
 *
 *  Created on: 9 feb 2013
 *      Author: Raymond
 */

#include "Game.h"

#undef main

int main(int argc, char** argv) {
	Game* game = Game::instance();
	int returnval = game->run(argc, argv);
	delete game;
	return returnval;
}
