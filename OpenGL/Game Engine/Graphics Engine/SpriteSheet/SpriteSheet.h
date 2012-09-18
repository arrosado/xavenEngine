/*
 *  SpriteSheet.h
 *  ARR Game Engine Core
 *
 *  Created by Ariel on 11/25/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */

#ifndef __SPRITE_SHEET_H__
#define __SPRITE_SHEET_H__

#include <Graphics Engine\Image\Image.h>
#include <string>
#include <vector>

class SpriteSheet {
private:
	Image *m_image;
	Size2Df m_spriteSize;
	GLuint m_spacing;
	GLuint m_margin;
	GLuint m_columnsCount;
	GLuint m_rowCount;
	vector<Image*> m_cachedSprites;
	
public:
	SpriteSheet(Image* image, Size2Df spriteSize, GLuint spacing, GLuint margin);
	SpriteSheet(const string& imageName, Size2Df spriteSize, GLuint spacing, GLuint margin, GLenum filter);
	~SpriteSheet();
	inline GLuint GetRows() { return this->m_rowCount; };
	inline GLuint GetColumns() { return this->m_columnsCount; };
	Image* GetSprite(GLuint x, GLuint y);
protected:
	void cacheSprites();
};

#endif /* __SPRITE_SHEET_H__ */