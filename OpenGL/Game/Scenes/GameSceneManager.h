/*
 *  GameSceneManager.h
 *  Chupaqui
 *
 *  Created by Ariel on 11/19/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */
#ifndef __GAME_SCENE_MANAGER_H__
#define __GAME_SCENE_MANAGER_H__

#include <Game/IComponent.h>
#include <Game/Scenes/States/IGameScene.h>
#include <vector>

class GameSceneManager : public IComponent {
public:
	static GameSceneManager* Instance() {
		return &m_SceneManagerInstance;
	}
	
	void Init(int argc, ...);
	
	void ChangeScene(IGameScene* scene);
	void PushScene(IGameScene* scene);
	void PopScene();
	
	void HandleEvents();
	void Update(double delta);
	void Draw();
	
	bool Running() { return m_running; }

	void Cleanup();
	void Quit() { m_running = false; }
	
private:
	static GameSceneManager m_SceneManagerInstance;
	// The stack of scenes.
	std::vector<IGameScene*> m_scenes;
	// The running flag.
	bool m_running;
protected:
	GameSceneManager() {};
};

#endif /* __GAME_SCENE_MANAGER_H__ */