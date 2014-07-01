#include "PlayerEntity.h"

#include <iostream>
#include <algorithm>

#include "Game.h"

template<typename T> T sgn(T i) { return i > T(0) ? T(1) : T(-1); };
template<typename T> T abs(T i) { return i * sgn(i); };

float movementAccel = 1024.0f;
float maxSideVel = 100.0f;

PlayerEntity::PlayerEntity() : Entity(),
		keyDir(0), oldJumpKey(false), jumpKey(false),
		oldDuckKey(false), duckKey(false),
		ducking(false), collideFlags(0), lookDir(1),
		aniMan("res/main.json") {

	aniMan.setAnimation("stand");
}

void PlayerEntity::action(const PlayerAction action, bool pressed) {
	switch(action) {
		case PLAYER_ACTION_LEFT: keyDir += (pressed ? -1 : 1); break;
		case PLAYER_ACTION_RIGHT: keyDir -= (pressed ? -1 : 1); break;

		case PLAYER_ACTION_DUCK: duckKey = pressed; break;
		case PLAYER_ACTION_JUMP:  jumpKey = pressed; break;
		default: break;
	}
}

void PlayerEntity::update(unsigned long delta, MapPtr map) {
	// std::cout << "Delta: " << delta << std::endl;

	float factor = ((float)delta / 1000.f);

	auto playerData = Game::getPlayerData();

	// std::cout << "Factor: " << factor << std::endl;

	// Handle button input
	// -- Walking
	if(keyDir != 0) acc.x() = movementAccel * float(keyDir);

	else if( abs(vel.x()) > movementAccel * factor ) {
		acc.x() = -movementAccel * sgn(vel.x());
	}
	else { acc.x() = vel.x() = 0; }

	// -- Wall sliding
	bool wallSliding = false;

	if( playerData.powerup && (
		collideFlags & COLLISION_DOWN) == 0 && vel.y() > 0 &&
		(
			((collideFlags & COLLISION_LEFT) != 0 && keyDir == -1) ||
			((collideFlags & COLLISION_RIGHT) != 0 && keyDir == 1)
		)
	) {

		acc.y() = 0;
		vel.y() = 24.0f;
		wallSliding = true;
	}
	else {
		acc.y() = 9.82f * 32.f;
	}

	// -- Jumping
	if(jumpKey && !oldJumpKey &&
		((collideFlags & COLLISION_DOWN) != 0 || wallSliding) && !ducking ) {

		vel.y() = -212.72179013913924f;
		if(wallSliding) {
			// vel.y() *= 1.1f;
			vel.x() = -keyDir * 900.0f;
		}
	}
	else if(!jumpKey && oldJumpKey && vel.y() < 0 && (collideFlags & COLLISION_DOWN) == 0) {
		vel.y() = 20.0f;
	}

	oldJumpKey = jumpKey;
	oldDuckKey = duckKey;

	// -- X velocity limiting
	vel += acc * factor;
	vel.x() = std::min( maxSideVel, abs(vel.x()) ) * sgn( vel.x() );

	// -- Ducking
	FloatRect headBbox(0,0, 16,24);
	headBbox.setCenterX(bbox().getCenterX());
	headBbox.setCenterY(bbox().getCenterY() - 16);

	static auto isTileDense = [](MapPtr map, unsigned long unused, long x, long y) {

		return isTileCollidable(map, x, y);
	};

	if(!ducking) {
		if((collideFlags & COLLISION_DOWN) != 0 && duckKey) ducking = true;
	}
	else {
		bool headBlocked = any(map, headBbox, isTileDense);

		if( ((collideFlags & COLLISION_DOWN) == 0 || !duckKey) && !headBlocked) ducking = false;
	}

	h = ducking ? 8 : 24;
	bboxOffset.y() = 8 - h;

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
	// std::cout << "Hurr?" << std::endl;

	// Update graphics
	// if(vel.x()) lookDir = (int)sgn(vel.x());
	if(keyDir) lookDir = keyDir;

	if( (collideFlags & COLLISION_DOWN) == 0 ) aniMan.setAnimation("jump");
	else if(vel.x()) aniMan.setAnimation("run");
	else aniMan.setAnimation("stand");

	aniMan.update(delta);
}

void PlayerEntity::render(SDL_Renderer* renderer) {
	// SDL_Rect rect = bbox().toSDLRect();
	// SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	// SDL_FillRect( screen.get(), &rect, SDL_MapRGB(screen.get()->format, 255, 0, 0) );
	// SDL_RenderFillRect(renderer, &rect);

	// std::cout << "RENDER #1" << std::endl;
	SDL_Rect src = aniMan.getArea();
	// std::cout << "RENDER #2; " << src.x << "," << src.y << " - " << src.w << "," << src.h << std::endl;
	TexturePtr texture = aniMan.texture();
	// std::cout << "RENDER #3" << std::endl;

	LongRect destRect(0,0, 32, 32);
	// std::cout << "RENDER #4" << std::endl;
	destRect.setCenterPos( bbox().getCenterPos() );
	// std::cout << "RENDER #5" << std::endl;
	SDL_Rect dest = destRect.toSDLRect();
	// std::cout << "RENDER #6; " << dest.x << "," << dest.y << " - " << dest.w << "," << dest.h << std::endl;

	SDL_RenderCopyEx(renderer, texture.get(), &src, &dest, 0.0, nullptr, lookDir == 1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}