/*
 *  Image.h
 *  ARR Game Engine Core
 *
 *  Created by Ariel on 11/23/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/GraphicsManager.h>
#include <Game/Graphics/Texture2D/Texture2D.h>
#include <string>


using namespace std;

class Image {
private:
	////////////////// Image iVars
	string imageFileName;
	string imageFileType;
	Texture2D *texture;
	
	Size2Df fullTextureSize;
	Size2Df textureSize;
	Size2Df imageSize;
	Size2Df originalImageSize;
	
	Size2Df maxTextureSize;
	Vector2Df textureOffset;
	Vector3Df rotation;
	Size2Df scale;
	bool flipHorizontally;
	bool flipVertically;
	GLuint IVAIndex;

public:
	// This is beign used by the ParticleEmitter.
	GLuint textureName;
private:
	Vector3Df point;
	Vector3Df rotationPoint;
	Color4f color;
	bool dirty;
	GLenum minMagFilter;
	Rect2Df subImageRectangle;
	Size2Df textureRatio;
	
	//////////////////// Render Information
	ImageDetails *imageDetails;
	float matrix[9];
	
public:
	Image(const string& name, GLenum filter);
	Image(const string& name, GLenum filter, Rect2Df subTexture);
	~Image();
	
	//inline GLuint GetRotation() { return this->rotation; }
	inline GLuint GetWidth() { return this->imageSize.width; } 
	inline GLuint GetHeight() { return this->imageSize.height; }
	inline Texture2D* GetTexture() { return this->texture; } 
	inline Vector2Df GetTextureOffset() { return this->textureOffset; } 
	
	inline void SetImageSizeToRender(Size2Df size) {
		// If the width or height passed in is < 0 or > 100 then log an error
		if (size.width < 0 || size.width > 100 || size.height < 0 || size.height > 100) {
			printf("ERROR - Image: Illegal provided to setImageSizeToRender 'width=%f, height=%f'", size.width, size.height);
			return;
		}
		
		// Using the original size of this image, calculate the new image width based on the
		// percentage provided
		this->imageSize.width = (this->originalImageSize.width / 100) * size.width;
		this->imageSize.height = (this->originalImageSize.height / 100) * size.height;
		
		// Calculate the width and height of the sub region this image is going to use.
		this->textureSize.width = (this->textureRatio.width * this->imageSize.width) + this->textureOffset.x;
		this->textureSize.height = (this->textureRatio.height * this->imageSize.height) + this->textureOffset.y;
		
		// Initialize the image details.  This will recalculate the images geometry and texture
		// coordinates in the imageDetails structure.
		this->InitializeImageDetails();
	}

	inline void SetFlip(bool vertically, bool horizontally) {
		this->flipVertically = vertically;
		this->flipHorizontally = horizontally;
		this->dirty = true;	
	}

	inline void SetRotation(Vector3Df rotation) {
		this->rotation = rotation;
		this->dirty = true;
	}
	
	Image* SubImageInRect(Rect2Df rect);
	Image* ImageDuplicate();
	
	void InitializeImage(const string& name, GLenum filter);
	void InitializeImageDetails();
	
	void Render();
	void Render(Vector3Df point);
	void Render(Vector3Df point, Size2Df scale, Vector3Df rotation);
	void Render(bool centered);
	void Render(Vector3Df point, bool centered);
	void Render(Vector3Df point, Size2Df scale, Vector3Df rotation, bool centered);
	
};

#endif /* __IMAGE_H__ */
