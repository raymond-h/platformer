#include <SDL2/SDL.h>
#include "SDLUtils.h"

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"
#include "Maps.h"

#include "Vector.h"
#include "Rect.h"
#include "Animation.h"

enum PlayerAction {
	PLAYER_ACTION_NONE,
	PLAYER_ACTION_LEFT,
	PLAYER_ACTION_RIGHT,
	PLAYER_ACTION_JUMP,
	PLAYER_ACTION_DUCK
};

class Player : public Entity {
public:
	Player();

	virtual ~Player() {};

	void action(const PlayerAction action, bool pressed);

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