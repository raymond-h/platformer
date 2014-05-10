#include <SDL2/SDL.h>
#include "util/SDLUtils.h"

class Entity;

#ifndef ENTITY_H_
#define ENTITY_H_

#include "Maps.h"
#include "Vector.h"
#include "Rect.h"

class Entity {
public:
	Entity();

	virtual ~Entity();

	virtual void event(const SDL_Event& event) = 0;

	virtual void update(unsigned long delta, MapPtr map) = 0;

	virtual void render(SDL_Renderer*) = 0;

	Vector& getPos() { return pos; };

	FloatRect bbox() { FloatRect r(0,0,w,h); r.setTopLeft(pos + bboxOffset); return r; };

protected:
	Vector pos, vel, acc;
	Vector bboxOffset;
	int w, h;
};

#endif
/* ENTITY_H_ */