#include "Player.h"

#include <iostream>
#include <algorithm>

template<typename T> T sgn(T i) { return i > T(0) ? T(1) : T(-1); };
template<typename T> T abs(T i) { return i * sgn(i); };

float movementAccel = 1024.0f;
float maxSideVel = 128.0f;

bool duckKey = false;
bool ducking = false;
int collideFlags = 0;

Player::Player() : pos(128, 128),vel(0, 0),acc(0, 9.82f * 32.f), bboxOffset(-8,-24),w(16),h(24), keyDir(0) {
}

void Player::event(const SDL_Event& event) {
	if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		// std::cout << (event.key.state == SDL_PRESSED ? "Pressed" : "Released") << " key " << SDL_GetKeyName(event.key.keysym.sym)
		// 	<< "; repeat: " << (int)event.key.repeat << std::endl;

		if(event.key.repeat) return;

		switch(event.key.keysym.sym) {
			case SDLK_LEFT: keyDir += (event.key.state == SDL_PRESSED ? -1 : 1); break;
			case SDLK_RIGHT: keyDir -= (event.key.state == SDL_PRESSED ? -1 : 1); break;

			case SDLK_DOWN: duckKey = (event.key.state == SDL_PRESSED); break;
			case SDLK_UP: if(event.key.state == SDL_PRESSED && (collideFlags & COLLISION_DOWN) != 0 && !ducking) vel.y() = -212.72179013913924f; break;

			default: break;
		}
	}
}

void Player::update(unsigned long delta, MapPtr map) {
	// std::cout << "Delta: " << delta << std::endl;

	float factor = ((float)delta / 1000.f);

	// std::cout << "Factor: " << factor << std::endl;

	if(keyDir != 0) acc.x() = movementAccel * float(keyDir);

	else if( abs(vel.x()) > movementAccel * factor ) {
		acc.x() = -movementAccel * sgn(vel.x());
	}
	else { acc.x() = vel.x() = 0; }

	vel += acc * factor;
	vel.x() = std::min( maxSideVel, abs(vel.x()) ) * sgn( vel.x() );

	FloatRect headBbox(0,0, 16,24);
	headBbox.setCenterX(bbox().getCenterX());
	headBbox.setCenterY(bbox().getCenterY() - 16);

	static auto isTileDense = [](MapPtr map, unsigned long unused, long x, long y) {

		return isTileCollidable(map, x, y);
	};

	bool oldDucking = ducking;

	if(!ducking) {
		if((collideFlags & COLLISION_DOWN) != 0 && duckKey) ducking = true;
	}
	else {
		bool headBlocked = any(map, headBbox, isTileDense);

		if( ((collideFlags & COLLISION_DOWN) == 0 || !duckKey) && !headBlocked) ducking = false;
	}

	h = ducking ? 8 : 24;
	bboxOffset.y() = 8 - h;

	// if((collideFlags & COLLISION_DOWN) == 0) {
	// 	if(!oldDucking && ducking) {
	// 		pos.y() -= 24;
	// 	}
	// 	else if(oldDucking && !ducking) {
	// 		pos.y() += 24;
	// 	}
	// }

	// Physics, collision with world
	Vector adjusts;
	Vector _vel = vel * factor;

	collideFlags = checkCollisionWithMap(map, bbox(), _vel, &adjusts);

	// std::cout << collideFlags << std::endl;

	if( (collideFlags & (COLLISION_UP | COLLISION_DOWN)) != 0 ) {
		vel.y() = _vel.y() = 0;
		pos.y() += adjusts.y();
	}

	if( (collideFlags & (COLLISION_LEFT | COLLISION_RIGHT)) != 0 ) {
		vel.x() = _vel.x() = 0;
		pos.x() += adjusts.x();
	}

	pos += _vel;
}

void Player::render(SDL_Renderer* renderer) {
	SDL_Rect rect = bbox().toSDLRect();
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	// SDL_FillRect( screen.get(), &rect, SDL_MapRGB(screen.get()->format, 255, 0, 0) );
	SDL_RenderFillRect(renderer, &rect);
}