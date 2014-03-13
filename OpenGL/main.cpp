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
#include <Game/Graphics/Texture2D/Texture2D.h>


#define WIDTH 600
#define HEIGHT 600

#define UPDATE_INTERVAL .25

Texture2D *textureLoader = NULL;
Texture texture;

// What's this???? [Start]
static const GLfloat vertices[] = { -0.5f, -0.33f,
100.5f, -100.33f,
-100.5f, 100.33f,
100.5f, 100.33f };

//static const GLubyte colors[] = {
//255, 255, 0, 255,
//0, 255, 255, 255,
//0, 0, 0, 0,
//255, 0, 255, 255
//};


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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearColor(0.60f, 0.70f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Console::Instance()->Draw();
	
    // Do Rendering here! [Start]
    glEnable(GL_DEPTH_TEST);
    
    glColor3f(255.0f, 0.0f, 0.0f);
    glPushMatrix();
    glRectf(-25.0f, -25.0f, 25.0f, 25.0f);
    glPopMatrix();
    glFlush();
    glDisable(GL_DEPTH_TEST);
    
	//glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    
	// Populate the vertex, texcoord and colorpointers with our interleaved vertex data
    //glVertexPointer(2, GL_FLOAT, 0, vertices);
    //glTexCoordPointer(2, GL_FLOAT, sizeof(TexturedColoredVertex), &iva[0].textureVertex);
    //glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	
	//glBindTexture(GL_TEXTURE_2D, texture.texture->name);
		
    // Now load the indices array with indexes into the IVA, we draw those triangles to the screen.
    //glDrawElements(GL_TRIANGLES, vertexCounter, GL_UNSIGNED_SHORT, ivaIndices);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	//glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    
    // Do Rendering here! [End]
    
    glutSwapBuffers();
}

void Update(int value) {
    
	const double delta = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    static double lastFrameTime = 0.0f;
    
    Console::Instance()->HandleEvents();
    Console::Instance()->Update((delta - lastFrameTime));

    lastFrameTime = delta;
    
	glutPostRedisplay();
    
	glutTimerFunc(UPDATE_INTERVAL, Update, 0);
}

void HandleResize(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);

	GLdouble fov, aspect, zNear, zFar, xmin, xmax, ymin, ymax;

	fov = 45.0f; // Field of view.
	aspect = w / h; // Aspect ratio of screen.
	zNear = 2;
	zFar = 1000000;

	ymax = zNear * tan(fov * M_PI / 360.0f);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;


	//glFrustum(xmin, xmax, ymin, ymax, zNear, zFar); /* 3D camera type */
	//glOrtho(-w/2, w/2, -h/2, h/2, zNear, zFar);
    gluOrtho2D(0.0f, (GLdouble)w, 0.0f, (GLdouble)h);
    
	/*
	* Switch to GL_MODELVIEW so we can now draw our objects
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void HandleKeyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }
}

void HandleKeyRelease(unsigned char key, int x, int y) { }

void HandleSpecialKeyPress(int key, int x, int y) { }

void HandleSpecialKeyRelease(int key, int x, int y) { }

void HandleMouse(int button, int state, int x, int y) { }

void HandleMousePassive(int x, int y){ }

void HandleMouseMotion(int x, int y){ }

void initGame(int w, int h)
{
	HandleResize(w, h);
    
    Console::Instance()->Init(0);
    
    /*
    // Load texture [Start]
    textureLoader = new Texture2D(RESOURCES_FOLDER "run.png", GL_LINEAR);
    texture.name = RESOURCES_FOLDER "run.png";
	texture.texture = textureLoader;
	texture.retainCount = 0;
	texture.retainCount++;
    // Load texture [End]
     */
}

void endGame()
{
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
    
    
