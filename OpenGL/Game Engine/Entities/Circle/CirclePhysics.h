#ifndef __CIRCLE_PHYSICS_H__
#define __CIRCLE_PHYSICS_H__

#include <PhysicsCommon.h>
#include <IPhysicsComponent.h>

class CirclePhysics: public IPhysicsComponent { 
public: 
	CirclePhysics(float x, float y, float w, bool isStatic, b2World* world, IEntity* entity) { 
		this->m_entity = entity;
		this->m_world = world;

		b2BodyDef bodyDef;
		bodyDef.position.Set(GetRealWH(x), GetRealWH(y));
		bodyDef.type = (isStatic) ? b2_staticBody : b2_dynamicBody;
		bodyDef.userData = entity;

		this->m_body = world->CreateBody(&bodyDef);

		b2CircleShape circleDef;
		circleDef.m_radius = GetRealWH(w);
		
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleDef;
		fixtureDef.density = (isStatic) ? 0.0f : 1.0f;
		fixtureDef.friction = 0.3;
		fixtureDef.restitution = 0.2;

		this->m_fixture = this->m_body->CreateFixture(&fixtureDef);
	}
};


#endif