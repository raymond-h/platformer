#include "EntityManager.h"

#include "Entity.h"

EntityManager::EntityManager() : entities() {

}

void EntityManager::add(Entity* entity) {
	entities.push_back(entity);
}

std::vector<Entity*> EntityManager::getEntitiesInBounds(Rect<int> bounds) {
	std::vector<Entity*> foundEntities;

	for(Entity* entity : entities) {
		if( bounds.overlaps(entity->bbox()) ) foundEntities.push_back(entity);
	}

	return foundEntities;
}

QuadTree::QuadTree(Rect<int> bounds) : capacity(5), bounds(bounds),
		topLeft(nullptr), topRight(nullptr),
		bottomLeft(nullptr), bottomRight(nullptr),
		entities() {
}

QuadTree::~QuadTree() {
	if(topLeft) delete topLeft;
	if(topRight) delete topRight;
	if(bottomLeft) delete bottomLeft;
	if(bottomRight) delete bottomRight;
}

bool QuadTree::insert(Entity* entity) {
	return false;
}