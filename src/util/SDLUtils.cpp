#include "SDLUtils.h"

std::function<void(SDL_Surface*)> surfaceDealloc = [](SDL_Surface* s) { SDL_FreeSurface(s); };
auto noopDeallocSurface = [](SDL_Surface*){};

SurfacePtr wrap(SDL_Surface* s) {
	return SurfacePtr(s, surfaceDealloc);
}

SurfacePtr wrapNoDealloc(SDL_Surface* s) {
	return SurfacePtr(s, noopDeallocSurface);
}

std::function<void(SDL_Texture*)> textureDealloc = [](SDL_Texture* s) { SDL_DestroyTexture(s); };
auto noopDeallocTexture = [](SDL_Texture*){};

TexturePtr wrap(SDL_Texture* s) {
	return TexturePtr(s, textureDealloc);
}

TexturePtr wrapNoDealloc(SDL_Texture* s) {
	return TexturePtr(s, noopDeallocTexture);
}

TexturePtr createFromSurface(SDL_Renderer* r, SDL_Surface* s) {
	return wrap( SDL_CreateTextureFromSurface(r, s) );
}

TexturePtr createFromSurface(SDL_Renderer* r, SurfacePtr s) {
	return createFromSurface(r, s.get());
}