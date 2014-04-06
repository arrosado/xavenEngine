//
//  main.cpp
//  OpenGL
//
//  Created by Ariel Rosado on 8/17/12.
//  Copyright (c) 2012 Ariel Rosado. All rights reserved.
//
// main.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include "stdafx.h"
#include <gl/glut.h>
#else
#include <glut.h>
#endif
#include <Game/Graphics/Console/Console.h>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/Grid/Grid.h>
#include <Game/Graphics/Texture2D/Texture2D.h>
#include <Game/GameObject.h>
#include <Game/IGameObject.h>
#include <Game/IGameObjectComponent.h>
#include <Game/SpriteRendererComponent.h>
#include <Game/Input/InputManager.h>
#include <Game/GameCamera.h>


#define WIDTH 800
#define HEIGHT 600

#define UPDATE_INTERVAL .25

#define CAMERA_TYPE_3D

GameObject * player;
GameCamera * camera;

/* Function Prototypes */
void HandleResize(int w, int h);
void HandleKeyPress(unsigned char key, int x, int y);
void HandleKeyRelease(unsigned char key, int x, int y);
void HandleSpecialKeyPress(int key, int x, int y);
void HandleSpecialKeyRelease(int key, int x, int y);
void HandleMouse(int button, int state, int x, int y);
void HandleMousePassive(int x, int y);
void HandleMouseMotion(int x, int y);
void Update(int value);
void Draw();


void Draw(void)
{
    camera->ClearScreen();
	
    // Do Rendering here! [Start]
    
    Console::Instance()->Write("Camera position x:%f y:%f z:%f", camera->position.x, camera->position.y, camera->position.z);
	Console::Instance()->Write("Camera rotation x:%f y:%f z:%f", camera->rotation.x, camera->rotation.y, camera->rotation.z);
    Console::Instance()->Write("Player position x:%f y:%f z:%f", player->position.x, player->position.y, player->position.z);
    Console::Instance()->Write("Player rotation x:%f y:%f z:%f", player->rotation.x, player->rotation.y, player->rotation.z);
    Console::Instance()->Write("Player is %s", player->centered ? "Centered" : "Not Centered");
    Console::Instance()->Write("Player scale w:%f h:%f d:%f", player->scale.x, player->scale.y, player->scale.z);
    

#ifdef CAMERA_TYPE_3D
    glEnable(GL_DEPTH_TEST);
#endif
    
	Grid *grid = new Grid(10, 10, 1, Color4fMake(0.0f, 0.0f, 0.0f, 255.0f));
	grid->SetColor(Color4fRed);
	grid->Draw(XGrid);
	grid->SetColor(Color4fGreen);
	grid->Draw(YGrid);
	grid->SetColor(Color4fBlue);
	grid->Draw(ZGrid);
	delete grid;
    
    player->Draw();
    
	Console::Instance()->Draw();

#ifdef CAMERA_TYPE_3D
    glDisable(GL_DEPTH_TEST);
#endif
    
    glFlush();
    
    glutSwapBuffers();
}


void DoStuff() {
    InputManager *i = InputManager::Instance();
    
    bool movePlayer = i->GetKeyState('m') && i->GetKeyState('p');
    bool rotatePlayer = i->GetKeyState('r') && i->GetKeyState('p');
    bool scalePlayer = i->GetKeyState('s') && i->GetKeyState('p');
    bool centerPlayer = i->GetKeyState('c') && i->GetKeyState('p');
    
    char up = 'e';
    char down = 'g';
    char left = 'd';
    char right = 'f';
    char in = 'i';
    char out = 'o';
    
    float speed = 2.0f;
    
    if (movePlayer)
    {
        player->position.x += i->GetKeyState(right) ? speed : (i->GetKeyState(left) ? -speed : 0.0f);
        player->position.y += i->GetKeyState(up) ? speed : (i->GetKeyState(down) ? -speed : 0.0f);
        player->position.z += i->GetKeyState(in) ? speed : (i->GetKeyState(out) ? -speed : 0.0f);
        return;
    }
    
    if (rotatePlayer)
    {
        player->rotation.z += i->GetKeyState(up) ? speed : (i->GetKeyState(down) ? -speed : 0.0f);
        player->rotation.y += i->GetKeyState(left) ? speed : (i->GetKeyState(right) ? -speed : 0.0f);
        return;
    }
    
    if (scalePlayer)
    {
        player->scale.x += i->GetKeyState(right) ? speed : (i->GetKeyState(left) ? -speed : 0.0f);
        player->scale.y += i->GetKeyState(up) ? speed : (i->GetKeyState(down) ? -speed : 0.0f);
        return;
    }
    
    player->centered = centerPlayer;
    
    bool rotateCamera = i->GetKeyState('r') && i->GetKeyState('c');
    bool rotateWorld = i->GetKeyState('r') && i->GetKeyState('w');
    bool moveCamera = i->GetKeyState('m') && i->GetKeyState('c');
    bool zoomCamera = i->GetKeyState('z') && i->GetKeyState('c');
    
    if (rotateCamera)
    {
        
#define ON 1.0f
#define OFF 0.0f
        
        if (i->GetKeyState(up) || i->GetKeyState(down) ? ON : OFF)
            camera->Rotate(i->GetKeyState(up) || i->GetKeyState(left) ? -speed : (i->GetKeyState(down) || i->GetKeyState(right) ? speed : 0.0f),
                           0.0f,
                           0.0f);
        
        if (i->GetKeyState(left) || i->GetKeyState(right) ? ON : OFF)
            camera->Rotate(0.0f,
                           i->GetKeyState(up) || i->GetKeyState(left) ? -speed : (i->GetKeyState(down) || i->GetKeyState(right) ? speed : 0.0f),
                           0.0f);
        
        if (i->GetKeyState('z') ? ON : OFF)
            camera->Rotate(0.0f,
                           0.0f,
                           i->GetKeyState(up) || i->GetKeyState(left) ? speed : (i->GetKeyState(down) || i->GetKeyState(right) ? -speed : 0.0f));
        
#undef ON
#undef OFF
        
        return;
    }
    
    if (rotateWorld)
    {
#define ON 1.0f
#define OFF 0.0f
        
        if (i->GetKeyState(up) || i->GetKeyState(down) ? ON : OFF)
            camera->RotateWorld(i->GetKeyState(up) || i->GetKeyState(left) ? -speed : (i->GetKeyState(down) || i->GetKeyState(right) ? speed : 0.0f),
                                0.0f,
                                0.0f);
        
        if (i->GetKeyState(left) || i->GetKeyState(right) ? ON : OFF)
            camera->RotateWorld(0.0f,
                                i->GetKeyState(up) || i->GetKeyState(left) ? -speed : (i->GetKeyState(down) || i->GetKeyState(right) ? speed : 0.0f),
                                0.0f);
        
        if (i->GetKeyState('z') ? ON : OFF)
            camera->RotateWorld(0.0f,
                                0.0f,
                                i->GetKeyState(up) || i->GetKeyState(left) ? speed : (i->GetKeyState(down) || i->GetKeyState(right) ? -speed : 0.0f));
        
#undef ON
#undef OFF
        
        return;
    }
    
    if (moveCamera)
    {
        camera->Move(i->GetKeyState(left) ? speed : (i->GetKeyState(right) ? -speed : 0.0f),
                     i->GetKeyState(up) ? -speed : (i->GetKeyState(down) ? speed : 0.0f),
                     0.0);
        
        return;
    }
    
    
    if (zoomCamera)
    {
        if (i->GetKeyState(in))
        {
            camera->Move(0.0f, 0.0f, speed);
        }
        else if (i->GetKeyState(out))
        {
            camera->Move(0.0f, 0.0f, -speed);
        }
        return;
    }
    
    bool reset = i->GetKeyState('x');
    
    if (reset)
    {
        glLoadIdentity();
        camera->Reset();
        player->position.x =
        player->position.y =
        player->position.z =
        player->rotation.x =
        player->rotation.y =
        player->rotation.z = 0.0f;
        return;
    }
}

void Update(int value) {
    
	const double delta = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    static double lastFrameTime = 0.0f;
    
    Console::Instance()->HandleEvents();
    Console::Instance()->Update((delta - lastFrameTime));
    
    InputManager::Instance()->HandleEvents();
    InputManager::Instance()->Update((delta - lastFrameTime));
    
    DoStuff();
    
    camera->Update(delta - lastFrameTime);

    lastFrameTime = delta;
    
	glutPostRedisplay();
    
	glutTimerFunc(UPDATE_INTERVAL, Update, 0);
}

void HandleResize(int w, int h) {
    camera->Initialize(w, h);
}

void HandleKeyPress(unsigned char key, int x, int y)
{
    InputManager::Instance()->SetKeyState(key, true);
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }
}

void HandleKeyRelease(unsigned char key, int x, int y) {
    InputManager::Instance()->SetKeyState(key, false);
}

void HandleSpecialKeyPress(int key, int x, int y) {
    InputManager::Instance()->SetKeyState(key, true);
}

void HandleSpecialKeyRelease(int key, int x, int y) {
    InputManager::Instance()->SetKeyState(key, false);
}

void HandleMouse(int button, int state, int x, int y) { }

void HandleMousePassive(int x, int y) { }

void HandleMouseMotion(int x, int y) { }

void initGame(int w, int h)
{
    Console::Instance()->Init(0);
    
    InputManager::Instance()->Init(2, w, h);
    
    camera = new GameCamera(Camera3D);
    
	HandleResize(w, h);
    
    player = new GameObject();
    
    //IGameObjectComponent * sprite = new SpriteRendererComponent(player, "run.png");
    IGameObjectComponent * sprite = new SpriteRendererComponent(player, "run.png", Rect2DfMake(170.0f * 2.0f, 170.0f * 3.0f, 170.0f, 170.0f));
    
    player->AddComponent(sprite);
}

void endGame()
{
    InputManager::Instance()->Quit();
    InputManager::Instance()->Cleanup();
    delete InputManager::Instance();
    
    Console::Instance()->Quit();
    Console::Instance()->Cleanup();
    delete Console::Instance();
    
    delete camera;
    
    delete player;
}

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[]) {
#else
    int main(int argc, char* argv[]) {
#endif
        
        glutInit(&argc, (char**)argv);
        glutInitWindowSize(WIDTH, HEIGHT);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        
        glutCreateWindow("XGE Game Engine Window");
        
        glutDisplayFunc(Draw);
        glutReshapeFunc(HandleResize);
        glutKeyboardFunc(HandleKeyPress);
        glutKeyboardUpFunc(HandleKeyRelease);
        glutSpecialFunc(HandleSpecialKeyPress);
        glutSpecialUpFunc(HandleSpecialKeyRelease);
        
        glutMouseFunc(HandleMouse);
        glutMotionFunc(HandleMouseMotion);
        glutPassiveMotionFunc(HandleMousePassive);
        
        glutTimerFunc(UPDATE_INTERVAL, Update, 0);
        
        initGame(WIDTH, HEIGHT);
        
        glutMainLoop();
        
        endGame();
        
        return 0;
    }
    
    
