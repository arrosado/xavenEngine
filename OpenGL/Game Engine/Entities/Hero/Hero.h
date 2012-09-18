#ifndef __HERO_H__
#define __HERO_H__

#include <Entities\IEntity.h>
#include <Entities\Hero\HeroPhysics.h>
#include <Entities\Hero\HeroGraphics.h>

class Hero: public Entity {
public:
	Hero(float x, float y, float w, float h, b2World* world) { 
		this->name = "Player";
		this->width = w;
		this->height = h;
		this->IsReady = false;
		this->m_physics = new HeroPhysics(x, y, w, h, world, this);
		this->m_graphics = new HeroGraphics(this);
	}

	void HandleEvents() { 
		InputManager* i = InputManager::Instance();
		bool left = i->GetKeyState(100);
		bool right = i->GetKeyState(102);
		bool up = i->GetKeyState(101);
		bool down = i->GetKeyState(103);
		const int speed = 8;

		b2Body *body = this->m_physics->GetBody();
		
		if (left) {
			body->SetAwake(true);
			body->SetLinearVelocity(b2Vec2(-speed, this->velocity.y));
		}

		if (right) { 
			body->SetAwake(true);
			body->SetLinearVelocity(b2Vec2(speed, this->velocity.y));
		}

		if (up) {
			if (this->velocity.y > -1) { // Stops the player form sometimes jumpping higher than it is supposed to.

				b2Body* Hit1 = this->m_physics->GetBodyAtPoint(position.x, position.y - (height/2), true); /* Under */
				b2Body* Hit2 = this->m_physics->GetBodyAtPoint(position.x - (width/2), position.y - (height/2), true); /* Down-Left */
				b2Body* Hit3 = this->m_physics->GetBodyAtPoint(position.x + (width/2), position.y - (height/2), true); /* Down-Right */
				
				if (Hit1 != NULL && Hit1->GetUserData() != this) { 
					body->ApplyLinearImpulse(b2Vec2(0.0, 30.0), body->GetWorldCenter()); // Apply an inpulse to the player. (Make it jump)
				} else if (Hit2 != NULL && Hit2->GetUserData() != this || 
						   Hit3 != NULL && Hit3->GetUserData() != this) { 
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), body->GetWorldCenter());
				}
			}
		}
	
	}
};

#endif