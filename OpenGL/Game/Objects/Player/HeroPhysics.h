#ifndef __HERO_PHYSICS_H__
#define __HERO_PHYSICS_H__

#include <Game/Objects/GameObject.h>
#include <Game/Objects/IGameObject.h>
#include <Game/Physics/PhysicsCommon.h>
#include <Game/Physics/IPhysicsComponent.h>

#define MAX_SPEED 1

class HeroPhysics: public IPhysicsComponent { 
public:
    Vector2Df velocity;
    enum Buttons {
        left = 100,
        right = 102,
        up = 101,
        down = 103
    };

	HeroPhysics(float x, float y, float w, float h, b2World* world, IGameObject* entity) {
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
    
    void HandleEvents()
    {
        InputManager* i = InputManager::Instance();
		bool left = i->GetKeyState(Buttons::left);
		bool right = i->GetKeyState(Buttons::right);
		bool up = i->GetKeyState(Buttons::up);
		bool down = i->GetKeyState(Buttons::down);
        
		//const float flyingThreshold  = 2.0f;
		const float runningThreshold = 2.0f;
		const float fallingThreshold = 2.0f;
		const float currentSpeed = 20.0f;
        
		b2Body *body = this->GetBody();
        
		/*
         b2Body* Hit1 = m_physics->GetBodyAtPoint(position.x, position.y - (height/2), true); // Under
         b2Body* Hit2 = m_physics->GetBodyAtPoint(position.x - (width/2), position.y - (height/2), true); // Down-Left
         b2Body* Hit3 = m_physics->GetBodyAtPoint(position.x + (width/2), position.y - (height/2), true); // Down-Right
         
         IsOnGround = (Hit1 != NULL && Hit1->GetUserData() != this ||
         Hit2 != NULL && Hit2->GetUserData() != this ||
         Hit3 != NULL && Hit3->GetUserData() != this) ? true : false;
         */
        
        
		m_entity->IsOnGround = (body->GetContactList()) ? true : false;
        
		if ((left || right) && m_entity->IsOnGround)
			m_entity->state = EntityState::RUNNING;
        
		if (left || right) {
            
			body->SetAwake(true);
            
			if (fabs(velocity.x) < MAX_SPEED)
                //body->ApplyForce(b2Vec2(fabs(velocity.x) + ((right) ? + 1 : -1) * currentSpeed, body->GetLocalCenter().y), body->GetLocalCenter());
				body->SetLinearVelocity(b2Vec2(fabs(velocity.x) + ((right) ? + 1 : - 1) * currentSpeed, velocity.y));
		}
        
		if (up &&
			velocity.y > -1 && // Stops the player form sometimes jumpping higher than it is supposed to.
			m_entity->IsOnGround) {
			m_entity->state = EntityState::JUMPING;
            body->SetAwake(true);
			body->ApplyLinearImpulse(b2Vec2(0.0, 10.0f), body->GetWorldCenter()); // Apply an impulse to the player. (Make it jump)
		} else if (up &&
				   m_entity->state != EntityState::JUMPING &&
				   m_entity->IsOnGround == false) {
			m_entity->state = EntityState::FLYING;
			body->SetLinearVelocity(b2Vec2(velocity.x, 3));
		}
        
		if (m_entity->IsReady) {
			if (m_entity->IsOnGround) {
                
				if (!(m_entity->state == EntityState::JUMPING ||
					  m_entity->state == EntityState::FLYING)) {
                    
					// Check if idle.
					if (velocity.x == 0 &&
						velocity.y == 0 &&
                        (m_entity->state != EntityState::LANDING))
						m_entity->state = EntityState::IDLE;
					// Check if running.
					else if (velocity.x != 0 &&
                             (velocity.x > runningThreshold || velocity.x < -runningThreshold))
						m_entity->state = EntityState::RUNNING;
					// Check if sliding.
					else if (velocity.x <  runningThreshold &&
							 velocity.x > -runningThreshold &&
							 velocity.x >  (runningThreshold/2) &&
							 velocity.x < -(runningThreshold/2) &&
							 m_entity->state == EntityState::RUNNING)
						m_entity->state = EntityState::SLIDING;
					// Check if climbing.
					// Check if stopping.
					else if (m_entity->state == EntityState::SLIDING &&
							 velocity.x < (runningThreshold/2) &&
							 velocity.x > -(runningThreshold/2))
						m_entity->state = EntityState::SLIDING_TO_IDLE;
					// Check if landing.
					else if (m_entity->state == EntityState::FALLING ||
							 m_entity->state == EntityState::STARTING_TO_FALL)
						m_entity->state = EntityState::LANDING;
					
				}
				
			} else if (!m_entity->IsOnGround) {
				if (velocity.y < fallingThreshold &&
					velocity.y > -fallingThreshold)
					m_entity->state = EntityState::STARTING_TO_FALL;
				else if (velocity.y < 0 &&
						 velocity.y < -fallingThreshold)
					m_entity->state = EntityState::FALLING;
			}
        }
        
		if (velocity.x != 0) {
			if (velocity.x > runningThreshold) {
				m_entity->direction = MOVING_RIGHT;
			} else if (velocity.x < -runningThreshold) {
				m_entity->direction = MOVING_LEFT;
            }
        }
    }

	void Update(float delta) {
        
		IPhysicsComponent::Update(delta);
		this->m_body->GetSweep()->a = 0;
        b2Vec2 v = this->m_body->GetLinearVelocity();
        this->velocity = Vector2DfMake(v.x, v.y);
	}
};

#endif