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
#include <IGameObject.h>
#include <Game/Input/InputManager.h>


#define WIDTH 800
#define HEIGHT 600

#define UPDATE_INTERVAL .25

#define CAMERA_TYPE_3D

IGameObject * player;

GLfloat angle = 0.0f;
GLfloat zoom = 0.0f;
vec2 cameraPosition = vec2(0.0f, 0.0f);
vec3 cameraRotation = vec3(0.0f, 0.0f, 0.0f);

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
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearColor(0.60f, 0.70f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    // Do Rendering here! [Start]
    
    Console::Instance()->Write("Camera position x:%f y:%f", cameraPosition.x, cameraPosition.y);
	Console::Instance()->Write("Camera rotation on a:%f on %s", angle, cameraRotation.x == 1.0f ? "x" :
                               (cameraRotation.y == 1.0f ? "y" :
                                (cameraRotation.z == 1.0f ? "z" : "none")));
	Console::Instance()->Write("Camera zoom on Z: %f", zoom);
    Console::Instance()->Write("Player position x:%f y:%f z:%f", player->position.x, player->position.y, player->position.z);
    Console::Instance()->Write("Player rotation x:%f y:%f z:%f", player->rotation.x, player->rotation.y, player->rotation.z);
    Console::Instance()->Write("Player is %s", player->centered ? "Centered" : "Not Centered");
    Console::Instance()->Write("Player scale w:%f h:%f d:%f", player->scale.x, player->scale.y, player->scale.z);
    

#ifdef CAMERA_TYPE_3D
    glEnable(GL_DEPTH_TEST);
    if (cameraRotation.x || cameraRotation.y || cameraRotation.z)
        glRotatef(angle, cameraRotation.x, cameraRotation.y, cameraRotation.z);
    glTranslatef(cameraPosition.x, cameraPosition.y, zoom);
    angle =
    cameraPosition.x =
    cameraPosition.y =
    zoom = 0.0f;
#else
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f + (zoom/2.0f), 1.0f + (zoom/2.0f), 1.0f + (zoom/2.0f));
    glTranslatef(cameraPosition.x, cameraPosition.y, 0.0f);
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
    bool moveCamera = i->GetKeyState('m') && i->GetKeyState('c');
    bool zoomCamera = i->GetKeyState('z') && i->GetKeyState('c');
    
    if (rotateCamera)
    {
        angle += i->GetKeyState(up) || i->GetKeyState(left) ? speed : (i->GetKeyState(down) || i->GetKeyState(right) ? -speed : 0.0f);
        
        
#define ON 1.0f
#define OFF 0.0f
        
        cameraRotation.x = i->GetKeyState(up) || i->GetKeyState(down) ? ON : OFF;
        cameraRotation.y = i->GetKeyState(left) || i->GetKeyState(right) ? ON : OFF;
        cameraRotation.z = i->GetKeyState('z') ? ON : OFF;
        
#undef ON
#undef OFF
        
        return;
    }
    
    if (moveCamera)
    {
        cameraPosition.x += i->GetKeyState(left) ? speed : (i->GetKeyState(right) ? -speed : 0.0f);
        cameraPosition.y += i->GetKeyState(up) ? speed : (i->GetKeyState(down) ? -speed : 0.0f);
        return;
    }
    
    
    if (zoomCamera)
    {
        if (i->GetKeyState(in))
        {
            zoom += speed;
        }
        else if (i->GetKeyState(out))
        {
            zoom -= speed;
        }
        return;
    }
    
    bool reset = i->GetKeyState('x');
    
    if (reset)
    {
        glLoadIdentity();
        angle =
        cameraPosition.x =
        cameraPosition.y =
        player->position.x =
        player->position.y =
        player->position.z =
        player->rotation.x =
        player->rotation.y =
        player->rotation.z =
        zoom = 0.0f;
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

    lastFrameTime = delta;
    
	glutPostRedisplay();
    
	glutTimerFunc(UPDATE_INTERVAL, Update, 0);
}

void HandleResize(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);

	GLdouble zNear, zFar;
    zNear = 1;
	zFar = 1500;



#ifdef CAMERA_TYPE_3D
    GLdouble fov, aspect, xmin, xmax, ymin, ymax;
    
    fov = 65.0f; // Field of view.
	aspect = w / h; // Aspect ratio of screen.
	   
	ymax = zNear * tan(fov * M_PI / 360.0f);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
    
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar); /* 3D camera type */
#else
	glOrtho(-w, (GLfloat)w, -h, (GLfloat)h, zNear, zFar);
#endif
    
	/*
	* Switch to GL_MODELVIEW so we can now draw our objects
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#ifdef CAMERA_TYPE_3D
    glTranslatef(0.0f, 0.0f, -250.0f);
#else
	glScalef(20.0f, 20.0f, 20.0f);
    glTranslatef(0.0f, 0.0f, -1.0f);
#endif
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

void HandleMousePassive(int x, int y){ }

void HandleMouseMotion(int x, int y){ }

void initGame(int w, int h)
{
	HandleResize(w, h);
    
    Console::Instance()->Init(0);
    
    InputManager::Instance()->Init(2, w, h);
    
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
    
    //delete textureLoader;
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
    
    
