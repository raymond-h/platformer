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

#include <SDL2/SDL_image.h>

#include "Animation.h"

int Game::init(int argc, char** argv) {
	std::cout << "SDL init... ";
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << std::endl << "Failure to init SDL: " << SDL_GetError() << std::endl;
		return 1;
	}
	std::cout << "Success" << std::endl;

	std::cout << "SDL_image init... ";
	int imgFlags = IMG_INIT_PNG;
	if((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
		std::cerr << std::endl << "Failure to init SDL_image: " << IMG_GetError() << std::endl;
		return 2;
	}
	std::cout << "Success" << std::endl;

	// SDL_WM_SetCaption( "Platformer", "Platformer" );
	window = SDL_CreateWindow(
		"Platformer",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Window position
		640, 480, // Window size
		SDL_WINDOW_SHOWN
	);
	if(!window) {
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
	}

	// screen = wrapNoDealloc( SDL_SetVideoMode(640, 480, 32, SDL_DOUBLEBUF|SDL_HWSURFACE) );
	// screen = wrapNoDealloc( SDL_GetWindowSurface(window) );
	mainRenderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );

	std::cout << "setAnimation???" << std::endl;

	player = new Player();
	std::cout << "bounce bounce" << std::endl;
	currentMap = loadMap("res/test.tmx");

	std::cout << "Hurr" << std::endl;

	return 0;
}

void Game::exit(int exitCode) {
	delete player;

	SDL_DestroyRenderer(mainRenderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	IMG_Quit();
}

unsigned long msCount = 0;
unsigned long msCountPerSecond = 0;
unsigned long frameCount = 0;
float fps = 0;

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

			msCountPerSecond += time.count();
			if(msCountPerSecond >= 1000) {
				fps = (float)frameCount / ((float)msCountPerSecond / 1000.0f);
				
				std::cout << "FPS: " << fps << " (" << frameCount << " frames over " << msCountPerSecond << "ms)" << std::endl;

				msCountPerSecond -= 1000;
				frameCount = 0;
			}

			msCount += time.count();
			while(msCount > 1000/120) {
				this->logic( 1000/120 );
				msCount -= 1000/120;
				frameCount++;
			}
			// this->logic( time.count() );

			if(!running) break;
			
			first = now;

			runFrame = false;
		}

		this->render();
	}

	return 0;
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

	// player->event(event);
	if((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {
		bool pressed = (event.key.state == SDL_PRESSED);

		switch(event.key.keysym.sym) {
			case SDLK_LEFT: player->action(PLAYER_ACTION_LEFT, pressed); break;
			case SDLK_RIGHT: player->action(PLAYER_ACTION_RIGHT, pressed); break;

			case SDLK_DOWN: player->action(PLAYER_ACTION_DUCK, pressed); break;
			case SDLK_UP: case SDLK_z: player->action(PLAYER_ACTION_JUMP, pressed); break;

			default: break;
		}
	}

	return true; //Keep processing by default
}

void Game::logic(unsigned long delta) {
	// delta = 17;
	player->update(delta, currentMap);
}

void Game::render() {
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 127, 255);

	SDL_RenderClear(mainRenderer);

	tiledpp::TileLayer* main = (tiledpp::TileLayer*)currentMap->getSingleLayerByName("main", tiledpp::LAYER_TILE);
	renderTileLayer(main, mainRenderer, nullptr, 0, 0);

	player->render(mainRenderer);

	SDL_RenderPresent(mainRenderer);
}