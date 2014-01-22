#include "SDLUtils.h"

std::function<void(SDL_Surface*)> surfaceDealloc = [](SDL_Surface* s) { SDL_FreeSurface(s); };
auto noopDealloc = [](SDL_Surface*){};

SurfacePtr wrap(SDL_Surface* s) {
	return SurfacePtr(s, surfaceDealloc);
}

SurfacePtr wrapNoDealloc(SDL_Surface* s) {
	return SurfacePtr(s, noopDealloc);
}