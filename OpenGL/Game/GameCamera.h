//
//  IGameCamera.h
//  OpenGL
//
//  Created by Ariel Rosado Rivera on 4/5/14.
//  Copyright (c) 2014 Ariel Rosado. All rights reserved.
//

#ifndef GameCamera_H
#define GameCamera_H

#include <Game/GameObject.h>
#include <Game/Graphics/Motion.h>
#include <Game/Graphics/GraphicsCommon.h>

enum CameraType
{
    Camera2D,
    Camera3D
};

class GameCamera : public GameObject
{
public:
    GameCamera(CameraType cameraType);
    ~GameCamera();
    
    void Initialize(int w, int h);
    void ClearScreen();
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
    
private:
    CameraType m_cameraType;
    vec3 m_direction;
    Color4f m_clearColor;
    Size2Df m_screenSize;
    
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
	
protected:
};



#endif
