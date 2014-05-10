/*
 * MapLoading.cpp
 *
 *  Created on: 17 feb 2013
 *      Author: Raymond
 */

#include "Maps.h"

#include <cmath>
#include <iostream>

#include <SDL2/SDL_image.h>

#include "Game.h"
#include "GraphicsUtils.h"

using namespace tiledpp;

void freeMap(tiledpp::Map*);

MapPtr loadMap(const boost::filesystem::path& file) {
	namespace fs = boost::filesystem;
	
	MapPtr map( tiledpp::load(file.string()), freeMap );
	map->precacheTilesetRetrieval();
	{
		MapUserData* mud = new MapUserData;
		map->setUserData(mud);

		// Setup map-specific data
		tiledpp::TileLayer* meta = static_cast<tiledpp::TileLayer*>( map->getSingleLayerByName("meta", LAYER_TILE) );
		mud->meta = meta;

		unsigned long long tilesAmnt = meta->getWidth() * meta->getHeight();
		mud->tile_flags = new unsigned long[tilesAmnt];
		for(unsigned long long i = 0; i < tilesAmnt; i++) {
			mud->tile_flags[i] = 0;

			tiledpp::Tileset* tileset = map->findTileset( meta->getTileData()[i] );
			if(!tileset) continue;

			auto props = tileset->tileProps( meta->getTileData()[i] - tileset->getFirstGId() );

			if( props.getPropertyAs<bool>("dense", false) ) mud->tile_flags[i] |= TILEFLAG_DENSE;
			// if( props.getPropertyAs<bool>("jump_through", false) ) mud->tile_flags[i] |= TILEFLAG_JUMPTHROUGH;
		}
	}
	
	TilesetSet tilesets = map->getTilesets();
	for(Tileset* tileset : tilesets) {

		TilesetUserData* tilesetUserData = new TilesetUserData;
		tileset->setUserData(tilesetUserData);

		// Setup tileset-specific data
		auto lastSlash = tileset->getImageSource().find_last_of("\\/");
		std::string fullname = "res/" + tileset->getImageSource().substr(lastSlash+1);

		SDL_Surface* tilesetImg = IMG_Load( fullname.c_str() );

		tilesetUserData->image = createFromSurface(Game::renderer(), tilesetImg);
	}
	
	return map;
}

void freeMap(tiledpp::Map* map) {
	MapUserData* mud = map->getUserDataAs<MapUserData>();

	// Free map-specific data
	delete[] mud->tile_flags;

	delete mud;
	
	TilesetSet tilesets = map->getTilesets();
	for(Tileset* tileset : tilesets) {
		
		delete tileset->getUserDataAs<TilesetUserData>();
	}
	
	tiledpp::free(map);
}

// void applyToArea(tiledpp::TileLayer* layer, long x1, long y1, long x2, long y2, std::function<void(unsigned long long, long, long)> callback) {
// 	for(long x = x1; x <= x2; ++x) {
// 		for(long y = y1; y <= y2; ++y) {
// 			if(y < 0 || y >= layer->getHeight() || x < 0 || x >= layer->getWidth()) continue;
// 			unsigned long long tilePos = ( y * layer->getWidth() + x );
// 			callback(tilePos, x, y);
// 		}
// 	}
// }

void renderTileLayer(tiledpp::TileLayer* layer, SDL_Renderer* destRenderer, SDL_Rect* dest, long mapx, long mapy) {
	SDL_Rect destRect, oldClip;
	if(dest) {
		// SDL_GetClipRect(destSurface.get(), &oldClip);
		// SDL_SetClipRect(destSurface.get(), dest);
		destRect = *dest;
	}
	else {
		int w = 0, h = 0;
		SDL_GetRendererOutputSize(destRenderer, &w, &h);
		destRect = {
				0, 0, w, h
		};
		// std::cout << w << "; " << h << std::endl;
	}
	
	tiledpp::Map* map = layer->getOwner();
	unsigned long* gids = layer->getTileData();

	long tilex = ( mapx / map->getTileWidth() );
	long tiley = ( mapy / map->getTileHeight() );
	if(tilex < 0) tilex = 0;
	if(tiley < 0) tiley = 0;

	long tilew = ( (mapx+destRect.w) / map->getTileWidth() ) + 1;
	long tileh = ( (mapy+destRect.h) / map->getTileHeight() ) + 1;
	if(tilew >= layer->getWidth()) tilew = layer->getWidth();
	if(tileh >= layer->getHeight()) tileh = layer->getHeight();
	
	for(long i = tilex; i < tilew; i++) {
		for(long j = tiley; j < tileh; j++) {
			
			unsigned long long tilePos = ( j*layer->getWidth() + i );
			unsigned long gid = gids[tilePos];
			
			if(gid == 0) continue;

			// std::cout << "Drawing tile " << i << "x" << j << std::endl;
			
			tiledpp::Tileset* tileset = layer->getOwner()->findTileset(gid);
			
			tiledpp::Point p = tileset->getTileCoordsOnSheet(gid);
			
			SDL_Rect srcNow = { (short)p.x, (short)p.y, tileset->getTileWidth(), tileset->getTileHeight() };
			SDL_Rect destNow = {
					short( i * map->getTileWidth() + tileset->getTileOffsetX() - mapx + destRect.x ),
					short( j * map->getTileHeight() + tileset->getTileOffsetY() - mapy + destRect.y ),
					tileset->getTileWidth(), tileset->getTileHeight()
			};

			// std::cout << "Hurr: " << short( i * map->getTileWidth() + tileset->getTileOffsetX() - mapx + destRect.x ) << std::endl;
			
			TexturePtr image = tileset->getUserDataAs<TilesetUserData>()->image;

			// std::cout << "Drawing tile " << srcNow.x << "," << srcNow.y << " to " << destNow.x << "," << destNow.y << std::endl;

			// SDL_BlitSurface( surface.get(), &srcNow, destSurface.get(), &destNow );
			SDL_RenderCopy( destRenderer, image.get(), &srcNow, &destNow );
		}
	}
	
	// if(dest) SDL_SetClipRect(destSurface.get(), &oldClip);
}

template<typename T> T sgn(T i) { return i > T(0) ? T(1) : T(-1); };

long seekCollidedX(MapPtr map, long top, long bottom, long startX, long dist, int dir) {
	// std::cout << "SEARCHING X FROM " << startX << " FOR " << dist << " TILES BETWEEN " << top << ", " << bottom << std::endl;

	if(dist == 0) return 0; // Seek... not at all? Well, then just say 0

	dist = abs(dist);

	for(long y = top; y <= bottom; y++) {
		for(long x = 0; x < dist; x++) {
			bool dense = isTileCollidable(map, startX + x*dir, y);
			// std::cout << "Checking " << (startX + x*dir) << "x" << y << ": " << dense << std::endl;

			if(dense) return x;
		}
	}

	return dist;
}

long seekCollidedY(MapPtr map, long left, long right, long startY, long dist, int dir) {
	if(dist == 0) return 0; // Seek... not at all? Well, then just say 0

	dist = abs(dist);

	for(long x = left; x <= right; x++) {
		for(long y = 0; y < dist; y++) {
			bool dense = isTileCollidable(map, x, startY + y*dir);

			if(dense) return y;
		}
	}

	return dist;
}

long calcTileX(MapPtr map, float x) {
	return std::floor( x / map->getTileWidth() );
}

long calcTileY(MapPtr map, float y) {
	return std::floor( y / map->getTileHeight() );
}

int checkCollisionWithMap(MapPtr map, FloatRect bbox, Vector vel, Vector* adjusts) {
	int collisionFlags = 0;
	auto tileWidth = map->getTileWidth();
	auto tileHeight = map->getTileHeight();

	if(vel.x()) { // X Axis Movement
		float sidePx = ( vel.x() > 0 ? bbox.getRight() : bbox.getLeft() );

		long side = std::floor( sidePx / tileWidth );
		int steps = abs( side - calcTileX(map, sidePx+vel.x()) ) + 1;

		long top = std::floor( bbox.getTop() / tileHeight );
		long bottom = std::floor( (bbox.getBottom()-1) / tileHeight );

		long collidedX = seekCollidedX(map, top, bottom, side, steps, sgn(vel.x()) );

		if(collidedX != steps) {
			// Collision occured on X axis

			long tileSide = ( vel.x() < 0 ? (side-collidedX+1) : side+collidedX ) * tileWidth;
			collisionFlags |= ( vel.x() > 0 ? COLLISION_RIGHT : COLLISION_LEFT );
			adjusts->x() = tileSide - sidePx;
		}
	}

	if(vel.y()) { // Y Axis Movement
		float sidePx = ( vel.y() > 0 ? bbox.getBottom() : bbox.getTop() );

		long side = std::floor( sidePx / tileHeight );
		int steps = abs( side - calcTileY(map, sidePx+vel.y()) ) + 1;

		long left = std::floor( bbox.getLeft() / tileWidth );
		long right = std::floor( (bbox.getRight()-1) / tileWidth );

		long collidedY = seekCollidedY(map, left, right, side, steps, sgn(vel.y()) );

		if(collidedY != steps) {
			// Collision occured on Y axis

			long tileSide = (vel.y() < 0 ? (side-collidedY+1) : side+collidedY) * tileHeight;
			collisionFlags |= ( vel.y() > 0 ? COLLISION_DOWN : COLLISION_UP );
			adjusts->y() = tileSide - sidePx;
		}
	}

	return collisionFlags;
}

bool isTileCollidable(MapPtr map, long x, long y) {
	MapUserData* mud = map->getUserDataAs<MapUserData>();
	unsigned long long tileIndex = y*map->getWidth() + x;

	return (mud->tile_flags[tileIndex] & TILEFLAG_DENSE) != 0;
}

bool any(MapPtr map, FloatRect bbox, std::function<bool(MapPtr, unsigned long long, long, long)> callback) {
	long	x1 = calcTileX(map, bbox.getLeft()),
			y1 = calcTileY(map, bbox.getTop()),
			x2 = calcTileX(map, bbox.getRight()-1),
			y2 = calcTileY(map, bbox.getBottom()-1);

	for(auto x = x1; x <= x2; ++x) {
		for(auto y = y1; y <= y2; ++y) {
			if(y < 0 || y >= map->getHeight() || x < 0 || x >= map->getWidth()) continue;
			unsigned long long tilePos = ( y * map->getWidth() + x );
			if(callback(map, tilePos, x, y)) return true;
		}
	}

	return false;
}

// bool isTileSlope(MapPtr map, long x, long y, long* left, long* right) {
// 	tiledpp::TileLayer* meta = map->getUserDataAs<MapUserData>()->meta;

// 	unsigned long long tileIndex = y*map->getWidth() + x;
// 	tiledpp::Tileset* tileset = map->findTileset( meta->getTileData()[tileIndex] );
// 	if(!tileset) return false;

// 	auto props = tileset->tileProps( meta->getTileData()[tileIndex] - tileset->getFirstGId() );

// 	if(left) props.getProperty("height-left", left);
// 	if(right) props.getProperty("height-right", right);

// 	return props.hasProperty("height-left") || props.hasProperty("height-right");
// }

// Slope movement

// long	slopeX = calcTileX(map, bbox.getCenterX() + vel.x()),
// 		slopeY = calcTileY(map, bbox.getBottom()-1);

// long	heightLeft = 16,
// 		heightRight = 16;

// if(isTileSlope(map, slopeX, slopeY, &heightLeft, &heightRight)) {
// 	heightLeft = 16 - heightLeft;
// 	heightRight = 16 - heightRight;

// 	float posFactor = (bbox.getCenterX() - float(slopeX * tileWidth)) / (float)tileWidth;

// 	std::cout << "Tile " << slopeX << "x" << slopeY << " is a slope!! " <<
// 		"Heights: " << heightLeft << ", " << heightRight << "; We're at " << posFactor << std::endl;

// 	float interpHeight = (heightLeft + float(heightRight - heightLeft) * posFactor) + ( slopeY * tileHeight );

// 	std::cout << interpHeight << "; " << bbox.getBottom() << "; " << (interpHeight - bbox.getBottom()) << std::endl;

// 	collisionFlags |= COLLISION_DOWN;
// 	adjusts->y() = interpHeight - bbox.getBottom();
// }