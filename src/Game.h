/*
 * Game.h
 *
 *  Created on: 11 feb 2013
 *      Author: Raymond
 */

#include <memory>

#include <SDL2/SDL.h>
#include "util/SDLUtils.h"

#include "BaseGame.h"

#ifndef GAME_H_
#define GAME_H_

#include "Maps.h"
#include "Player.h"

class Game : public BaseGame {
public:
	Game() {};
	virtual ~Game() {};

	static Game* instance() {
		static Game* inst = new Game();
		return inst;
	};

	static SDL_Renderer* renderer() {
		return instance()->mainRenderer;
	}
	
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

	SDL_Window* window;
	SurfacePtr screen;
	SDL_Renderer* mainRenderer;

	Player* player;
	MapPtr currentMap;
};

#endif /* GAME_H_ */
