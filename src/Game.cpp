/*
 * Game.cpp
 *
 *  Created on: 11 feb 2013
 *      Author: Raymond
 */

#include "Game.h"

#include <iostream>
#include <chrono>
#include <ratio>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "util/SDLUtils.h"

#include "Maps.h"
#include "Player.h"

SurfacePtr screen;

Player* player;
MapPtr currentMap;

int Game::init(int argc, char** argv) {
	std::cout << "SDL init... ";
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << std::endl << "Failure to init SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	std::cout << "Success" << std::endl;

	std::cout << "SDL_image init... ";
	int imgFlags = IMG_INIT_PNG;
	if((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
		std::cout << std::endl << "Failure to init SDL_image: " << IMG_GetError() << std::endl;
		return 2;
	}
	std::cout << "Success" << std::endl;

	SDL_WM_SetCaption( "Platformer", "Platformer" );

	screen = wrapNoDealloc( SDL_SetVideoMode(640, 480, 32, SDL_DOUBLEBUF|SDL_HWSURFACE) );

	player = new Player();
	currentMap = loadMap("res/test.tmx");

	return 0;
}

int Game::loop() {
	namespace ch = std::chrono;
	auto first = ch::high_resolution_clock::now();
	
	while(true) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(!this->event( event )) break;
		}
		if(!running) break;
		
		if(!stepMode || runFrame) {
			auto now = ch::high_resolution_clock::now();
			auto time = ch::duration_cast< ch::duration<unsigned long, std::milli> >( now - first );

			this->logic( time.count() );
			if(!running) break;
			
			first = now;

			runFrame = false;
		}

		this->render();
		SDL_Flip(screen.get());
	}

	return 0;
}

void Game::exit(int exitCode) {
	delete player;

	SDL_Quit();
	IMG_Quit();
}

bool Game::event(const SDL_Event& event) { //true == keep processing events, false == break out of event processing NOW
	switch(event.type) {
		case SDL_KEYDOWN: {
			switch(event.key.keysym.sym) {
				case SDLK_p: {
					stepMode = !stepMode;
					break;
				}

				case SDLK_i: {
					if(stepMode) runFrame = true;
					else stepMode = true;

					break;
				}

				default: break;
			}
			break;
		}

		case SDL_QUIT: {
			running = false;
			return false; //Do not keep processing events - the user just quit the game anyway
		}

		default: break;
	}

	player->event(event);

	return true; //Keep processing by default
}

void Game::logic(unsigned long delta) {
	player->update(delta, currentMap);
}

void Game::render() {
	SDL_FillRect(screen.get(), nullptr, SDL_MapRGB(screen.get()->format, 0, 0, 127));
	// std::cout << "render" << std::endl;
	tiledpp::TileLayer* main = (tiledpp::TileLayer*)currentMap->getSingleLayerByName("main", tiledpp::LAYER_TILE);

	renderTileLayer(main, screen, nullptr, 0, 0);

	// tiledpp::TileLayer* meta = (tiledpp::TileLayer*)currentMap->getSingleLayerByName("meta", tiledpp::LAYER_TILE);

	// renderTileLayer(meta, screen, nullptr, 0, 0);

	player->render(screen);
}