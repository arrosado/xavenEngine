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


#define WIDTH 800
#define HEIGHT 600

#define UPDATE_INTERVAL .25

#define CAMERA_TYPE_3D

Texture2D *textureLoader = NULL;
Texture texture;

Image image;

IGameObject * player;

GLfloat rotation = 0.0f;
GLfloat zoom = 0.0f;

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
    glEnable(GL_DEPTH_TEST);
#ifdef CAMERA_TYPE_3D
	glRotatef(rotation, 1.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, zoom);
#else
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(1.0f + (zoom/2.0f), 1.0f + (zoom/2.0f), 1.0f + (zoom/2.0f));
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
    
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ALPHA);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glBindTexture(GL_TEXTURE_2D, textureLoader->name);
#ifdef CAMERA_TYPE_3D
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex) , &image.vertex1.geometry);
#else
    glVertexPointer(2, GL_FLOAT, sizeof(Vertex) , &image.vertex1.geometry);
#endif
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &image.vertex1.texture);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &image.vertex1.color);

	//glDrawElements(GL_TRIANGLES, vertexCounter, GL_UNSIGNED_SHORT, ivaIndices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
    
    
	

	Console::Instance()->Write("Rotation on X: %f", rotation);
	Console::Instance()->Write("Zoom on Z: %f", zoom);
	Console::Instance()->Draw();

    
    glFlush();
    glDisable(GL_DEPTH_TEST);
    
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

	GLdouble zNear, zFar;
    zNear = 1;
	zFar = 500;



#ifdef CAMERA_TYPE_3D
    GLdouble fov, aspect, xmin, xmax, ymin, ymax;
    
    fov = 45.0f; // Field of view.
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
    glTranslatef(0.0f, 0.0f, -20.0f);
#else
	glScalef(20.0f, 20.0f, 20.0f);
    glTranslatef(0.0f, 0.0f, -1.0f);
#endif
}

void HandleKeyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
		case 'r':
			rotation += 0.1f;
			break;
		case 'e':
			rotation -= 0.1f;
			break;
		case 'i':
			zoom += 0.1f;
			break;
		case 'o':
			zoom -= 0.1f;
			break;
		case 'x':
			glLoadIdentity();
			rotation = 0.0f;
			zoom = 0.0f;
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
    
    IGameObjectComponent * sprite = new SpriteRendererComponent("run.png");
    
    player = new GameObject();
    player->AddComponent(sprite);
    
    // Load texture [Start]
    textureLoader = new Texture2D(RESOURCES_FOLDER "blink.png", GL_LINEAR);
    texture.name = RESOURCES_FOLDER "blink.png";
	texture.data = textureLoader;
	texture.retainCount = 0;
	texture.retainCount++;
    // Load texture [End]
    
    GLfloat z = 10.0f;
    
    // Set image geometry
    image.vertex1.geometry.x = 0.0f;
    image.vertex1.geometry.y = 0.0f;
    image.vertex1.geometry.z = z;
    
    image.vertex2.geometry.x = texture.data->contentSize.width;
    image.vertex2.geometry.y = 0.0f;
    image.vertex2.geometry.z = z;
    
    image.vertex3.geometry.x = 0.0f;
    image.vertex3.geometry.y = texture.data->contentSize.height;
    image.vertex3.geometry.z = z;
    
    image.vertex4.geometry.x = texture.data->contentSize.width;
    image.vertex4.geometry.y = texture.data->contentSize.height;
    image.vertex4.geometry.z = z;
    
    // Set image colors
    image.vertex1.color = Color4fWhite;
    image.vertex2.color = Color4fWhite;
    image.vertex3.color = Color4fWhite;
    image.vertex4.color = Color4fWhite;
    
    // Set texture coordinates
    image.vertex1.texture.x = 0.0f;
    image.vertex1.texture.y = 0.0f;
    
    image.vertex2.texture.x = texture.data->maxS;
    image.vertex2.texture.y = 0.0f;
    
    image.vertex3.texture.x = 0.0f;
    image.vertex3.texture.y = texture.data->maxT;
    
    image.vertex4.texture.x = texture.data->maxS;
    image.vertex4.texture.y = texture.data->maxT;
    
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
    
    
