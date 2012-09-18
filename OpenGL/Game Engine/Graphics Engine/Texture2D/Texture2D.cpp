/*
 *  Texture2D.cpp
 *  Chupaqui
 *
 *  Created by Ariel on 11/21/09.
 *  Copyright 2009 Xaven. All rights reserved.
 */

#include <iostream>
#include <Graphics Engine\libpng\png.h>
#include <Graphics Engine\Texture2D\Texture2D.h>

#define kMaxTextureSize	1024

#ifdef __APPLE__

Texture2D::Texture2D(string imageName, GLenum filter) {
	
	CGDataProviderRef		provider;
	CFStringRef				name;
	CFURLRef				url;
	CFBundleRef				mainBundle = CFBundleGetMainBundle();
	uint					imageWidth, imageHeight, i;
	CGContextRef			context = NULL;
	void*					data = NULL;
	CGColorSpaceRef			colorSpace;
	void*					tempData;
	unsigned int*			inPixel32;
	unsigned short*			outPixel16;
	bool					hasAlpha;
	CGImageAlphaInfo		info;
	CGAffineTransform		transform = CGAffineTransformIdentity;
	CGSize					imageSize;
	Texture2DPixelFormat	pixelFormat;
	CGImageRef				image;
	bool					sizeToFit = false;
	
	// Get the URL to the bundle resource.
	name = CFStringCreateWithCString(NULL, imageName.c_str(), kCFStringEncodingUTF8);
	url = CFBundleCopyResourceURL(mainBundle, name, CFSTR("png"), NULL);
    
	// Create the data provider object
	provider = CGDataProviderCreateWithURL(url);
	
	// Create the image object from that provider.
	image = CGImageCreateWithPNGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
	
	
	if(image == NULL) 
		throw "ERROR - Texture2D: Image is Null";
	
    // Get the alpha information from the image and check to see if the image does contain
    // alpha information
	info = CGImageGetAlphaInfo(image);
	hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || 
                (info == kCGImageAlphaPremultipliedFirst) || 
                (info == kCGImageAlphaLast) || 
                (info == kCGImageAlphaFirst) ? true : false);
    
    // Check to see what pixel format the image is using
	if(CGImageGetColorSpace(image)) {
		if(hasAlpha)
			pixelFormat = kTexture2DPixelFormat_RGBA8888;
		else
			pixelFormat = kTexture2DPixelFormat_RGB565;
	} else  //NOTE: No colorspace means a mask image
		pixelFormat = kTexture2DPixelFormat_A8;
	
	
	imageSize = CGSizeMake(CGImageGetWidth(image), CGImageGetHeight(image));
	transform = CGAffineTransformIdentity;
	
	imageWidth = imageSize.width;
	
	if((imageWidth != 1) && (imageWidth & (imageWidth - 1))) {
		i = 1;
		while((sizeToFit ? 2 * i : i) < imageWidth)
			i *= 2;
		imageWidth = i;
	}
	imageHeight = imageSize.height;
	if((imageHeight != 1) && (imageHeight & (imageHeight - 1))) {
		i = 1;
		while((sizeToFit ? 2 * i : i) < imageHeight)
			i *= 2;
		imageHeight = i;
	}
	while((imageWidth > kMaxTextureSize) || (imageHeight > kMaxTextureSize)) {
		imageWidth /= 2;
		imageHeight /= 2;
		transform = CGAffineTransformScale(transform, 0.5, 0.5);
		imageSize.width *= 0.5;
		imageSize.height *= 0.5;
	}
	
	switch(pixelFormat) {		
		case kTexture2DPixelFormat_RGBA8888:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			data = malloc(imageHeight * imageWidth * 4);
			context = CGBitmapContextCreate(data, imageWidth, imageHeight, 8, 4 * imageWidth, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
			break;
		case kTexture2DPixelFormat_RGB565:
			colorSpace = CGColorSpaceCreateDeviceRGB();
			data = malloc(imageHeight * imageWidth * 4);
			context = CGBitmapContextCreate(data, imageWidth, imageHeight, 8, 4 * imageWidth, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
			CGColorSpaceRelease(colorSpace);
			break;
			
		case kTexture2DPixelFormat_A8:
			data = malloc(imageHeight * imageWidth);
			context = CGBitmapContextCreate(data, imageWidth, imageHeight, 8, imageWidth, NULL, kCGImageAlphaOnly);
			break;				
		default:
			throw "Internal Inconsistency Exception Invalid pixel format";
	}
	
	
	CGContextClearRect(context, CGRectMake(0, 0, imageWidth, imageHeight));
	CGContextTranslateCTM(context, 0, imageHeight - imageSize.height);
	
	if(!CGAffineTransformIsIdentity(transform))
		CGContextConcatCTM(context, transform);
	CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image), CGImageGetHeight(image)), image);
    
	//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
	if(pixelFormat == kTexture2DPixelFormat_RGB565) {
		tempData = malloc(imageHeight * imageWidth * 2);
		inPixel32 = (unsigned int*)data;
		outPixel16 = (unsigned short*)tempData;
		for(i = 0; i < imageWidth * imageHeight; ++i, ++inPixel32)
			*outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);
		free(data);
		data = tempData;
	}
	
	GLint saveName;
	glGenTextures(1, &this->name);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
	glBindTexture(GL_TEXTURE_2D, this->name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	switch(pixelFormat) {
		case kTexture2DPixelFormat_RGBA8888:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case kTexture2DPixelFormat_RGB565:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
			break;
		case kTexture2DPixelFormat_A8:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, imageWidth, imageHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			throw "Inconsistency Exception Binding texture";
	}
	glBindTexture(GL_TEXTURE_2D, saveName);
	
	this->contentSize = imageSize;
	this->width = imageWidth;
	this->height = imageHeight;
	this->pixelFormat = pixelFormat;
	this->maxS = this->contentSize.width / (float)imageWidth;
	this->maxT = this->contentSize.height / (float)imageHeight;
	this->textureRatio.width = 1.0f / (float)imageWidth;
	this->textureRatio.height = 1.0f / (float)imageHeight;
	
	
	CFRelease(name);
	CFRelease(url);
	mainBundle = NULL;
	CGDataProviderRelease(provider);
	CGContextRelease(context);
	CFRelease(image);
	free(data);	
}

#elif _WIN32


bool loadPngImage(const char *name, unsigned int &outWidth, unsigned int &outHeight, bool &outHasAlpha, GLubyte **outData) {
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
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

#endif

Texture2D::~Texture2D() {
	if(this->name)
		glDeleteTextures(1, &this->name);
}