/*
 *  GameLevel.h
 *  ARR Game Engine Core
 *
 *  Created by Ariel Rosado on 2/20/11.
 *  Copyright 2011 Xaven. All rights reserved.
 *
 */

#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include <stdio.h>
#include <Game State Engine\States\IGameScene.h>

class GameLevel : public IGameScene {
public:
	static IGameScene* Instance() {
		return (IGameScene*)&m_GameLevelState;
	}
	
	void Init();
	void Cleanup();
	
	void Pause();
	void Resume();
	
	void HandleEvents();
	void Update(float delta);
	void Draw();

protected:
	GameLevel() { }
	
private:
	static GameLevel m_GameLevelState;

};

#endif /* __GAME_LEVEL_H__ */
