#ifndef __BOX_PHYSICS_H__
#define __BOX_PHYSICS_H__

#include <Game/Physics/PhysicsCommon.h>
#include <Game/Physics/IPhysicsComponent.h>

class BoxPhysics: public IPhysicsComponent {
public:
	BoxPhysics(float x, float y, float w, float h, bool isStatic, b2World* world, IEntity* entity) {
		this->m_entity = entity;
		this->m_world = world;

		b2BodyDef bodyDef;
		bodyDef.bullet = true;
		bodyDef.position.Set(GetRealWH(x), GetRealWH(y));
		bodyDef.type = (isStatic) ? b2_staticBody : b2_dynamicBody;
		bodyDef.userData = entity;

		this->m_body = world->CreateBody(&bodyDef);

		b2PolygonShape boxDef;
		boxDef.SetAsBox(GetRealWH(w), GetRealWH(h));
		
		b2FixtureDef fixtureDef;
		fixtureDef.density = (isStatic) ? 0.0f : 1.0f;
		fixtureDef.shape = &boxDef;
		fixtureDef.friction = 0.3;
		fixtureDef.restitution = 0.4;

		this->m_fixture = m_body->CreateFixture(&fixtureDef);
	}
};

#endif 