//
//  2DGameCamera.cpp
//  ARR Game Engine Core
//
//  Created by Ariel Rosado on 3/13/11.
//  Copyright 2011 Xaven. All rights reserved.
//

#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/IGameCamera.h>
#include <Game/Graphics/Motion.h>
#include <math.h>

#define DEFAULT_MOTION_ANIMATION_DURATION 0.15f
#define DEFAULT_MOTION_ANIMATION_SPEED 1.0f
#define INITIAL_Z_POSITION -850.0f

class Camera : public IGameCamera {
private:
	/*
	 * This struct defines the clear color.
	 */
	Color4f m_clearColor;
	
	vec3 position;
	vec3 direction;
	vec3 rotation;

	Size2Df screen;

	enum CameraAnimations {
		XAnimation = 0,
		YAnimation,
		ZAnimation,
		RXAnimation,
		RYAnimation,
		RZAnimation,
		DXAnimation,
		DYAnimation,
		DZAnimation,
		AnimationCount
	};
	
	MotionAnimation* animations[AnimationCount];
	
public:
	Camera();
	~Camera();
	
	void InitializeOpenGL(int w, int h);
	void PrepareNewFrame();
	void Update(float delta);
	void Move(float x, float y, float z);
	void Move(float x, float y, float z, float duration);
	void Zoom(float zoom);
	void Zoom(float zoom, float duration);
	void Rotate(float x, float y, float z);
	void Rotate(float x, float y, float z, float duration);
	void PointAt(float x, float y, float z);
	void PointAt(float x, float y, float z, float duration);
	void Reset();
};

IGameCamera* CreateGameCamera() {
	return new Camera();
}


Camera::Camera() {
	this->m_clearColor = Color4fMake(0.60f, 0.70f, 0.85f, 1.0f);
	
	this->animations[XAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.x, this->position.x, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[YAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.y, this->position.y, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[ZAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.z, this->position.z, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[RXAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->rotation.x, this->rotation.x, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[RYAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->rotation.y, this->rotation.y, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[RZAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->rotation.z, this->rotation.z, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[DXAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.x, this->direction.x, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[DYAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.y, this->direction.y, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[DZAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.z, this->direction.z, DEFAULT_MOTION_ANIMATION_DURATION);

	this->Reset();
}

Camera::~Camera() {
	this->position = vec3(0.0f, 0.0f, 0.0f);
	this->direction = vec3(0.0f, 0.0f, 0.0f);
	this->rotation = vec3(0.0f, 0.0f, 0.0f);
	this->m_clearColor = Color4fZero;

	for(int i = 0; i < AnimationCount; i++)
		delete this->animations[i];
}

void Camera::PrepareNewFrame() {
	/*
	 * Clear the Render Buffer.
	 */
	glClearColor(this->m_clearColor.red, this->m_clearColor.green, this->m_clearColor.blue, this->m_clearColor.alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::InitializeOpenGL(int w, int h) {

	this->screen.width = w;
	this->screen.height =  h;
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
    
    glViewport(0, 0, this->screen.width, this->screen.height);

	GLdouble fov, aspect, zNear, zFar, xmin, xmax, ymin, ymax;

	fov = 45.0f; // Field of view.
	aspect = this->screen.width/this->screen.height; // Aspect ratio of screen.
	zNear = 2;
	zFar = 1000000;

	ymax = zNear * tan(fov * M_PI / 360.0f);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;

	
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar); /* 3D camera type */
	//glOrtho(-this->screen.width/2, this->screen.width/2, -this->screen.height/2, this->screen.height/2, zNear, zFar);

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
	glTranslatef(this->position.x, this->position.y, this->position.z);
	
	/* 
	 * Rotate the projection matrix on the cameras
	 * local coordinate system.
	 */
	glRotatef(this->rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(this->rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(this->rotation.z, 0.0f, 0.0f, 1.0f);

	//glTranslatef(this->position.x, this->position.y, this->position.z);

	//gluLookAt(0.0f, 100.0f, 500.0f, 
	//			this->direction.x, this->direction.y, this->direction.z, 
	//			0.0f, 1.0f, 0.0f);
    /*
	float m[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	Vector3Df rv = Vector3DfMake(m[0], m[4], m[8] ); // Right vector
	Vector3Df uv = Vector3DfMake(m[1], m[5], m[9] ); // Up vector
	Vector3Df fv = Vector3DfMake(m[2], m[6], m[10]); // Forward vector
     */
}

void Camera::Update(float delta) {

	bool updated = false;

	for(int i = 0; i < AnimationCount; i++)
		if (this->animations[i]->Update(delta))
			updated = true;

	if (updated)
    {
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
        glTranslatef(this->position.x, this->position.y, this->position.z);
        
        /*
         * Rotate the projection matrix on the cameras
         * local coordinate system.
         */
        glRotatef(this->rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(this->rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(this->rotation.z, 0.0f, 0.0f, 1.0f);
    }
		//this->InitializeOpenGL(this->screen.width, this->screen.height);
}

void Camera::Move(float x, float y, float z) {
	this->Move(x, y, z, DEFAULT_MOTION_ANIMATION_DURATION);
}

void Camera::Move(float x, float y, float z, float duration) {
	
	this->animations[XAnimation]->Reset();
	this->animations[XAnimation]->SetDuration(duration);
	this->animations[XAnimation]->SetDesired(x + this->position.x);
	this->animations[XAnimation]->Activate();
	
	this->animations[YAnimation]->Reset();
	this->animations[YAnimation]->SetDuration(duration);
	this->animations[YAnimation]->SetDesired(y + this->position.y);
	this->animations[YAnimation]->Activate();
	
	this->animations[ZAnimation]->Reset();
	this->animations[ZAnimation]->SetDuration(duration);
	this->animations[ZAnimation]->SetDesired(z + this->position.z);
	this->animations[ZAnimation]->Activate();
}

void Camera::Zoom(float zoom) {
	this->Zoom(zoom, DEFAULT_MOTION_ANIMATION_DURATION);
}

void Camera::Zoom(float zoom, float duration) {
	this->animations[ZAnimation]->Reset();
	this->animations[ZAnimation]->SetDuration(duration);
	this->animations[ZAnimation]->SetDesired(zoom + this->position.z);
	this->animations[ZAnimation]->Activate();
}

void Camera::Rotate(float x, float y, float z) {
	this->Rotate(x, y, z, DEFAULT_MOTION_ANIMATION_DURATION);
}

void Camera::Rotate(float x, float y, float z, float duration) {

	float newX, newY, newZ;
	newX = x+this->rotation.x;//std::fmod(x+this->rotation.x, 360.0f);
	newY = y+this->rotation.y;//std::fmod(y+this->rotation.y, 360.0f);
	newZ = z+this->rotation.z;//std::fmod(z+this->rotation.z, 360.0f);

	this->animations[RXAnimation]->Reset();
	this->animations[RXAnimation]->SetDuration(duration);
	this->animations[RXAnimation]->SetDesired(newX);
	this->animations[RXAnimation]->Activate();
	
	this->animations[RYAnimation]->Reset();
	this->animations[RYAnimation]->SetDuration(duration);
	this->animations[RYAnimation]->SetDesired(newY);
	this->animations[RYAnimation]->Activate();
	
	this->animations[RZAnimation]->Reset();
	this->animations[RZAnimation]->SetDuration(duration);
	this->animations[RZAnimation]->SetDesired(newZ);
	this->animations[RZAnimation]->Activate();
}

void Camera::PointAt(float x, float y, float z) {
	this->PointAt(x, y, z, DEFAULT_MOTION_ANIMATION_DURATION);
}

void Camera::PointAt(float x, float y, float z, float duration) {
	this->animations[DXAnimation]->Reset();
	this->animations[DXAnimation]->SetDuration(duration);
	this->animations[DXAnimation]->SetDesired(x);
	this->animations[DXAnimation]->Activate();

	this->animations[DYAnimation]->Reset();
	this->animations[DYAnimation]->SetDuration(duration);
	this->animations[DYAnimation]->SetDesired(y);
	this->animations[DYAnimation]->Activate();

	this->animations[DZAnimation]->Reset();
	this->animations[DZAnimation]->SetDuration(duration);
	this->animations[DZAnimation]->SetDesired(z);
	this->animations[DZAnimation]->Activate();
}

void Camera::Reset() {
	this->position = vec3(0.0f, 0.0f, 0.0f);
	this->rotation = vec3(0.0f, 0.0f, 0.0f);
	this->direction = vec3(0.0f, 0.0f, 0.0f);
	this->Zoom(INITIAL_Z_POSITION);
}