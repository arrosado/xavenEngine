#ifndef __IENTITY_H__
#define __IENTITY_H__

#include <Game/Graphics/GraphicsCommon.h>
#include <string>

struct IPhysicsComponent;
struct IGraphicsComponent;
struct IAudioComponent;

enum EntityState {
	RUNNING = 0,
	STOPPING,
	SLIDING,
	SLIDING_TO_IDLE,
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
	GRABBING_START,
	GRABBING,
	GRABBING_END,
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
    virtual ~IEntity() {}

protected:
	IPhysicsComponent  *m_physics;
	IGraphicsComponent *m_graphics;
	IAudioComponent    *m_audio;
};

#endif