#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include <Game/Objects/Entity.h>
#include <Game/Objects/Circle/CirclePhysics.h>

class Circle: public Entity {
public:
	Circle(float x, float y, float w, bool isStatic, b2World* world) { 
		this->m_physics = new CirclePhysics(x, y, w, isStatic, world, this);
	}

	void HandleEvents() { } 
};
#endif 