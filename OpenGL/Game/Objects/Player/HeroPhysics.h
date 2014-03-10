#ifndef __HERO_PHYSICS_H__
#define __HERO_PHYSICS_H__

#include <Game/Objects/IEntity.h>
#include <Game/Physics/PhysicsCommon.h>
#include <Game/Physics/IPhysicsComponent.h>

class HeroPhysics: public IPhysicsComponent { 
public:

	HeroPhysics(float x, float y, float w, float h, b2World* world, IEntity* entity) { 
		this->m_entity = entity;
		this->m_world = world;

		b2BodyDef bodyDef;
		bodyDef.bullet = true;
		bodyDef.position.Set(GetRealWH(x), GetRealWH(y));
		bodyDef.type = b2_dynamicBody;
		bodyDef.allowSleep = true;
		bodyDef.userData = entity;

		this->m_body = world->CreateBody(&bodyDef);

		b2PolygonShape shapeDef;
		shapeDef.SetAsBox(GetRealWH(w), GetRealWH(h));

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shapeDef;
		fixtureDef.density = 2.0f;
		fixtureDef.friction = 0.1;
		fixtureDef.restitution = 0.1;

		this->m_fixture = m_body->CreateFixture(&fixtureDef);
	}

	void Update(float delta) {
		IPhysicsComponent::Update(delta);
		this->m_body->GetSweep()->a = 0;
	}
};

#endif