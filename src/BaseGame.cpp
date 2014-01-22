/*
 * Game.cpp
 *
 *  Created on: 11 feb 2013
 *      Author: Raymond
 */

#include "BaseGame.h"

BaseGame::BaseGame():running(false) {
}

BaseGame::~BaseGame() {
}

int BaseGame::run(int argc, char** argv) {
	int return_val;
	if( (return_val = this->init(argc, argv)) != 0 ) return return_val;
	running = true;
	return_val = this->loop();
	this->exit(return_val);
	return return_val;
}

int BaseGame::init(int argc, char** argv) {
	return 0;
}

int BaseGame::loop() {
	return 0;
}

void BaseGame::exit(int exitCode) {
}
