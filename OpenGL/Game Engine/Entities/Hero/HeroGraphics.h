#ifndef __HERO_GRAPHICS_H__
#define __HERO_GRAPHICS_H__

#include <Graphics Engine\Animation\Animation.h>
#include <Entities\IEntity.h>
#include <Graphics Engine\IGraphicsComponent.h>

#define IMAGES_FOLDER "..\\OpenGL\\Resources\\"

class HeroGraphics : public IGraphicsComponent {
public:
	HeroGraphics(IEntity * entity) { 
		
		this->m_entity = entity;
		this->m_animations["Run"]				= new Animation(IMAGES_FOLDER "run.png", 170, 170);
		this->m_animations["RunToSlide"]		= new Animation(IMAGES_FOLDER "runToSlide.png", 170, 170);
		this->m_animations["Slide"]				= new Animation(IMAGES_FOLDER "slide.png", 170, 170);
		this->m_animations["SlideToIdle"]		= new Animation(IMAGES_FOLDER "slideToIdle.png", 170, 170);
		this->m_animations["Idle"]				= new Animation(IMAGES_FOLDER "idle.png", 170, 170);
		this->m_animations["Fly"]				= new Animation(IMAGES_FOLDER "fly.png", 170, 170);
		this->m_animations["Jump"]				= new Animation(IMAGES_FOLDER "jump.png", 170, 170);
		this->m_animations["JumptoFall"]		= new Animation(IMAGES_FOLDER "jumpToFall.png", 170, 170);
		this->m_animations["Fall"]				= new Animation(IMAGES_FOLDER "fall.png", 170, 170);
		this->m_animations["Land"]				= new Animation(IMAGES_FOLDER "land.png", 170, 170);
		this->m_animations["Damage"]			= new Animation(IMAGES_FOLDER "damage.png", 170, 170);
		this->m_animations["Die"]				= new Animation(IMAGES_FOLDER "die.png", 170, 170);
		this->m_animations["Spawn"]				= new Animation(IMAGES_FOLDER "spawn.png", 170, 170);
		this->m_animations["Climb"]				= new Animation(IMAGES_FOLDER "climb.png", 170, 170);
		this->m_animations["SlideDown"]			= new Animation(IMAGES_FOLDER "slideDown.png", 170, 170);
		this->m_animations["ClimbToClimbIdle"]	= new Animation(IMAGES_FOLDER "climbToClimbIdle.png", 170, 170);
		this->m_animations["ClimbIdle"]			= new Animation(IMAGES_FOLDER "climbIdle.png", 170, 170);
		
		this->SwapCurrentAnimation(this->m_animations["Spawn"], AnimationType::kAnimationType_Once);
	}

	~HeroGraphics() {
		map<string, Animation*>::iterator it;
		for (it = this->m_animations.begin(); it != this->m_animations.end(); it++) {
			delete it->second;
		}
	}

	void Update(float delta) {

		float flyingThreshold, runningThreshold, fallingThreshold, currentSpeed;
		flyingThreshold = 0.35;
		runningThreshold = 0.2;
		fallingThreshold = -1.0f;
		currentSpeed = 20.0f;

		IGraphicsComponent::Update(delta);

		if ((this->m_currentAnimation == this->m_animations["Spawn"]) && 
			(this->m_currentAnimation->state == AnimationState::kAnimationState_Stopped)) {
			this->m_entity->IsReady = true;
			return;
		}

		IEntity* e = this->m_entity;

		if (e->velocity.x != 0 &&
			fabs(e->velocity.y) < flyingThreshold &&
			fabs(e->velocity.x) > runningThreshold &&
			e->IsOnGround) {
			this->SwapCurrentAnimation(m_animations["Run"], AnimationType::kAnimationType_Repeating);
		} else if (e->velocity.y != 0 && 
			fabs(e->velocity.y) > flyingThreshold) {
			this->SwapCurrentAnimation(m_animations["Fly"], AnimationType::kAnimationType_Repeating);
		} else if (fabs(e->velocity.x) < runningThreshold &&
			fabs(e->velocity.y) < flyingThreshold && 
			e->IsOnGround == false &&
			e->velocity.y < 0 &&
			e->velocity.y < fallingThreshold) {
			this->SwapCurrentAnimation(m_animations["Fall"], AnimationType::kAnimationType_Repeating);
		} else if (fabs(e->velocity.x) < runningThreshold && 
			fabs(e->velocity.y) < flyingThreshold &&
			e->IsOnGround && 
			e->velocity.y < 0 && 
			e->velocity.y < fallingThreshold) {
			this->SwapCurrentAnimation(m_animations["Landing"], AnimationType::kAnimationType_Repeating);
		} else if (e->velocity.x != 0 &&
			fabs(e->velocity.x) < runningThreshold &&
			fabs(e->velocity.y) < flyingThreshold &&
			e->velocity.y > fallingThreshold &&
			e->IsOnGround && 
			e->velocity.x != 0) {
			this->SwapCurrentAnimation(m_animations["RunToSlide"], AnimationType::kAnimationType_Once);
		} else if (e->velocity.x == 0 && 
			fabs(e->velocity.y) < flyingThreshold && 
			e->velocity.y > fallingThreshold && 
			e->IsOnGround && 
			e->velocity.x != 0) {
			this->SwapCurrentAnimation(m_animations["Slide"], AnimationType::kAnimationType_Once);
		} else if (fabs(e->velocity.x) < runningThreshold &&
			fabs(e->velocity.y) < flyingThreshold && 
			e->velocity.x == 0 &&
			e->velocity.y == 0) {
			this->SwapCurrentAnimation(m_animations["Idle"], AnimationType::kAnimationType_Repeating);
		}
		if (this->m_entity->velocity.x != 0)
			if (this->m_entity->velocity.x > runningThreshold)
				e->direction = MOVING_RIGHT;
			else if (this->m_entity->velocity.x < -runningThreshold)
				e->direction = MOVING_LEFT;

		
		this->m_currentAnimation->FlipAnimation(false, false);

		if (this->m_entity->direction == MOVING_LEFT) 
			this->m_currentAnimation->FlipAnimation(false, true);

	}

};

#endif