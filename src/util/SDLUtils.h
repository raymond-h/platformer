#include <memory>
#include <functional>

#include <SDL/SDL.h>

#ifndef SDL_UTILS_H_
#define SDL_UTILS_H_

typedef std::shared_ptr<SDL_Surface> SurfacePtr;

extern std::function<void(SDL_Surface*)> surfaceDealloc;

SurfacePtr wrap(SDL_Surface*);
SurfacePtr wrapNoDealloc(SDL_Surface*);

#endif /* SDL_UTILS_H_ */