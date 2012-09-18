#ifndef __BOX_H__
#define __BOX_H__

#include <Entities\Entity.h>
#include <Entities\Box\BoxPhysics.h>

class Box: public Entity {
public:
	Box(float x, float y, float w, float h, bool isStatic, b2World* world) {
		this->m_physics = new BoxPhysics(x, y, w, h, isStatic, world, this);
	}

	void HandleEvents() {}
};

#endif