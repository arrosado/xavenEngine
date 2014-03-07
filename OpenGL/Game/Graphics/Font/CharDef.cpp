/*
 *  CharDef.cpp
 *  GameEngine
 *
 *  Created by Ariel Rosado on 12/23/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */

#include <Game/Graphics/Font/CharDef.h>

CharDef::CharDef(Image* pImage, float fontScale) {
	this->image = pImage;
	this->scale = fontScale;
}

CharDef::~CharDef() {
	this->image = NULL;
}
