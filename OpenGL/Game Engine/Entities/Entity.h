#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <Entities\IEntity.h>
#include <Sound Engine\IAudioComponent.h>
#include <Physics Engine\IPhysicsComponent.h>
#include <Graphics Engine\IGraphicsComponent.h>

class Entity : public IEntity {
public:
	Entity() {
		this->m_physics = NULL;
		this->m_graphics = NULL;
		this->m_audio = NULL;
	}

	~Entity() {
		if (m_physics)
			delete m_physics;
		if (m_graphics)
			delete m_graphics;
		if (m_audio)
			delete m_audio;
	}

	virtual void HandleEvents() = 0;

	virtual void Update(float delta) {
		/* Should update before synchronizing 
		 * components because this update changes
		 * the property values in case any conversion
		 * is needed. For example the physics update
		 * applies the PTM_Ratio to all the fields
		 * it needs.
		 */
		if (m_physics) m_physics->Update(delta);
		if (m_graphics) m_graphics->Update(delta);
		if (m_audio) m_audio->Update(delta);
	}

	virtual void Render() {

		if (m_graphics) m_graphics->Render();
		if (m_audio) m_audio->Play();

	}
};

#endif /* __ENTITY_H__ */