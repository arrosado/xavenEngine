/*
 *  Texture2D.h
 *
 *  Created by Ariel on 11/21/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */
#ifndef __TEXTURE_2D__
#define __TEXTURE_2D__

#include <Graphics Engine\GraphicsCommon.h>
#include <string>

typedef enum {
	kTexture2DPixelFormat_Automatic = 0,
	kTexture2DPixelFormat_RGBA8888,
	kTexture2DPixelFormat_RGB565,
	kTexture2DPixelFormat_A8,
} Texture2DPixelFormat;

class Texture2D {
public:
	GLuint	name;
	Size2Df	contentSize;
	GLuint	width;
	GLuint	height;
	GLfloat maxS;
	GLfloat maxT;
	Size2Df	textureRatio;
	Texture2DPixelFormat pixelFormat;
	
public:
	Texture2D(std::string imageName, GLenum filter);
	~Texture2D();
};

#endif /* __TEXTURE_2D__ */