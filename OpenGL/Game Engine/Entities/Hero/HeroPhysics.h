#ifndef __HERO_PHYSICS_H__
#define __HERO_PHYSICS_H__

#include <Entities\IEntity.h>
#include <Physics Engine\PhysicsCommon.h>
#include <Physics Engine\IPhysicsComponent.h>

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

		b2PolygonShape boxDef;
		boxDef.SetAsBox(GetRealWH(w), GetRealWH(h));

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxDef;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.5;
		fixtureDef.restitution = 0.2;

		this->m_fixture = m_body->CreateFixture(&fixtureDef);
	}

	void Update(float delta) {
		IPhysicsComponent::Update(delta);
		this->m_body->GetSweep()->a = 0;
	}
};

#endif