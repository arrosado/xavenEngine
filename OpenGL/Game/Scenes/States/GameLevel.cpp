/*
 *  State_GameLevel.cpp
 *  ARR Game Engine Core
 *
 *  Created by Ariel Rosado on 2/20/11.
 *  Copyright 2011 Xaven. All rights reserved.
 *
 */

#include <Game/Scenes/States/GameLevel.h>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Input/InputManager.h>
#include <Game/Physics/PhysicsCommon.h>
#include <Game/Objects/Player/Hero.h>
#include <Game/Objects/Box/Box.h>
#include <Game/Objects/Circle/Circle.h>
#include <Game/Graphics/Console/Console.h>
#include <Game/Graphics/Grid/Grid.h>
//#include <Game/Graphics/Tilemap/Tmx.h>

GameLevel GameLevel::m_GameLevelState;
DebugDraw GameLevel::IGameScene::m_debugDraw(PTM_RATIO);

#define HERO "Hero"

void GameLevel::Init() {
	//m_map = new Tmx::Map();
	//m_map->ParseFile(RESOURCES_FOLDER "desert.tmx");
	
	b2Vec2 gravity(0.0f, -20.0);
	m_world = new b2World(gravity);
	
	uint32 flags = 0;
	flags += 1	* b2Draw::e_shapeBit;
	flags += 1	* b2Draw::e_jointBit;
	flags += 1	* b2Draw::e_aabbBit;
	flags += 1	* b2Draw::e_pairBit;
	flags += 1	* b2Draw::e_centerOfMassBit;
	m_debugDraw.SetFlags(flags);
	
	m_world->SetDebugDraw(&m_debugDraw);
	this->m_entities[HERO] = new Hero(200, 300 , 20, 20, m_world);
	this->m_entities["Floor1"] = new Box(0, -50, 4600, 50, true, m_world);
	this->m_entities["Floor2"] = new Box(-200, 0, 200, 50, true, m_world);
	this->m_entities["Circle"] = new Circle(-50, 220, 50, false, m_world);

	b2Vec2 stackPoint(10.0f, 20.0f);

	/* following code make some stack of blocks */
	for (int i = 1; i < 10; i++)
		for (int k = 1; k < 5; k++) {
			string name = "box";
	
			char buff[3];
			sprintf(buff, "%d%d", i, k);
			name.append(buff);

			int w = 20; // Box Width
			int h = 20; // Box height
			int s = 10; // Space between boxes

			b2Vec2 p;

			p.x = ((w * 2 + s) * i) + stackPoint.x;
			p.y = ((h * 2 + s) * k) + stackPoint.y;

			this->m_entities[name] = new Box(p.x, p.y, w, h, false, m_world);
		}
}

void GameLevel::Cleanup() {
	map<string, IEntity*>::iterator it;
	for(it = this->m_entities.begin(); it != this->m_entities.end(); it++)
		delete it->second;

	this->m_entities.clear();

	delete this->m_world;
}

void GameLevel::Pause() {}

void GameLevel::Resume() {}

void GameLevel::HandleEvents() {
	/* This gives the oportunity to all entities to read input and change state accordingly. */
	map<string, IEntity*>::iterator it;
	for(it = this->m_entities.begin(); it != this->m_entities.end(); it++)
		it->second->HandleEvents();
}
	
static float accumDeltas = 0.0f;
static float frames = 0.0f;
static float frameRate = 0.0f;

void GameLevel::Update(float delta) {

	frames++;
	accumDeltas += delta;

	if (accumDeltas > 0.1f) { 
		frameRate = frames/accumDeltas;
		frames = 0.0f;
		accumDeltas = 0.0f;
	}

	/* This syncronizes the Physics with the Audio and Graphics. */
	map<string, IEntity*>::iterator it;
	for(it = this->m_entities.begin(); it != this->m_entities.end(); it++)
		it->second->Update(delta);

	if (this->m_entities[HERO]->IsReady == false)
		return;

	/* This code runs the Box2D World. */
	const int32 iterations			= 2;
	const int32 velocityIterations	= 8;
	const int32 positionIterations	= 3;

	for(int i = 0; i < iterations; i++)
		this->m_world->Step(1.0f/60.0f, velocityIterations, positionIterations);	
}

void GameLevel::Draw() {
	Console* c = Console::Instance();
	
	c->Write("FPS = %.1f", frameRate);
	c->Write("Player %s", m_entities[HERO]->IsOnGround ? "Is On Ground" : "Is On Air");
	
	Grid* grid = new Grid(10, 10, 10);
	grid->Draw(GridType::XGrid);
	delete grid;

	//if (!m_map->HasError())
	//	m_map->Render();

	/* This syncronizes the Physics with the Audio and Graphics. */
	map<string, IEntity*>::iterator it;
	for(it = this->m_entities.begin(); it != this->m_entities.end(); it++)
		it->second->Render();
	
	this->m_world->DrawDebugData();
}

