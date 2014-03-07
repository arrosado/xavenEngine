#ifndef __HERO_H__
#define __HERO_H__

#include <Game/Entities/Entity.h>
#include <Game/Entities/Hero/HeroPhysics.h>
#include <Game/Entities/Hero/HeroGraphics.h>

#define MAX_SPEED 1

class Hero: public Entity {
public:
    
    enum Buttons {
        left = 100,
        right = 102,
        up = 101,
        down = 103
    };
    
	Hero(float x, float y, float w, float h, b2World* world) { 
		this->name = "Player";
		this->width = w;
		this->height = h;
		this->IsReady = false;
		this->IsOnGround = false;
		this->m_physics = new HeroPhysics(x, y, w, h, world, this);
		this->m_graphics = new HeroGraphics(this);
	}

	void HandleEvents() { 

		if (IsReady == false)
			return;
		
		InputManager* i = InputManager::Instance();
		bool left = i->GetKeyState(Buttons::left);
		bool right = i->GetKeyState(Buttons::right);
		bool up = i->GetKeyState(Buttons::up);
		//bool down = i->GetKeyState(Buttons::down);

		//const float flyingThreshold  = 2.0f;
		const float runningThreshold = 2.0f;
		const float fallingThreshold = 2.0f;
		const float currentSpeed = 20.0f;

		b2Body *body = m_physics->GetBody();

		/*
		b2Body* Hit1 = m_physics->GetBodyAtPoint(position.x, position.y - (height/2), true); // Under 
		b2Body* Hit2 = m_physics->GetBodyAtPoint(position.x - (width/2), position.y - (height/2), true); // Down-Left 
		b2Body* Hit3 = m_physics->GetBodyAtPoint(position.x + (width/2), position.y - (height/2), true); // Down-Right 

		IsOnGround = (Hit1 != NULL && Hit1->GetUserData() != this ||
					  Hit2 != NULL && Hit2->GetUserData() != this || 
					  Hit3 != NULL && Hit3->GetUserData() != this) ? true : false;
		*/

        
		IsOnGround = (body->GetContactList()) ? true : false;

		if ((left || right) && IsOnGround)
			state = EntityState::RUNNING;

		if (left || right) {
            
			body->SetAwake(true);

			if (fabs(velocity.x) < MAX_SPEED)
                //body->ApplyForce(b2Vec2(fabs(velocity.x) + ((right) ? + 1 : -1) * currentSpeed, body->GetLocalCenter().y), body->GetLocalCenter());
				body->SetLinearVelocity(b2Vec2(fabs(velocity.x) + ((right) ? + 1 : - 1) * currentSpeed, velocity.y));
		}

		if (up &&
			velocity.y > -1 && // Stops the player form sometimes jumpping higher than it is supposed to.
			IsOnGround) {
			state = EntityState::JUMPING;
            body->SetAwake(true);
			body->ApplyLinearImpulse(b2Vec2(0.0, 10.0f), body->GetWorldCenter()); // Apply an impulse to the player. (Make it jump)
		} else if (up &&
				   state != EntityState::JUMPING &&
				   IsOnGround == false) {
			state = EntityState::FLYING;
			body->SetLinearVelocity(b2Vec2(velocity.x, 3));
		}

		if (IsReady) {
			if (IsOnGround) { 

				if (!(state == EntityState::JUMPING ||
					  state == EntityState::FLYING)) {

					// Check if idle.
					if (velocity.x == 0 &&
						velocity.y == 0 &&
						((state == EntityState::LANDING) ? this->m_graphics->CurrentAnimationFinished() : true))
						state = EntityState::IDLE;
					// Check if running.
					else if (velocity.x != 0 && 
							(velocity.x > runningThreshold || velocity.x < -runningThreshold))
						state = EntityState::RUNNING;
					// Check if sliding.
					else if (velocity.x <  runningThreshold && 
							 velocity.x > -runningThreshold &&
							 velocity.x >  (runningThreshold/2) &&
							 velocity.x < -(runningThreshold/2) &&
							 state == EntityState::RUNNING)
						state = EntityState::SLIDING;
					// Check if climbing.
					// Check if stopping.
					else if (state == EntityState::SLIDING && 
							 velocity.x < (runningThreshold/2) &&
							 velocity.x > -(runningThreshold/2))
						state = EntityState::SLIDING_TO_IDLE;
					// Check if landing.
					else if (state == EntityState::FALLING || 
							 state == EntityState::STARTING_TO_FALL)
						state = EntityState::LANDING;
					
				}
				
			} else if (!IsOnGround) {
				if (velocity.y < fallingThreshold && 
					velocity.y > -fallingThreshold)
					state = EntityState::STARTING_TO_FALL;
				else if (velocity.y < 0 && 
						 velocity.y < -fallingThreshold)
					state = EntityState::FALLING;
			}
        }
        
		if (velocity.x != 0) {
			if (velocity.x > runningThreshold) {
				direction = MOVING_RIGHT;
			} else if (velocity.x < -runningThreshold) {
				direction = MOVING_LEFT;
            }
        }
	}
};

#endif