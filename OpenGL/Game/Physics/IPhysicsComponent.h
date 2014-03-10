#ifndef __IPHYSICS_COMPONENT_H__
#define	__IPHYSICS_COMPONENT_H__

/*
 *  IPhysicsComponent.h
 *  ARR Game Engine Core
 *
 *  Created by Ariel Rosado on 2/17/11.
 *  Copyright 2011 Xaven. All rights reserved.
 *
 */
#include <Game/Physics/PhysicsCommon.h>
#include <Game/Objects/IEntity.h>

class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point, bool includeStatic) 
	{
		m_point = point;
		m_fixture = NULL;
		m_includeStatic = includeStatic;
	}

	bool ReportFixture(b2Fixture* fixture) 
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody || m_includeStatic)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside) 
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
	bool m_includeStatic;
};

struct IPhysicsComponent { 
protected:
	b2World *m_world;
	b2Body *m_body;
	b2Fixture *m_fixture;
	IEntity *m_entity;

public:

	inline b2Body* GetBody() const { return m_body; }

	b2Body* GetBodyAtPoint(float x, float y, bool includeStatic) { 

		b2Vec2 p(x, y);

		b2AABB aabb;
		b2Vec2 d(0.5f, 0.5f);
		aabb.lowerBound = p - d;
		aabb.upperBound = p + d;

		QueryCallback callback(p, includeStatic);
	
		m_world->QueryAABB(&callback, aabb);

		if (callback.m_fixture) { 
			return callback.m_fixture->GetBody();
		}

		return NULL;
	}

	virtual void Update(float delta) {
		float a = this->m_body->GetAngle();
		b2Vec2 p = this->m_body->GetPosition();
		b2Vec2 v = this->m_body->GetLinearVelocity();
		IEntity* e = ((IEntity*)this->m_body->GetUserData());
		
		e->position = Vector2DfMake(p.x, p.y);//Vector2DfMake(GetUnRealWH(p.x), GetUnRealWH(p.y));
		e->velocity = Vector2DfMake(v.x, v.y);
		e->rotation = a * (180.0f/b2_pi);
	}
};

#endif /* __IPHYSICS_COMPONENT_H__ */