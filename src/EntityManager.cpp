#include "EntityManager.h"

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
	
}