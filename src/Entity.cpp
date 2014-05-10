#include "Entity.h"

Entity::Entity() :
		pos(128, 128), vel(0, 0), acc(0, 9.82f * 32.f),
		bboxOffset(-8,-24), w(16), h(24) {

	
}

Entity::~Entity() {}