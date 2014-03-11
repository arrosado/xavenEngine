#ifndef __HERO_H__
#define __HERO_H__

#include <Game/Objects/GameObject.h>
#include <Game/Objects/Player/HeroPhysics.h>
#include <Game/Objects/Player/HeroGraphics.h>

class Hero: public GameObject {
public:
    
	Hero(float x, float y, float w, float h, b2World* world) { 
		this->name = "Player";
		this->IsReady = false;
		this->IsOnGround = false;
		this->m_physics = new HeroPhysics(x, y, w, h, world, this);
		this->m_graphics = new HeroGraphics(this);
	}

	void HandleEvents() { 

		if (IsReady == false)
			return;
        
        this->m_physics->HandleEvents();
	}
};

#endif