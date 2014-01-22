/*
 * MapLoading.h
 *
 *  Created on: 17 feb 2013
 *      Author: Raymond
 */

#include <memory>
#include <vector>
#include <functional>

#include <SDL/SDL.h>
#include "util/SDLUtils.h"

#include <boost/filesystem.hpp>

#include <tiledpp.h>

typedef std::shared_ptr<tiledpp::Map> MapPtr;

#ifndef MAPLOADING_H_
#define MAPLOADING_H_

#include "Vector.h"
#include "Rect.h"
#include "Directions.h"

enum { TILEFLAG_DENSE = 1, TILEFLAG_JUMPTHROUGH = 2 };

typedef struct {

	tiledpp::TileLayer* meta;

	/* Enter your map-specific data here */
	unsigned long* tile_flags;
	
} MapUserData;

typedef struct {

	SurfacePtr surface;

	/* Enter your tileset-specific data here */
	
} TilesetUserData;

MapPtr loadMap(const boost::filesystem::path&);

// void applyToArea(tiledpp::TileLayer* layer, long x1, long y1, long x2, long y2, std::function<void(unsigned long long, long, long)> callback);

long calcTileX(MapPtr map, float x);
long calcTileY(MapPtr map, float y);

void renderTileLayer(tiledpp::TileLayer* layer, SurfacePtr destSurface, SDL_Rect* dest, long x, long y);

int checkCollisionWithMap(MapPtr map, FloatRect bbox, Vector vel, Vector* adjusts);

bool isTileCollidable(MapPtr map, long x, long y);

bool isTileSlope(MapPtr map, long x, long y, long* left, long* right);

bool any(MapPtr map, FloatRect bbox, std::function<bool(MapPtr, unsigned long long, long, long)> callback);

#endif /* MAPLOADING_H_ */
