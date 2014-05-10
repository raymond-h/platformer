#include <SDL2/SDL.h>
#include "util/SDLUtils.h"

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"
#include "Maps.h"

#include "Vector.h"
#include "Rect.h"
#include "Animation.h"

class Player : public Entity {
public:
	Player();

	virtual ~Player() {};

	void event(const SDL_Event& event);

	void update(unsigned long delta, MapPtr map);

	void render(SDL_Renderer*);

private:
	int keyDir;
	bool oldDuckKey, duckKey;
	bool oldJumpKey, jumpKey;

	bool ducking;
	int lookDir;
	int collideFlags;

	AnimationManager aniMan;
};

#endif /* PLAYER_H_ */