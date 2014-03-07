/*
 *  Texture2D.cpp
 *  Chupaqui
 *
 *  Created by Ariel on 11/21/09.
 *  Copyright 2009 Xaven. All rights reserved.
 */

#include <iostream>
#include <png.h>
#include <Game/Graphics/Texture2D/Texture2D.h>

#define kMaxTextureSize	1024

bool loadPngImage(const char *name, unsigned int &outWidth, unsigned int &outHeight, bool &outHasAlpha, GLubyte **outData) {
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	//int color_type, interlace_type;
	FILE *fp;

	if ((fp = fopen(name, "rb")) == NULL)
		return false;

	/* Create and initialize the png_struct
	 * with the desired error handler
	 * functions. If you want to use the 
	 * default stderr and longjump method,
	 * you can supply NULL for the last
	 * three parameters. We also supply the 
	 * compiler header file version, so 
	 * that we know if the application 
	 * was compiled with a compatible version
	 * of the library. REQUIRED
	 */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL) { 
		fclose(fp);
		return false;
	};

	/* Allocate/initialize the memory
	 * for image information. REQUIRED
	 */
	info_ptr = png_create_info_struct(png_ptr);

	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return false;
	};

	/* Set error handling if you are
	 * using the setjmp/longjmp method
	 * (this is the normal method of 
	 * doing things with libpng).
	 * REQUIRED unless you set up 
	 * your own error handlers in 
	 * the png_create_read_struct()
	 * earlier.
	 */
	if (setjmp(png_jmpbuf(png_ptr))) { 
		/* Free all of the memory associated 
		 * with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(fp);
		/* If we get here, we had a 
		 * problem reading the file.
		 */
		return false;
	}

	/* Set up the output control if 
	 * you are using standard C streams 
	 */
	png_init_io(png_ptr, fp);

	/* If we have already
	 * read some of the signature
	 */
	png_set_sig_bytes(png_ptr, sig_read);

	/* 
	 * If you have enough memory to read 
	 * in the entire image at once, and 
	 * you need to specify only 
	 * transforms that can be controlled
	 * with one of the PNG_TRANSFORM_* 
	 * bits (this presently excludes
	 * dithering, filling, setting
	 * background, and going gamma 
	 * adjustment), then you can read the
	 * entire image (including pixels)
	 * into the info structure with this 
	 * call
	 *
	 * PNG_TRANSFORM_STRIP_16 |
	 * PNG_TRANSFORM_PACKING forces 8 bit
	 * PNG_TRANSFORM_EXPAND forces to
	 *   expand a palette into RGB
	 */
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, png_voidp_NULL);

	outWidth = info_ptr->width;
	outHeight = info_ptr->height;
	switch (info_ptr->color_type) {
	case PNG_COLOR_TYPE_RGBA:
		outHasAlpha = true;
		break;
	case PNG_COLOR_TYPE_RGB:
		outHasAlpha = false;
		break;
	default: 
		std::cout << "Color type " << info_ptr->color_type << " not supported" << std::endl;
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}
	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*outData = (unsigned char*)malloc(row_bytes * outHeight);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	for (unsigned int i = 0; i < outHeight; i++) { 
		/* Note that png is ordered top to 
		 * bottom, but OpenGL expects it bottom
		 * to top so the order should be swapped
		 */
		memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
	}

	/* Clean up after the read, 
	 * and free any memory allocated
	 */
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	/* Close the file */
	fclose(fp);

	/* That's it */
	return true;

}

Texture2D::Texture2D(std::string imageName, GLenum filter) {
	unsigned int			imageWidth, imageHeight;
	GLubyte*				imageData;
	bool					hasAlpha;
	Size2Df					imageSize;
	Texture2DPixelFormat	pixelFormat;
	bool					sizeToFit = false;

	bool success = loadPngImage(imageName.c_str(), imageWidth, imageHeight, hasAlpha, &imageData);

	if (!success) {
		std::cout << "Unable to load png file" << std::endl;

		imageWidth = 0;
		imageHeight = 0;
		imageData = NULL;
		hasAlpha = false;
		imageSize = Size2DfMake(0.0f, 0.0f);
		sizeToFit = false;
		pixelFormat = kTexture2DPixelFormat_Automatic;
		
		this->contentSize = Size2DfMake(0.0f, 0.0f);
		this->height = 0;
		this->width = 0;
		this->maxS = 0;
		this->maxT = 0;
		this->name = 0;
		this->pixelFormat = kTexture2DPixelFormat_Automatic;
		this->textureRatio = Size2DfMake(0.0f, 0.0f);
		
		return;
	}

	std::cout << "Image loaded " << width << " " << height << " alpha " << hasAlpha << std::endl;

	glGenTextures(1, &this->name);
	glBindTexture(GL_TEXTURE_2D, this->name);

	imageSize = Size2DfMake((GLfloat)imageWidth, (GLfloat)imageHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, imageWidth,
            imageHeight, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
            imageData);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
	
	pixelFormat = (hasAlpha) ? kTexture2DPixelFormat_RGBA8888 : kTexture2DPixelFormat_RGB565;

	this->contentSize = imageSize;
	this->width = imageWidth;
	this->height = imageHeight;
	this->pixelFormat = pixelFormat;
	this->maxS = this->contentSize.width / (float)imageWidth;
	this->maxT = this->contentSize.height / (float)imageHeight;
	this->textureRatio.width = 1.0f / (float)imageWidth;
	this->textureRatio.height = 1.0f / (float)imageHeight;

	free(imageData);
}

Texture2D::~Texture2D() {
	if(this->name)
		glDeleteTextures(1, &this->name);
}