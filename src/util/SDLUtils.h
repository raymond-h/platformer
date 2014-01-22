#include <memory>
#include <functional>

#include <SDL2/SDL.h>

#ifndef SDL_UTILS_H_
#define SDL_UTILS_H_

typedef std::shared_ptr<SDL_Surface> SurfacePtr;
typedef std::shared_ptr<SDL_Texture> TexturePtr;

extern std::function<void(SDL_Surface*)> surfaceDealloc;
extern std::function<void(SDL_Texture*)> textureDealloc;

SurfacePtr wrap(SDL_Surface*);
SurfacePtr wrapNoDealloc(SDL_Surface*);

TexturePtr wrap(SDL_Texture*);
TexturePtr wrapNoDealloc(SDL_Texture*);

TexturePtr createFromSurface(SDL_Renderer*, SDL_Surface*);
TexturePtr createFromSurface(SDL_Renderer*, SurfacePtr);

#endif /* SDL_UTILS_H_ */