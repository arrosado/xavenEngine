#ifndef __IENTITY_H__
#define __IENTITY_H__

#include <Graphics Engine\GraphicsCommon.h>
#include <string>

struct IPhysicsComponent;
struct IGraphicsComponent;
struct IAudioComponent;

enum EntityState {
	RUNNING,
	STOPPING,
	SLIDING,
	IDLE,
	JUMPING,
	STARTING_TO_FALL,
	FALLING,
	LANDING,
	FLYING,
	TAKING_DAMAGE,
	DYING,
	SPAWNING,
	CLIMBING_UP,
	CLIMBING_FALL,
	CLIMBING_STOPPING,
	CLIMBING_IDLE,
	EntityStatesCount
};

enum EntityDirection { 
	MOVING_LEFT,
	MOVING_RIGHT,
	MOVING_UP,
	MOVING_DOWN,
	EntityDirectionsCount
};

struct IEntity {
public: 
	std::string name;
	float width;
	float height;
	float rotation;
	bool IsReady;
	bool IsOnGround;
	Vector2Df position;
	Vector2Df velocity;
	EntityDirection direction;
	EntityState state;

	virtual void HandleEvents() = 0;
	virtual void Update(float delta) = 0;
	virtual void Render() = 0;

protected:
	IPhysicsComponent  *m_physics;
	IGraphicsComponent *m_graphics;
	IAudioComponent    *m_audio;
};

#endif