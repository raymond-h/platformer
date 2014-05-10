#include <SDL2/SDL.h>
#include "util/SDLUtils.h"

class Entity;

#ifndef ENTITY_H_
#define ENTITY_H_

#include "Maps.h"

class Entity {
public:
	virtual ~Entity();

	virtual void event(const SDL_Event& event) = 0;

	virtual void update(unsigned long delta, MapPtr map) = 0;

	virtual void render(SDL_Renderer*) = 0;
};

#endif
/* ENTITY_H_ */