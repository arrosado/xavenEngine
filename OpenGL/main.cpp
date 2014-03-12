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
#include <Game/IComponent.h>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/IGameCamera.h>
#include <Game/Graphics/Console/Console.h>
#include <Game/Graphics/Grid/Grid.h>
#include <Game/Graphics/Texture2D/Texture2D.h>
#include <Game/Input/InputManager.h>


#define WIDTH 600
#define HEIGHT 600

#define UPDATE_INTERVAL .25

float mouseX, mouseY, rotateX, rotateY;
Grid* grid = NULL;
IGameCamera *camera = NULL;
Texture2D *textureLoader = NULL;
Texture texture;

Vector3Df cam_position = Vector3DfZero;
Vector3Df cam_rotation = Vector3DfZero;


// What's this???? [Start]
static const GLfloat vertices[] = { -0.5f, -0.33f,
0.5f, -0.33f,
-0.5f, 0.33f,
0.5f, 0.33f };

static const GLubyte colors[] = {
255, 255, 0, 255,
0, 255, 255, 255,
0, 0, 0, 0,
255, 0, 255, 255
};

float matrix[9];
bool dirty = false;
Vector3Df point;
Vector3Df rotation;
Vector3Df rotationPoint;
Size2Df scale;
bool flipHorizontally;
bool flipVertically;
// What's this???? [End]



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
	glClearColor(0.60f, 0.70f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Working : image->Render();
    // Working : image->Render(Vector3DfMake(0.0f, 0.0f, 0.0f), true);
    // Working : image->Render(Vector3DfMake(0.0f, 0.0f, 0.0f));
    // Working : image->Render(Vector3DfMake(-100.0f, 0.0f, 0.0f), Size2DfMake(0.5f, 0.5f), Vector3DfMake(1.0f, 0.0f, 0.0f));
    
    // Modify matrix here! [Start]
    // If the images point, scale or rotation are changed, it means we need to adjust the
    // images matrix and transform the vertices.  If dirty is set we also check to see if it is
    // necessary to adjust the rotation and scale.  If they are 0 then nothing needs to
    // be done and we can save some cycles.
    /*if (dirty) {
        // Load the identity matrix before applying transforming the matrix for this image.  The
        // order in which the transformations are applied is important.
        loadIdentityMatrix(matrix);
		
        // Translate the position of the image first
        translateMatrix(matrix, point);
		
        // If this image has been configured to be flipped vertically or horizontally
        // then set the scale for the image to -1 for the appropriate axis and then translate
        // the image so that the images origin is rendered in the correct place
        if(flipVertically) {
            scaleMatrix(matrix, Size2DfMake(1, -1));
            translateMatrix(matrix, Vector3DfMake(0, (-textureLoader->contentSize.height * scale.height), 0));
        }
		
        if(flipHorizontally) {
            scaleMatrix(matrix, Size2DfMake(-1, 1));
            translateMatrix(matrix, Vector3DfMake((-textureLoader->contentSize.width * scale.width), 0, 0));
        }
        
		// No point in calculating a rotation matrix if there is no rotation been set
        if(rotation.z != 0)
            rotateMatrix(matrix, rotationPoint, rotation.z);
        
        // No point in calculcating scale if no scale has been set.
		if(scale.width != 1.0f || scale.height != 1.0f)
            scaleMatrix(matrix, scale);
        
        // Transform the images matrix based on the calculations done above
        transformMatrix(matrix, image->texturedColoredQuad, image->texturedColoredQuadIVA);
        
        // Mark the image as now clean
		dirty = false;
	}*/
    // Modify matrix here! [End]
    
    grid->Draw(GridType::XGrid);
    
    Console::Instance()->Draw();
	
    // Do Rendering here! [Start]
    
    /*
	 * Setup how textures should be rendered i.e.
	 * how a texture with alpha should be rendered
	 * on top of another texture.
	 */
	if (!glIsEnabled(GL_BLEND)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
    
	if (!glIsEnabled(GL_TEXTURE_2D)) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ALPHA);
	}
    
	if (!glIsEnabled(GL_DEPTH_TEST))
		glEnable(GL_DEPTH_TEST);
    
	glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
	// Populate the vertex, texcoord and colorpointers with our interleaved vertex data
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    //glTexCoordPointer(2, GL_FLOAT, sizeof(TexturedColoredVertex), &iva[0].textureVertex);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	
	//glBindTexture(GL_TEXTURE_2D, texture.texture->name);
		
    // Now load the indices array with indexes into the IVA, we draw those triangles to the screen.
    //glDrawElements(GL_TRIANGLES, vertexCounter, GL_UNSIGNED_SHORT, ivaIndices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    
	if (glIsEnabled(GL_BLEND))
		glDisable(GL_BLEND);
    
	if (glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);
    
	if (glIsEnabled(GL_DEPTH_TEST))
		glDisable(GL_DEPTH_TEST);
    
    // Do Rendering here! [End]
    
    glutSwapBuffers();
}

void Update(int value) {
    
	const double delta = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    static double lastFrameTime = 0.0f;
    
    InputManager::Instance()->HandleEvents();
    InputManager ::Instance()->Update((delta - lastFrameTime));
    
    Console::Instance()->HandleEvents();
    Console::Instance()->Update((delta - lastFrameTime));

    // GameGraphics Handle Events [Start]
    
    bool up, down, left, right, cam;
    
	float SPEED = 10.0f;
    
	InputManager * i = InputManager::Instance();
    
	up = i->GetKeyState('i');
	down = i->GetKeyState('k');
	right = i->GetKeyState('l');
	left = i->GetKeyState('j');
	cam = i->GetKeyState('c');
    
	if (i->GetKeyState('z'))
		camera->Reset();
    
	if (up || down || right || left)
		camera->Move(right && left ? 0.0f : right ? -SPEED/2 : left ? SPEED/2 : 0.0f,
							 0.0f,
							 down && up ? 0.0 : up ? SPEED : down ? -SPEED : 0.0f);
    
	int m_x = 0;
	int m_y = 0;
	int speed = 10;
	int direction = 1;
    
	switch(i->GetJoystick()->Speed.Upward) {
        case GameJoyStick::eJoyStickSpeeds::Slow:
            m_y = direction * speed * 1;
            break;
        case GameJoyStick::eJoyStickSpeeds::Normal:
            m_y = direction * speed * 2;
            break;
        case GameJoyStick::eJoyStickSpeeds::Moderate:
            m_y = direction * speed * 3;
            break;
        case GameJoyStick::eJoyStickSpeeds::Fast:
            m_y = direction * speed * 4;
            break;
        default:
            break;
	}
    
	direction = -1;
    
	switch(i->GetJoystick()->Speed.Downward) {
        case GameJoyStick::eJoyStickSpeeds::Slow:
            m_y = direction * speed * 1;
            break;
        case GameJoyStick::eJoyStickSpeeds::Normal:
            m_y = direction * speed * 2;
            break;
        case GameJoyStick::eJoyStickSpeeds::Moderate:
            m_y = direction * speed * 3;
            break;
        case GameJoyStick::eJoyStickSpeeds::Fast:
            m_y = direction * speed * 4;
            break;
        default:
            break;
	}
    
	direction = -1;
    
	switch(i->GetJoystick()->Speed.Leftward) {
        case GameJoyStick::eJoyStickSpeeds::Slow:
            m_x = direction * speed * 1;
            break;
        case GameJoyStick::eJoyStickSpeeds::Normal:
            m_x = direction * speed * 2;
            break;
        case GameJoyStick::eJoyStickSpeeds::Moderate:
            m_x = direction * speed * 3;
            break;
        case GameJoyStick::eJoyStickSpeeds::Fast:
            m_x = direction * speed * 4;
            break;
        default:
            break;
	}
    
	direction = 1;
    
	switch(i->GetJoystick()->Speed.Rightward) {
        case GameJoyStick::eJoyStickSpeeds::Slow:
            m_x = direction * speed * 1;
            break;
        case GameJoyStick::eJoyStickSpeeds::Normal:
            m_x = direction * speed * 2;
            break;
        case GameJoyStick::eJoyStickSpeeds::Moderate:
            m_x = direction * speed * 3;
            break;
        case GameJoyStick::eJoyStickSpeeds::Fast:
            m_x = direction * speed * 4;
            break;
        default:
            break;
	}
    
	if (cam)
		camera->Rotate(m_y, m_x, 0.0f);

    
    // GameGraphics Handle Events [End]
    
    camera->Update((delta - lastFrameTime));
    
    lastFrameTime = delta;
    
    static float rotationAngle = 0.0f;
    
    bool rotate = InputManager::Instance()->GetKeyState('r');
    bool clockwise = InputManager::Instance()->GetKeyState('e');
    
    if (fabs(rotationAngle) > 360.0f)
        rotationAngle = 0.0f;
    
    if (rotate)
        rotationAngle += clockwise ? -1 : 1;
    
    // ToDo: Rotate image here with the rotation variable
    rotation = Vector3DfMake(0.0f, 0.0f, rotationAngle);
    
    Console* c = Console::Instance();
	
	c->Write("Image Rotation = %.1f", rotationAngle);
	c->Write("Image rotating %s", clockwise ? "Clockwise" : "CounterClockwise");
    
    
	glutPostRedisplay();
    
	glutTimerFunc(UPDATE_INTERVAL, Update, 0);
}

void HandleResize(int w, int h) {
	/*
	* Initialize the Screen Orientation.
	*
	* Switch to GL_PROJECTION matrix mode
	* and reset the current matrix with the
	* identity matrix
	*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	* Switch the orientation of the view port
	* to match the current device orientation.
	*
	* {0, 0} at the middle of the screen.
	*/

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


	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar); /* 3D camera type */
	//glOrtho(-w/2, w/2, -h/2, h/2, zNear, zFar);

	/*
	* Switch to GL_MODELVIEW so we can now draw our objects
	*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Camera local coordinate system translation.
	* If moving the camera in its local coordinate system is desired,
	* then the translation has to be done before any rotation. If
	* translation is done after rotating, then the translation will
	* be executed on world coordinates.
	*/
	glTranslatef(cam_position.x, cam_position.y, cam_position.z);

	/*
	* Rotate the projection matrix on the cameras
	* local coordinate system.
	*/
	glRotatef(cam_rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(cam_rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(cam_rotation.z, 0.0f, 0.0f, 1.0f);

	//glTranslatef(this->position.x, this->position.y, this->position.z);
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
    camera = CreateGameCamera();
	HandleResize(w, h);
    
    InputManager::Instance()->Init(3, w, h, 100);
    Console::Instance()->Init(0);

    grid = new Grid(20, 20, 20);
    
    // Load texture [Start]
    textureLoader = new Texture2D(RESOURCES_FOLDER "run.png", GL_LINEAR);
    
    texture.name = RESOURCES_FOLDER "run.png";
	texture.texture = textureLoader;
	texture.retainCount = 0;
	texture.retainCount++;
    // Load texture [End]
}

void endGame()
{
    InputManager::Instance()->Quit();
    InputManager::Instance()->Cleanup();
    delete InputManager::Instance();
    
    Console::Instance()->Quit();
    Console::Instance()->Cleanup();
    delete Console::Instance();
    
    delete grid;
    delete camera;
    delete textureLoader;
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
    
    
