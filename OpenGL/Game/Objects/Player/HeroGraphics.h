#ifndef __HERO_GRAPHICS_H__
#define __HERO_GRAPHICS_H__

#include <Game/Graphics/IGraphicsComponent.h>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/Animation/Animation.h>
#include <Game/Objects/IEntity.h>

class HeroGraphics : public IGraphicsComponent {
public:
	HeroGraphics(IEntity * entity) { 
		
		IGraphicsComponent::IGraphicsComponent();

		m_entity = entity;
		m_animations[EntityState::RUNNING]				= new Animation(RESOURCES_FOLDER "run.png", 170, 170);
		m_animations[EntityState::STOPPING]				= new Animation(RESOURCES_FOLDER "runToSlide.png", 170, 170);
		m_animations[EntityState::SLIDING]				= new Animation(RESOURCES_FOLDER "slide.png", 170, 170);
		m_animations[EntityState::SLIDING_TO_IDLE]		= new Animation(RESOURCES_FOLDER "slideToIdle.png", 170, 170);
		m_animations[EntityState::IDLE]					= new Animation(RESOURCES_FOLDER "idle.png", 170, 170);
		m_animations[EntityState::FLYING]				= new Animation(RESOURCES_FOLDER "fly.png", 170, 170);
		m_animations[EntityState::JUMPING]				= new Animation(RESOURCES_FOLDER "jump.png", 170, 170);
		m_animations[EntityState::STARTING_TO_FALL]		= new Animation(RESOURCES_FOLDER "jumpToFall.png", 170, 170);
		m_animations[EntityState::FALLING]				= new Animation(RESOURCES_FOLDER "fall.png", 170, 170);
		m_animations[EntityState::LANDING]				= new Animation(RESOURCES_FOLDER "land.png", 170, 170);
		m_animations[EntityState::TAKING_DAMAGE]		= new Animation(RESOURCES_FOLDER "damage.png", 170, 170);
		m_animations[EntityState::DYING]				= new Animation(RESOURCES_FOLDER "die.png", 170, 170);
		m_animations[EntityState::SPAWNING]				= new Animation(RESOURCES_FOLDER "spawn.png", 170, 170);
		m_animations[EntityState::CLIMBING_UP]			= new Animation(RESOURCES_FOLDER "climb.png", 170, 170);
		m_animations[EntityState::CLIMBING_FALL]		= new Animation(RESOURCES_FOLDER "slideDown.png", 170, 170);
		m_animations[EntityState::CLIMBING_STOPPING]	= new Animation(RESOURCES_FOLDER "climbToClimbIdle.png", 170, 170);
		m_animations[EntityState::CLIMBING_IDLE]		= new Animation(RESOURCES_FOLDER "climbIdle.png", 170, 170);

		m_animations[EntityState::LANDING]->SetAnimationFramesDelay(0.2f);
		
		m_entity->state = EntityState::SPAWNING;
	}

	~HeroGraphics() {
		map<EntityState, Animation*>::iterator it;
		for (it = this->m_animations.begin(); it != this->m_animations.end(); it++) 
			delete it->second;
	}

	void Update(float delta) {

		IGraphicsComponent::Update(delta);

		IEntity* e = this->m_entity;

		if (this->m_currentAnimation && this->m_currentAnimation == this->m_animations[EntityState::SPAWNING])
			e->IsReady = (m_currentAnimation->GetAnimationState() == AnimationState::AS_STOPPED) ? true : false;

		switch(e->state) {
		case EntityState::SLIDING:
		case EntityState::STOPPING:
		case EntityState::JUMPING:
		case EntityState::SPAWNING:
		case EntityState::LANDING:
		case EntityState::SLIDING_TO_IDLE:
				this->SwapCurrentAnimation(m_animations[e->state], AnimationType::ONCE);
			break;
		default:
				this->SwapCurrentAnimation(m_animations[e->state], AnimationType::REPEAT);
			break;
		}

		this->m_currentAnimation->FlipAnimation(false, false);

		if (e->direction == MOVING_LEFT) 
			this->m_currentAnimation->FlipAnimation(false, true);

	}

};

#endif