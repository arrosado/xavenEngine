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

struct IGameObject {
public: 
	std::string name;
	bool IsReady;
	bool IsOnGround;
	EntityDirection direction;
	EntityState state;
    Vector3Df translation;
    Vector3Df position;
    Vector3Df rotation;

	virtual void HandleEvents() = 0;
	virtual void Update(float delta) = 0;
	virtual void Render() = 0;
    virtual ~IGameObject() {}

protected:
	IPhysicsComponent  *m_physics;
	IGraphicsComponent *m_graphics;
	IAudioComponent    *m_audio;
};

#endif