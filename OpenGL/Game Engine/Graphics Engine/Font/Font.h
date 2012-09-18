/*
 *  AngelCodeFont.h
 *  GameEngine
 *
 *  Created by Ariel Rosado on 12/23/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */
/*
 Whilst looking for how to render Bitmap Fonts I found this page http://www.angelcode.com/products/bmfont/
 
 They have an application which can take a true type font and turn it into a spritesheet and control file.
 With these files you can then render a string to the screen using the font generated.  I develop of a Mac of
 course so was disappointed that this app was for Windows.  Fear not, luckily a tool has been developed by
 Kev Glass over at http://slick.cokeandcode.com/index.php for use with his open source Java 2D game library.
 The app is called Hiero. 
 
 This generates the necessary image file and control file in the format defined on the AngelCode Font website.
 Hiero has also been updated recently to handle Unicode fonts as well accoriding to the forums and this new 
 versions webstart can be found here http://www.n4te.com/hiero/hiero.jnlp 
 
 Being Java this version will work on any platform :o)
 
 This implementation has been tested against the output from Hiero V2
 */
#ifndef __FONT_H__
#define __FONT_H__

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <Graphics Engine\Image\Image.h>
#include <Graphics Engine\Font\CharDef.h>
#include <Graphics Engine\GraphicsCommon.h>

#define MAX_CHARS 256
#define kMaxCharsInFont 512

class Font {
public:
	// The image which contains the bitmap font
	Image *image;
	// The characters building up the font
	CharDef	*charsArray[MAX_CHARS];
	// The height of a line
	GLuint  lineHeight;
	// Color filter = Red, Green, Blue, Alpha
	Color4f	colorFilter;
	// The scale to be used when rendering the font
	GLfloat	scale;
	// Should kerning be used if available
	//bool useKerning;
	// Kerning Dictionary
	//std::map<void *, void *> *KerningDictionary;
	// Vertex arrays
	
	Font(const string& fontImageName,const string& controlFile, float fontScale, GLenum filter);
	
	~Font();
	
	void DrawString(Vector2Df point, const string& text);
	int GetWidthForString(const string& text);
	int GetHeightForString(const string& text);
	void SetColorFilter(float red, float green, float blue, float alpha);
	void SetScale(float newScale);
private:
	void ParseFont(const string& controlFile);
	void ParseCharacterDefinition(const string& line, CharDef* charDef);
	//void ParseKerningCapacity(const string& line);
	//void ParseKerningEntry(const string& line);
	//int KerningAmountForFirst(char first, char second);
	void InitVertexArrays(int totalQuads);
};

#endif /* __FONT_H__ */