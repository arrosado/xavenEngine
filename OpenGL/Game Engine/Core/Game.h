//
//  Game.h
//  ARR Game Engine Core
//
//  Created by Ariel Rosado on 3/15/11.
//  Copyright 2011 Xaven. All rights reserved.
//

#ifndef __GAME_H__
#define __GAME_H__

#include <IGame.h>
#include <Game State Engine\InputManager.h>
#include <Sound Engine\SoundManager.h>
#include <Graphics Engine\GraphicsManager.h>
#include <Game State Engine\GameSceneManager.h>

class Game : public IGameEngine {
private:
	void SaveState();
	void LoadState();
	
	InputManager* GetInput();
	SoundManager* GetSound();
	GraphicsManager* GetGraphics();
	GameSceneManager* GetScenes();

public:
    Game();
	~Game();
	void Initialize( int width, int height );
	
	void Render();
	void Update(double delta);
	void UpdateInput(int key, bool keyState, int button, bool buttonState, int x, int y);
	void ResizeView(int w, int h);
	void Pause();
	void Start();
	
	void Exit( bool andSaveCurrentState );
};

IGameEngine* CreateGame() {
	return new Game();
}

IGameEngine* DestoryGame(IGameEngine* game) {
	delete game;
	return NULL;
}

#endif /* __GAME_H__ */