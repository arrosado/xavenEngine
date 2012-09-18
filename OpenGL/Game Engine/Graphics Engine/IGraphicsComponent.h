#ifndef __IGRAPHICS_COMPONENT_H__
#define __IGRAPHICS_COMPONENT_H__

#include <Physics Engine\PhysicsCommon.h> // I shouldn't have to include this one but im using it because of the PTM_RATIO.
#include <Graphics Engine\Animation\Animation.h>
#include <Entities\IEntity.h>
#include <map>

struct IGraphicsComponent { 
protected:
	IEntity *m_entity;
	std::map<std::string, Animation*> m_animations;
	Animation* m_currentAnimation;
public:
	virtual void Update(float delta) {
		this->m_currentAnimation->Update(delta);
	}

	void Render(){
		this->m_currentAnimation->Render(this->m_entity->position, 1.0f/PTM_RATIO, this->m_entity->rotation);
	}
protected:
	void SwapCurrentAnimation(Animation *newAnimation, AnimationType type) {
		if (this->m_currentAnimation != newAnimation)
			(this->m_currentAnimation = newAnimation)->Reset(type);
	}
};

#endif /* __IGRAPHICS_COMPONENT_H__ */
