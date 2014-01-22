#include "Maps.h"

#include <SDL2/SDL.h>
#include "util/SDLUtils.h"

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Vector.h"
#include "Rect.h"

class Player {
public:
	Player();

	void event(const SDL_Event& event);

	void update(unsigned long delta, MapPtr map);

	void render(SurfacePtr screen);

	Vector& getPos() { return pos; };

	FloatRect bbox() { FloatRect r(0,0,w,h); r.setTopLeft(pos + bboxOffset); return r; };

private:
	Vector pos, vel, acc;
	Vector bboxOffset;
	int w, h;

	int keyDir;
};

#endif /* PLAYER_H_ */