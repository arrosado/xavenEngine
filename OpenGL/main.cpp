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
#include <windows.h>
#include <tchar.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <IGame.h>
#else
#include <glut.h>
#include <Game/IGame.h>
#endif

#define WIDTH 600
#define HEIGHT 600

#define UPDATE_INTERVAL .25

float mouseX, mouseY, rotateX, rotateY;

IGameEngine *game = CreateGame();

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
	game->Render();
    
    glutSwapBuffers();
}

void Update(int value) {
    
	const double delta = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    
	game->Update(delta);
    
	glutPostRedisplay();
    
	glutTimerFunc(UPDATE_INTERVAL, Update, 0);
}

void HandleResize(int w, int h) {
	game->ResizeView(w, h);
}

void HandleKeyPress(unsigned char key, int x, int y)
{
	game->UpdateInput(key, true, 0, false, x, y);
    
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

void HandleKeyRelease(unsigned char key, int x, int y)
{
	game->UpdateInput(key, false, 0, false, x, y);
    glutPostRedisplay();
}

void HandleSpecialKeyPress(int key, int x, int y) {
	game->UpdateInput(key, true, 0, false, x, y);
	glutPostRedisplay();
}

void HandleSpecialKeyRelease(int key, int x, int y) {
	game->UpdateInput(key, false, 0, false, x, y);
	glutPostRedisplay();
}

void HandleMouse(int button, int state, int x, int y) {
	game->UpdateInput(-1, false, button, state, x, y);
}

void HandleMousePassive(int x, int y){
    mouseX = x;
    mouseY = y;
	game->UpdateInput(-1, false, -1, false, x, y);
}

void HandleMouseMotion(int x, int y){
    const float SPEED = 10;
    
    rotateX += (mouseX-x)/SPEED;
    rotateY += (mouseY-y)/SPEED;
    
    HandleMousePassive(x, y);
    
    glutPostRedisplay();
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
        
        game->Initialize(WIDTH, HEIGHT);
        
        glutMainLoop();
        
        game->Exit(true);
        
        //DestroyGame(game);
        delete game;
        
        return 0;
    }
    
    
