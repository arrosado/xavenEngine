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
#include <Game/Graphics/Image/Image.h>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/Console/Console.h>
#include <Game/Graphics/Grid/Grid.h>
#include <Game/Graphics/Texture2D/Texture2D.h>
#include <Game/Input/InputManager.h>


#define WIDTH 600
#define HEIGHT 600

#define UPDATE_INTERVAL .25

float mouseX, mouseY, rotateX, rotateY;
Image *image = NULL;
Grid* grid = NULL;

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
    GraphicsManager::Instance()->PrepareNewFrame();
    
    image->Render();
    // Working : image->Render(Vector3DfMake(0.0f, 0.0f, 0.0f), true);
    // Working : image->Render(Vector3DfMake(0.0f, 0.0f, 0.0f));
    // Working : image->Render(Vector3DfMake(-100.0f, 0.0f, 0.0f), Size2DfMake(0.5f, 0.5f), Vector3DfMake(1.0f, 0.0f, 0.0f));
    
    grid->Draw(GridType::XGrid);
    
    Console::Instance()->Draw();
	
    GraphicsManager::Instance()->RenderNewFrame();
    
    glutSwapBuffers();
}

void Update(int value) {
    
	const double delta = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    static double lastFrameTime = 0.0f;
    
    InputManager::Instance()->HandleEvents();
    InputManager ::Instance()->Update((delta - lastFrameTime));

    GraphicsManager::Instance()->HandleEvents();
    GraphicsManager::Instance()->Update((delta - lastFrameTime));
    
    Console::Instance()->HandleEvents();
    Console::Instance()->Update((delta - lastFrameTime));
    
    lastFrameTime = delta;
    
    static float rotation = 0.0f;
    
    bool rotate = InputManager::Instance()->GetKeyState('r');
    bool clockwise = InputManager::Instance()->GetKeyState('e');
    
    if (fabs(rotation) > 360.0f)
        rotation = 0.0f;
    
    if (rotate)
        rotation += clockwise ? -1 : 1;
    
    image->SetRotation(Vector3DfMake(0.0f, 0.0f, rotation));
    
    Console* c = Console::Instance();
	
	c->Write("Image Rotation = %.1f", rotation);
	c->Write("Image rotating %s", clockwise ? "Clockwise" : "CounterClockwise");
    
    
	glutPostRedisplay();
    
	glutTimerFunc(UPDATE_INTERVAL, Update, 0);
}

void HandleResize(int w, int h) {
    GraphicsManager::Instance()->ResizeView(w, h);
}

void HandleKeyPress(unsigned char key, int x, int y)
{
    if (key >= 0) InputManager::Instance()->SetKeyState(key, true);
    
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }
}

void HandleKeyRelease(unsigned char key, int x, int y)
{
    if (key >= 0) InputManager::Instance()->SetKeyState(key, false);
}

void HandleSpecialKeyPress(int key, int x, int y) {
    if (key >= 0) InputManager::Instance()->SetKeyState(key, true);
}

void HandleSpecialKeyRelease(int key, int x, int y) {
    if (key >= 0) InputManager::Instance()->SetKeyState(key, false);
}

void HandleMouse(int button, int state, int x, int y) {
    InputManager::Instance()->UpdatePointer(button, state, x, y);
}

void HandleMousePassive(int x, int y){
    mouseX = x;
    mouseY = y;
    InputManager::Instance()->UpdatePointer(-1, false, x, y);
}

void HandleMouseMotion(int x, int y){
    const float SPEED = 10;
    
    rotateX += (mouseX-x)/SPEED;
    rotateY += (mouseY-y)/SPEED;
    
    HandleMousePassive(x, y);
}

void initGame(int w, int h)
{
    GraphicsManager::Instance()->Init(2, w, h);
    InputManager::Instance()->Init(3, w, h, 100);
    Console::Instance()->Init(0);
    image = new Image(RESOURCES_FOLDER "run.png", GL_LINEAR);
    grid = new Grid(20, 20, 20);

}

void endGame()
{
    InputManager::Instance()->Quit();
    InputManager::Instance()->Cleanup();
    delete InputManager::Instance();
    
    GraphicsManager::Instance()->Quit();
    GraphicsManager::Instance()->Cleanup();
    delete GraphicsManager::Instance();
    
    Console::Instance()->Quit();
    Console::Instance()->Cleanup();
    delete Console::Instance();
    
    delete image;
    delete grid;
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
        
        //GLenum err = glewInit();
        //if (GLEW_OK != err) {
        //	/* Problem: glewInit failed, something is seriously wrong. */
        //	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        //}
        //fprintf(stdout, "Status Using GLEW %s\n", glewGetString(GLEW_VERSION));
        //
        //if (!glewIsSupported("GL_VERSION_2_0")) {
        //	fprintf(stderr, "No open GL 2.0 support...");
        //	exit(1);
        //}
        
        
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
    
    
