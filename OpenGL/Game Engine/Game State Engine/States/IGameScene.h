#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <Graphics Engine\GraphicsCommon.h>
#include <DebugDraw.h>
#include <Entities\Entity.h>
#include <string>
#include <vector>
#include <map>

/* 
 * Base class for which we can inherit classes for 
 * each scene needed in the game - intro scene, 
 * menu scene, pause scene, level scene, overworld scene,
 * settings scene, etc.
 */

typedef enum {
	kGameScene_Running,
	kGameScene_Idle,
	kGameScene_Transitioning_In,
	kGameScene_Transitioning_Out,
	kGameScene_StateCount
} GameSceneState;

struct IGameScene {
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;
	
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	
	virtual void HandleEvents() = 0;
	virtual void Update(float delta) = 0;
	virtual void Draw() = 0;
	
	bool IsTransparentState() { return this->m_isTransparent; }
	
protected:
	/* 
	 * This variable tells the Scene Manager
	 * that the GameScene that is under the 
	 * current scene should be updated and drawed.
	 */
	Vector2Df m_screenBounds;
	float m_transitionSpeed;
	GameSceneState m_state;
	std::string m_nextSceneKey;
	float m_sceneAlpha;
	bool m_isTransparent;

	b2World* m_world;
	static DebugDraw m_debugDraw;
	std::map<std::string, IEntity*> m_entities;

};

#endif