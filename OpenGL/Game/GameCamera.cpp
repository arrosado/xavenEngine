//
//  GameCamera.cpp
//  OpenGL
//
//  Created by Ariel Rosado Rivera on 4/5/14.
//  Copyright (c) 2014 Ariel Rosado. All rights reserved.
//

#include <Game/GameCamera.h>

#define DEFAULT_MOTION_ANIMATION_DURATION 0.15f
#define DEFAULT_MOTION_ANIMATION_SPEED 1.0f
#define INITIAL_Z_POSITION -850.0f

GameCamera::GameCamera(CameraType cameraType) {
	this->m_clearColor = Color4fMake(0.60f, 0.70f, 0.85f, 1.0f);
    this->m_cameraType = cameraType;
	
	this->animations[XAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.x, this->position.x, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[YAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.y, this->position.y, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[ZAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.z, this->position.z, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[RXAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->rotation.x, this->rotation.x, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[RYAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->rotation.y, this->rotation.y, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[RZAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->rotation.z, this->rotation.z, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[DXAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.x, this->m_direction.x, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[DYAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.y, this->m_direction.y, DEFAULT_MOTION_ANIMATION_DURATION);
	this->animations[DZAnimation] = new MotionAnimation(new Exponential(), EaseOut, &this->position.z, this->m_direction.z, DEFAULT_MOTION_ANIMATION_DURATION);
    this->animations[WRXAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->m_worldRotation.x, this->m_worldRotation.x, DEFAULT_MOTION_ANIMATION_DURATION);
    this->animations[WRYAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->m_worldRotation.y, this->m_worldRotation.y, DEFAULT_MOTION_ANIMATION_DURATION);
    this->animations[WRZAnimation] = new MotionAnimation(new Linear(), EaseNone, &this->m_worldRotation.z, this->m_worldRotation.z, DEFAULT_MOTION_ANIMATION_DURATION);
    
	this->Reset();
}

GameCamera::~GameCamera() {
	this->position = vec3(0.0f, 0.0f, 0.0f);
	this->rotation = vec3(0.0f, 0.0f, 0.0f);
    this->scale = vec3(1.0f, 1.0f, 1.0f);
    this->m_direction = vec3(0.0f, 0.0f, 0.0f);
    this->m_worldRotation = vec3(0.0f, 0.0f, 0.0f);
	this->m_clearColor = Color4fZero;
    
	for(int i = 0; i < AnimationCount; i++)
		delete this->animations[i];
}

void GameCamera::Initialize(int w, int h) {
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
    
	GLdouble zNear, zFar;
    zNear = 1;
	zFar = 1500;
    
    switch (m_cameraType) {
        case Camera3D:
            
            GLdouble fov, aspect, xmin, xmax, ymin, ymax;
            
            fov = 65.0f; // Field of view.
            aspect = w / h; // Aspect ratio of screen.
            
            ymax = zNear * tan(fov * M_PI / 360.0f);
            ymin = -ymax;
            xmin = ymin * aspect;
            xmax = ymax * aspect;
            
            glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
            
            break;
        case Camera2D:
            
            glOrtho(-w, (GLfloat)w, -h, (GLfloat)h, zNear, zFar);
            
            break;
        default:
            throw "Invalid camera type";
            break;
    };
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GameCamera::ClearScreen() {
	glClearColor(this->m_clearColor.red, this->m_clearColor.green, this->m_clearColor.blue, this->m_clearColor.alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameCamera::Update(float delta) {
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
        
        switch (m_cameraType) {
            case Camera3D:
                
                if (rotation.x || rotation.y || rotation.z) {
                    /*
                     * Rotate the projection matrix on the cameras
                     * local coordinate system.
                     */
                    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
                    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
                    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
                }
                
                /* Camera local coordinate system translation.
                 * If moving the camera in its local coordinate system is desired,
                 * then the translation has to be done before any rotation. If
                 * translation is done after rotating, then the translation will
                 * be executed on world coordinates.
                 */
                glTranslatef(position.x, position.y, position.z);
                
                if (m_worldRotation.x || m_worldRotation.y || m_worldRotation.z)
                {
                    glRotatef(m_worldRotation.x, 1.0f, 0.0f, 0.0f);
                    glRotatef(m_worldRotation.y, 0.0f, 1.0f, 0.0f);
                    glRotatef(m_worldRotation.z, 0.0f, 0.0f, 1.0f);
                }
                
                break;
                
            case Camera2D:
                
                glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
                glScalef(1.0f + (position.z/2.0f), 1.0f + (position.z/2.0f), 1.0f + (position.z/2.0f));
                glTranslatef(position.x, position.y, 0.0f);
                
                break;
                
            default:
                break;
        }
    }

}

void GameCamera::Move(float x, float y, float z) {
	this->Move(x, y, z, DEFAULT_MOTION_ANIMATION_DURATION);
}

void GameCamera::Move(float x, float y, float z, float duration) {
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

void GameCamera::Zoom(float zoom) {
	this->Zoom(zoom, DEFAULT_MOTION_ANIMATION_DURATION);
}

void GameCamera::Zoom(float zoom, float duration) {
	this->animations[ZAnimation]->Reset();
	this->animations[ZAnimation]->SetDuration(duration);
	this->animations[ZAnimation]->SetDesired(zoom + this->position.z);
	this->animations[ZAnimation]->Activate();
}

void GameCamera::Rotate(float x, float y, float z) {
	this->Rotate(x, y, z, DEFAULT_MOTION_ANIMATION_DURATION);
}

void GameCamera::Rotate(float x, float y, float z, float duration) {
	this->animations[RXAnimation]->Reset();
	this->animations[RXAnimation]->SetDuration(duration);
	this->animations[RXAnimation]->SetDesired(x + this->rotation.x);
	this->animations[RXAnimation]->Activate();
	
	this->animations[RYAnimation]->Reset();
	this->animations[RYAnimation]->SetDuration(duration);
	this->animations[RYAnimation]->SetDesired(y + this->rotation.y);
	this->animations[RYAnimation]->Activate();
	
	this->animations[RZAnimation]->Reset();
	this->animations[RZAnimation]->SetDuration(duration);
	this->animations[RZAnimation]->SetDesired(z + this->rotation.z);
	this->animations[RZAnimation]->Activate();
}

void GameCamera::RotateWorld(float x, float y, float z) {
    this->RotateWorld(x, y, z, DEFAULT_MOTION_ANIMATION_DURATION);
}

void GameCamera::RotateWorld(float x, float y, float z, float duration) {
    this->animations[WRXAnimation]->Reset();
    this->animations[WRXAnimation]->SetDuration(duration);
    this->animations[WRXAnimation]->SetDesired(x + this->m_worldRotation.x);
    this->animations[WRXAnimation]->Activate();
    
    this->animations[WRYAnimation]->Reset();
    this->animations[WRYAnimation]->SetDuration(duration);
    this->animations[WRYAnimation]->SetDesired(y + this->m_worldRotation.y);
    this->animations[WRYAnimation]->Activate();
    
    this->animations[WRZAnimation]->Reset();
    this->animations[WRZAnimation]->SetDuration(duration);
    this->animations[WRZAnimation]->SetDesired(z + this->m_worldRotation.z);
    this->animations[WRZAnimation]->Activate();
}

void GameCamera::PointAt(float x, float y, float z) {
    this->PointAt(x, y, z, DEFAULT_MOTION_ANIMATION_DURATION);
}

void GameCamera::PointAt(float x, float y, float z, float duration) {
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

void GameCamera::Reset() {
	this->position = vec3(0.0f, 0.0f, 0.0f);
	this->rotation = vec3(0.0f, 0.0f, 0.0f);
	this->m_direction = vec3(0.0f, 0.0f, 0.0f);
    this->m_worldRotation = vec3(0.0f, 0.0f, 0.0f);
	this->Zoom(INITIAL_Z_POSITION);
}
