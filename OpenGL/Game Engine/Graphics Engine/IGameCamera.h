//
//  IGameCamera.h
//  ARR Game Engine Core
//
//  Created by Ariel Rosado on 3/13/11.
//  Copyright 2011 Xaven. All rights reserved.
//


#ifndef __GAME_CAMERA_H__
#define __GAME_CAMERA_H__

#include <Graphics Engine\GraphicsCommon.h>

typedef struct _Screen {
	Size2Df size;
	GameScreenOrientation orientation;
} Screen;

struct IGameCamera {
	virtual void InitializeOpenGL(int w, int h) = 0;
	virtual void PrepareNewFrame() = 0;
	virtual void Update(float delta) = 0;
	virtual void Move(float x, float y, float z) = 0;
	virtual void Move(float x, float y, float z, float duration) = 0;
	virtual void Zoom(float zoom) = 0;
	virtual void Zoom(float zoom, float duration) = 0;
	virtual void Rotate(float x, float y, float z) = 0;
	virtual void Rotate(float x, float y, float z, float duration) = 0;
	virtual void PointAt(float x, float y, float z) = 0;
	virtual void PointAt(float x, float y, float z, float duration) = 0;
	virtual void Reset() = 0;
	virtual ~IGameCamera() {};
};

// Game Camera Factory Method.
struct IGameCamera* CreateGameCamera();

#endif
