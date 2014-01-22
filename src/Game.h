/*
 * Game.h
 *
 *  Created on: 11 feb 2013
 *      Author: Raymond
 */

#include <memory>

#include <SDL2/SDL.h>

#include "BaseGame.h"

#ifndef GAME_H_
#define GAME_H_

class Game : public BaseGame {
public:
	Game() {};
	virtual ~Game() {};

	static Game* instance() {
		static Game* inst = new Game();
		return inst;
	};
	
protected:
	int init(int argc, char** argv);
	
	int loop();
	
	void exit(int exitCode);
	
protected:
	bool event(const SDL_Event&);
	
	void logic(unsigned long delta);
	
	void render();

private:
	bool stepMode = false;
	bool runFrame = false;
};

#endif /* GAME_H_ */
