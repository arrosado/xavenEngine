/*
 *  GameSceneManager.cpp
 *  Chupaqui
 *
 *  Created by Ariel on 11/19/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */

#include <Game State Engine\GameSceneManager.h>
#include <Game State Engine\InputManager.h>
#include <Game State Engine\States\IGameScene.h>
#include <Game State Engine\States\GameLevel.h>

GameSceneManager GameSceneManager::m_SceneManagerInstance;

void GameSceneManager::Init(int argc, ...) {
	this->ChangeScene(GameLevel::Instance());
}

void GameSceneManager::Cleanup() {
	// Cleanup all the states.
	while (!this->m_scenes.empty()) {
		this->m_scenes.back()->Cleanup();
		this->m_scenes.pop_back();
	}
}

void GameSceneManager::ChangeScene(IGameScene* scene) {
	// Cleanup the current scene.
	if (!this->m_scenes.empty()) {
		this->m_scenes.back()->Cleanup();
		this->m_scenes.pop_back();
	}
	
	// store and init the new state.
	this->m_scenes.push_back(scene);
	this->m_scenes.back()->Init();
}

void GameSceneManager::PushScene(IGameScene* scene) {
	// Pause current scene.
	if (!this->m_scenes.empty()) {
		this->m_scenes.back()->Pause();
	}
	
	// Store and init the new scene.
	this->m_scenes.push_back(scene);
	this->m_scenes.back()->Init();
}

void GameSceneManager::PopScene() {
	// Cleanup the current scene.
	if (!this->m_scenes.empty()) {
		this->m_scenes.back()->Cleanup();
		this->m_scenes.pop_back();
	}
	
	// Resume previous scene.
	if (!this->m_scenes.empty()) {
		this->m_scenes.back()->Resume();
	}
}

void GameSceneManager::HandleEvents() {
	InputManager* i = InputManager::Instance();
	bool reset = i->GetKeyState('r');

	if (reset) { 
		this->ChangeScene(GameLevel::Instance());
	}

	// Only the current scene should handle player events.
	this->m_scenes.back()->HandleEvents();
}

void GameSceneManager::Update(double delta) {
	// If the current scene is transparent, 
	// update the last scene (if any).
	if (this->m_scenes.back()->IsTransparentState())
		if (this->m_scenes.size() > 1)
			this->m_scenes[this->m_scenes.size()-2]->Update(delta);
	
	// Update the current game scene.
	this->m_scenes.back()->Update(delta);
}

void GameSceneManager::Draw() {	
	// If the current scene is transparent, 
	// draw the last scene (if any).
	if (this->m_scenes.back()->IsTransparentState())
		if(this->m_scenes.size() > 1) 
			this->m_scenes[this->m_scenes.size()-2]->Draw();
	
	// Draw the current game scene.
	this->m_scenes.back()->Draw();
}
