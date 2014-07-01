#include "GraphicsUtils.h"

#include <iostream>

void getColorComponents(SDL_PixelFormat* fmt, Uint32 pixel, unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a) {
	if(r) *r = ( (pixel & fmt->Rmask) >> fmt->Rshift ) << fmt->Rloss;
	if(g) *g = ( (pixel & fmt->Gmask) >> fmt->Gshift ) << fmt->Gloss;
	if(b) *b = ( (pixel & fmt->Bmask) >> fmt->Bshift ) << fmt->Bloss;
	if(a) *a = ( (pixel & fmt->Amask) >> fmt->Ashift ) << fmt->Aloss;
}

void getColorComponents(SDL_PixelFormat* fmt, Uint32 pixel, tiledpp::Color* c) {
	getColorComponents(fmt, pixel, &(c->r), &(c->g), &(c->b), &(c->a));
}

// void renderTile(SurfacePtr dest, tiledpp::TileLayer* layer, long tileX, long tileY) {
// 	unsigned long long tilePos = ( tileY*layer->getWidth() + tileX );
// 	unsigned long gid = layer->getTileData()[tilePos];
// 	unsigned long colorNum = layer->getOwner()->getUserDataAs<MapUserData>()->region_nums[tilePos];

// 	tiledpp::Tileset* tileset = layer->getOwner()->findTileset(gid);
// 	tiledpp::Point p = tileset->getTileCoordsOnSheet(gid);
// }

//Assumes both surfaces have bit depth 32-bit
//color should be based on the format in src
void blitTinted(SurfacePtr src, SDL_Rect* srcRect, SurfacePtr dest, SDL_Rect* destRect, Uint32 color) {
}

void blitByMask(SurfacePtr src, SDL_Rect* srcRect, SurfacePtr dest, SDL_Rect* destRect, SurfacePtr mask, short maskX, short maskY, Uint32 color) {
	SurfaceLock srcLock(src);
	SurfaceLock destLock(dest);
	SurfaceLock maskLock(mask);

	short destX, destY;
	if(destRect) { destX = destRect->x; destY = destRect->y; }
	else destX = destY = 0;

	for(short currY = 0; currY < srcRect->h; ++currY) {
		for(short currX = 0; currX < srcRect->w; ++currX) {
			bool useMask = (destY + currY) > maskY && (destY + currY) < (maskY + mask->h) && (destX + currX) > maskX && (destX + currX) < (maskX + mask->w);
			if(!useMask) continue;

			//std::cout << "hurr" << std::endl;
			//std::cout << "(destY + currY) = " << (destY + currY) << " and maskY = " << maskY << " and (maskY + mask->h) = " << (maskY + mask->h) << std::endl;
			//std::cout << "(destX + currX) = " << (destX + currX) << " and maskX = " << maskX << " and (maskX + mask->w) = " << (maskX + mask->w) << std::endl;

			long srcPos = (currY + srcRect->y) * src->w + (currX + srcRect->x);
			long destPos = (currY + destY) * dest->w + (currX + destX);

			tiledpp::Color cSrc;
			tiledpp::Color cBlend;
			unsigned char destR, destG, destB;
			unsigned char maskA;

			getColorComponents(src->format, ((Uint32*)src->pixels)[srcPos], &(cSrc.r), &(cSrc.g), &(cSrc.b), &(cSrc.a) );
			getColorComponents(dest->format, color, &(cBlend.r), &(cBlend.g), &(cBlend.b), nullptr );
			cBlend.a = 255;
			cSrc = multiply(cSrc, cBlend);

			getColorComponents(dest->format, ((Uint32*)dest->pixels)[destPos], &destR, &destG, &destB, nullptr );

			float alpha = ((float)cSrc.a) / 255.0f; //Alpha should be based on the mask instead of (or as well as) on the source surface
			
			long maskPos = (currY + destY - maskY) * mask->w + (currX + destX - maskX);
			getColorComponents(mask->format, ((Uint32*)mask->pixels)[maskPos], &maskA, nullptr, nullptr, nullptr );

			alpha *= ((float)maskA) / 255.0f; //Which it is now
			//std::cout << "Got the value " << (int)maskA << " at " << maskPos
			//		<< " (which is " << (currX + destX - maskX) << "x" << (currY + destY - maskY) << ")" << std::endl;

			char finalR = (cSrc.r * alpha) + ( destR * (1.0f - alpha) );
			char finalG = (cSrc.g * alpha) + ( destG * (1.0f - alpha) );
			char finalB = (cSrc.b * alpha) + ( destB * (1.0f - alpha) );

			((Uint32*)dest->pixels)[destPos] = SDL_MapRGB(dest->format, finalR, finalG, finalB);
		}
	}
}