#include <memory>

#include <SDL2/SDL.h>

#include <tiledpp/tiledpp.h>

#ifndef GRAPHICSUTILS_H_
#define GRAPHICSUTILS_H_

typedef std::shared_ptr<SDL_Surface> SurfacePtr;

class SurfaceLock {
public:
	SurfaceLock(SurfacePtr p):ptr(p) { if(SDL_MUSTLOCK(p.get())) SDL_LockSurface(p.get()); };
	~SurfaceLock() { if(SDL_MUSTLOCK(ptr.get())) SDL_UnlockSurface(ptr.get()); };

private:
	SurfacePtr ptr;
};

inline tiledpp::Color multiply(tiledpp::Color c1, tiledpp::Color c2) {
	return {
		(unsigned char)( ( (((float)c1.r)/255.0f) * (((float)c2.r)/255.0f) ) * 255.0f),
		(unsigned char)( ( (((float)c1.g)/255.0f) * (((float)c2.g)/255.0f) ) * 255.0f),
		(unsigned char)( ( (((float)c1.b)/255.0f) * (((float)c2.b)/255.0f) ) * 255.0f),
		(unsigned char)( ( (((float)c1.a)/255.0f) * (((float)c2.a)/255.0f) ) * 255.0f)
	};
}

void getColorComponents(SDL_PixelFormat* format, Uint32 pixel, unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a);
void getColorComponents(SDL_PixelFormat* format, Uint32 pixel, tiledpp::Color* c);

void renderTile(SurfacePtr dest, tiledpp::TileLayer* layer, long tileX, long tileY);

void blitTinted(SurfacePtr src, SDL_Rect* srcRect, SurfacePtr dest, SDL_Rect* destRect, Uint32 color);

void blitByMask(SurfacePtr src, SDL_Rect* srcRect, SurfacePtr dest, SDL_Rect* destRect, SurfacePtr mask, short maskX, short maskY, Uint32 color);

#endif /* GRAPHICSUTILS_H_ */