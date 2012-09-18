/*
 *  Font.cpp
 *  GameEngine
 *
 *  Created by Ariel Rosado on 12/23/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */

#include <Graphics Engine\Font\Font.h>
#include <Graphics Engine\GraphicsManager.h>

Font::~Font() {
	delete this->image;
}

Font::Font(const string& fontImageName,const string& controlFile, float fontScale, GLenum filter) {
	this->image = new Image(fontImageName, filter);
	this->scale = fontScale;
	this->ParseFont(controlFile);
}

void Font::DrawString(Vector2Df point, const string& text) {

	for(int i = 0; i < text.length(); i++) {
		CharDef *c = charsArray[(int)text.c_str()[i]];

		if (point.x > 0 - (c->width * scale) || 
			point.x < GraphicsManager::Instance()->screen.width ||
			point.y > 0 - (c->height * scale) ||
			point.y < GraphicsManager::Instance()->screen.height) {

			Vector2Df newPoint = Vector2DfMake(point.x + (c->xOffset * c->scale),
											   point.y - (c->height + c->yOffset * c->scale));
			
			c->image->Render(newPoint, Vector2DfMake(scale, scale), 0.0f, false);
		}
		
		point.x += c->xAdvance * scale;
	}
}

int Font::GetWidthForString(const string& text) {
	return 0;
}

int Font::GetHeightForString(const string& text) {
	return 0;
}

void Font::SetColorFilter(float red, float green, float blue, float alpha) {
	this->colorFilter = Color4fMake(red, green, blue, alpha);
}

void Font::SetScale(float newScale) {
	this->scale = newScale;
}

std::map<std::string, float> GetLineData(const string& line) {
	map<string, float> data;

	std::vector<std::string> tokens;
	std::stringstream ss(line);
	
	std::string item;
	while(std::getline(ss, item, ' ')) {
		tokens.push_back(item);
	}

	for(int i = 0; i < tokens.size(); i++)
	{
		std::string key;
		std::string value;
		std::stringstream tss(tokens[i]);
		
		// Check if there is a = character.
		while(std::getline(tss, item, '=')) {
			if (key.size() == 0)
				key = item;	
			else 
				value = item;
		}

		float val = 0.0;
		if (value.size())
			sscanf(value.c_str(), "%f", &val);

		data[key] = val;
	}

	return data;
}

void Font::ParseFont(const string& controlFile) {

#define MAX_REC_LEN 1024
	vector<string> *lines = new vector<string>();
	char line[MAX_REC_LEN];
	/* Initialize the structure to 
	 * hold the geometry, texture 
	 * and color vertices.
	 */
	Vector2Df textureOffset;

	FILE *f = fopen(controlFile.c_str(), "rt");

	if (f) {
		/* if the file was successfully opened, read all its 
		 * content to the contents variable.
		 */
		while(fgets(line, sizeof line, f)) {
			lines->push_back(line);

			std::string page = "page";
			std::string common = "common";
			std::string info = "info";
			std::string charsCount = "chars count";
			std::string charsDef = "char";
			
			if (strncmp(line, info.c_str(), strlen(info.c_str())) == 0) {
			
			} else if (strncmp(line, common.c_str(), strlen(common.c_str())) == 0) {

			} else if (strncmp(line, page.c_str(), strlen(page.c_str())) == 0) {
				/* Look for file="image.png" */
				
				// find file on the current line and load an image from that.


			} else if (strncmp(line, charsCount.c_str(), strlen(charsCount.c_str())) == 0) {
				
			} else if (strncmp(line, charsDef.c_str(), strlen(charsDef.c_str())) == 0) {
				/* Parse the current line and create a new CharDef */
				CharDef *c = new CharDef(image, scale);

				map<string, float> data = GetLineData(line);

				c->m_iCharID = data["id"];
				c->width = data["width"] - 1.0f; // Substracted one just to work with some coords errors.
				c->height = data["height"];
				c->xOffset = data["xoffset"];
				c->yOffset = data["yoffset"];
				c->xAdvance = data["xadvance"];
				c->x = data["x"];
				c->y = image->GetHeight() - (data["y"] + data["height"]) + 0.5f; // Invert the Y coordinate to match OpenGL's
				c->scale = scale;

				c->image = image->SubImageInRect(Rect2DfMake(c->x,  c->y, c->width, c->height));

				/* Add the CharDef returned to the charArray */
				charsArray[c->m_iCharID] = c;
			}

		}
		/* Close the file */
		fclose(f);
	} else {
		perror(controlFile.c_str());
	}
}

//void Font::ParseKerningCapacity(const string& line) {
//	
//}
//
//void Font::ParseKerningEntry(const string& line) {
//	
//}
//
//int Font::KerningAmountForFirst(char first, char second) {
//	return 0;
//}


