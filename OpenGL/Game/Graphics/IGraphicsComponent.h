#ifndef __IGRAPHICS_COMPONENT_H__
#define __IGRAPHICS_COMPONENT_H__

#include <Game/Physics/PhysicsCommon.h> // I shouldn't have to include this one but im using it because of the PTM_RATIO.
#include <Game/Graphics/Animation/Animation.h>
#include <Game/Objects/IEntity.h>
#include <map>

struct IGraphicsComponent { 
protected:
	IEntity *m_entity;
	std::map<EntityState, Animation*> m_animations;
	Animation* m_currentAnimation;
public:

	IGraphicsComponent() { 
		this->m_currentAnimation = NULL;
		this->m_entity = NULL;
		this->m_animations.clear();
	}

	virtual void Update(float delta) {
		if (this->m_currentAnimation)
			this->m_currentAnimation->Update(delta);
	}

	void Render(){
		if (this->m_currentAnimation)
			this->m_currentAnimation->Render(this->m_entity->position, 1.0f/PTM_RATIO, this->m_entity->rotation);
	}

	bool CurrentAnimationFinished() { 
		return (this->m_currentAnimation->GetAnimationState() == AnimationState::AS_STOPPED);
	}
protected:
	void SwapCurrentAnimation(Animation *newAnimation, AnimationType type) {
		if (this->m_currentAnimation != newAnimation)
			(this->m_currentAnimation = newAnimation)->Reset(type);
	}
};

#endif /* __IGRAPHICS_COMPONENT_H__ */
