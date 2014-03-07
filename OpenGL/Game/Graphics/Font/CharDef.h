/*
 *  CharDef.h
 *  GameEngine
 *
 *  Created by Ariel Rosado on 12/23/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */
#ifndef __CHAR_DEF_H__
#define __CHAR_DEF_H__

#include <Game/Graphics/Image/Image.h>

class CharDef {
public:
	// ID of the character
	int m_iCharID;
	// X location on the spritesheet
	float x;
	// Y location on the spritesheet
	float y;
	// Width of the character image
	float width;
	// Height of the character image
	float height;
	// The X amount the image should be offset when drawing the image
	float xOffset;
	// The Y amount the image should be offset when drawing the image
	float yOffset;
	// The amount to move the current position after drawing the character
	float xAdvance;
	// The image containing the character
	Image *image;
	// Scale to be used when rendering the character
	float scale;
	
	CharDef(Image* pImage, float fontScale);
	~CharDef();
};

#endif /* __CHAR_DEF_H__ */