#include <vector>

class EntityManager;
class QuadTree;

#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

class Entity;

#include "Rect.h"

class EntityManager {
public:
	EntityManager();

	~EntityManager();

private:

};

class QuadTree {
public:
	QuadTree(Rect<int> bounds);

	~QuadTree();

	bool insert(Entity* entity);

private:
	int capacity;

	Rect<unsigned long> bounds;

	QuadTree* topLeft;
	QuadTree* topRight;
	QuadTree* bottomLeft;
	QuadTree* bottomRight;

	vector<Entity*> entities;
};

#endif
/* ENTITY_MANAGER_H_ */