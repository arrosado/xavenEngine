/*
 *  Image.cpp
 *  ARR Game Engine Core
 *
 *  Created by Ariel on 11/23/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */

#include <Game/Graphics/Image/Image.h>

Image::Image(const string& name, GLenum filter) {
    
	this->InitializeImage(name, filter);
	this->imageSize.width = this->texture->contentSize.width;
	this->imageSize.height = this->texture->contentSize.height;
	this->originalImageSize = this->imageSize;
	this->textureSize.width = this->texture->maxS;
	this->textureSize.height = this->texture->maxT;
	this->textureOffset = Vector2DfZero;
	this->imageDetails = NULL;
	this->InitializeImageDetails();
	
}

Image::Image(const string& name, GLenum filter, Rect2Df subTexture) {
    
	this->subImageRectangle = subTexture;
	this->InitializeImage(name, filter);
	this->imageSize = subTexture.size;
	this->originalImageSize = this->imageSize;
	
	this->textureOffset.x = this->textureRatio.width * subTexture.origin.x;
	this->textureOffset.y = this->textureRatio.height * subTexture.origin.y;
	
	this->textureSize.width = (this->textureRatio.width * this->imageSize.width) + this->textureOffset.x;
	this->textureSize.height = (this->textureRatio.height * this->imageSize.height) + this->textureOffset.y;
	
	this->imageDetails = NULL;
	this->InitializeImageDetails();
	
}

Image::~Image() {
	
	if (imageDetails) {
		if (imageDetails->texturedColoredQuad)
			free(imageDetails->texturedColoredQuad);
		free(imageDetails);
	}
	
	GraphicsManager::Instance()->RemoveTexture(this->imageFileName);		
}


Image* Image::SubImageInRect(Rect2Df rect) {
    
	Image *subImage = new Image(this->imageFileName, this->minMagFilter, rect);
	subImage->scale = this->scale;
    subImage->color = this->color;
    subImage->flipVertically = this->flipVertically;
    subImage->flipHorizontally = this->flipHorizontally;
	subImage->rotation = this->rotation;
	subImage->rotationPoint = this->rotationPoint;
	return subImage;
	
}

Image* Image::ImageDuplicate() {
    
	Image *imageCopy = this->SubImageInRect(this->subImageRectangle);
	return imageCopy;
}

void Image::InitializeImage(const string& name, GLenum filter) {
    
	this->imageFileName = name;
	this->texture = GraphicsManager::Instance()->GenTexture(name, filter);
	this->textureName = this->texture->name;
	this->fullTextureSize.width = this->texture->width;
	this->fullTextureSize.height = this->texture->height;
	this->textureRatio.width = this->texture->textureRatio.width;
	this->textureRatio.height = this->texture->textureRatio.height;
	this->color = Color4fMake(1.0f, 1.0f, 1.0f, 1.0f);
	this->rotationPoint = Vector3DfZero;
	this->minMagFilter = filter;
	
	// Initialize properties with defaults.
	this->rotation = Vector3DfZero;
	this->scale = Size2DfZero;
	this->flipHorizontally = false;
	this->flipVertically = false;
	
}

void Image::InitializeImageDetails() {
    
	
    // Set up a TexturedColoredQuad structure which is going to hold the origial informtion
    // about our image.  This structure will never change, but will be used when performing
    // transforms on the image with the results being loaded into the RenderManager using this
    // images render index
	if (!this->imageDetails) {
		this->imageDetails = (ImageDetails*)calloc(1, sizeof(ImageDetails));
		this->imageDetails->texturedColoredQuad = (TexturedColoredQuad*)calloc(1, sizeof(TexturedColoredQuad));
	}
    
    // Set up the geometry for the image
    this->imageDetails->texturedColoredQuad->vertex1.geometryVertex = Vector2DfMake(0.0f, 0.0f);
    this->imageDetails->texturedColoredQuad->vertex2.geometryVertex = Vector2DfMake(this->imageSize.width, 0.0f);
    this->imageDetails->texturedColoredQuad->vertex3.geometryVertex = Vector2DfMake(0.0f, this->imageSize.height);
    this->imageDetails->texturedColoredQuad->vertex4.geometryVertex = Vector2DfMake(this->imageSize.width, this->imageSize.height);

    // Set up the texture coordinates for the image as is.  If a subimage is needed then
    // the getSubImage selector can be used to create a new image with the adjusted
    // texture coordinates.  The texture inside a Texture2D object is upside down, so the
    // texture coordinates need to account for that so the image will show the right way up
    // when rendered.
#ifdef ROTATE_IMAGE
	/* This is the rotated image version */
    this->imageDetails->texturedColoredQuad->vertex1.textureVertex = Vector2DfMake(this->textureOffset.x, this->textureSize.height);
    this->imageDetails->texturedColoredQuad->vertex2.textureVertex = Vector2DfMake(this->textureSize.width, this->textureSize.height);
    this->imageDetails->texturedColoredQuad->vertex3.textureVertex = Vector2DfMake(this->textureOffset.x, this->textureOffset.y);
    this->imageDetails->texturedColoredQuad->vertex4.textureVertex = Vector2DfMake(this->textureSize.width, this->textureOffset.y);
#else
	/* This is the normal image version */
	this->imageDetails->texturedColoredQuad->vertex1.textureVertex = Vector2DfMake(this->textureOffset.x, this->textureOffset.y);
	this->imageDetails->texturedColoredQuad->vertex2.textureVertex = Vector2DfMake(this->textureSize.width, this->textureOffset.y);
	this->imageDetails->texturedColoredQuad->vertex3.textureVertex = Vector2DfMake(this->textureOffset.x, this->textureSize.height);
	this->imageDetails->texturedColoredQuad->vertex4.textureVertex = Vector2DfMake(this->textureSize.width, this->textureSize.height);
#endif

    // Set up the vertex colors.  To start with these are all 1.0's
    this->imageDetails->texturedColoredQuad->vertex1.vertexColor = 
    this->imageDetails->texturedColoredQuad->vertex2.vertexColor = 
    this->imageDetails->texturedColoredQuad->vertex3.vertexColor = 
    this->imageDetails->texturedColoredQuad->vertex4.vertexColor = this->color;    
	
    // Set the imageDetails textureName
    this->imageDetails->textureName = this->textureName;
    
    // Mark the image as dirty which means that the images matrix will be transformed
    // with the results loaded into the images IVA pointer
    this->dirty = true;
}

void Image::Render() {
	
	// Update the color of the image before it gets copied to the render manager
	this->imageDetails->texturedColoredQuad->vertex1.vertexColor = 
	this->imageDetails->texturedColoredQuad->vertex2.vertexColor =
	this->imageDetails->texturedColoredQuad->vertex3.vertexColor =
	this->imageDetails->texturedColoredQuad->vertex4.vertexColor = this->color;
	
	// Add this image to the render queue.  This will cause this image to be rendered the next time
    // the renderManager is asked to render.  It also copies the data over to the image renderer
    // IVA.  It is this data that is changed next by applying the images matrix
	GraphicsManager::Instance()->AddImageDetailsToRenderQueue(this->imageDetails);
    
	// If the images point, scale or rotation are changed, it means we need to adjust the 
    // images matrix and transform the vertices.  If dirty is set we also check to see if it is 
    // necessary to adjust the rotation and scale.  If they are 0 then nothing needs to
    // be done and we can save some cycles.
    if (this->dirty) {
        // Load the identity matrix before applying transforming the matrix for this image.  The
        // order in which the transformations are applied is important.
        loadIdentityMatrix(this->matrix);
		
        // Translate the position of the image first
        translateMatrix(this->matrix, this->point);
		
        // If this image has been configured to be flipped vertically or horizontally
        // then set the scale for the image to -1 for the appropriate axis and then translate 
        // the image so that the images origin is rendered in the correct place
        if(this->flipVertically) {
            scaleMatrix(this->matrix, Size2DfMake(1, -1));
            translateMatrix(this->matrix, Vector3DfMake(0, (-this->imageSize.height * this->scale.height), 0));
        }
		
        if(this->flipHorizontally) {
            scaleMatrix(this->matrix, Size2DfMake(-1, 1));
            translateMatrix(this->matrix, Vector3DfMake((-this->imageSize.width * this->scale.width), 0, 0));
        }
        
		// No point in calculating a rotation matrix if there is no rotation been set
        if(this->rotation.z != 0)
            rotateMatrix(this->matrix, this->rotationPoint, this->rotation.z);
        
        // No point in calculcating scale if no scale has been set.
		if(this->scale.width != 1.0f || this->scale.height != 1.0f)
            scaleMatrix(this->matrix, this->scale);
        
        // Transform the images matrix based on the calculations done above
        transformMatrix(this->matrix, this->imageDetails->texturedColoredQuad, this->imageDetails->texturedColoredQuadIVA);
        
        // Mark the image as now clean
		this->dirty = false;
	}	
}

void Image::Render(Vector3Df point) {
	this->Render(point, this->scale, this->rotation);
}

void Image::Render(Vector3Df point, Size2Df scale, Vector3Df rotation) {
	this->point = point;
	this->scale = scale;
	this->rotation = rotation;
	this->dirty = true;
	this->Render();
}

void Image::Render(bool centered) {
	this->dirty = true;
	this->Render(Vector3DfMake(this->point.x, this->point.y, this->point.z), Size2DfZero, Vector3DfMake(this->rotation.x, this->rotation.y, this->rotation.z), centered);
}

void Image::Render(Vector3Df point, bool centered) {
	this->Render(point, this->scale, this->rotation, centered);
}

void Image::Render(Vector3Df point, Size2Df scale, Vector3Df rotation, bool centered) {
	
	this->scale = scale;
	this->rotation = rotation;
    if (centered) {
        this->rotationPoint.x = ((this->imageSize.width * scale.width) / 2);
        this->rotationPoint.y = ((this->imageSize.height * scale.height) / 2);
        this->rotationPoint.z = point.z;
    }
	this->point.x = point.x - ((this->imageSize.width * scale.width) / 2);
	this->point.y = point.y - ((this->imageSize.height * scale.height) / 2);
	this->dirty = true;
	this->Render();
	
}

