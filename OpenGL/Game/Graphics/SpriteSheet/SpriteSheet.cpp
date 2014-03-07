#include <Game/Graphics/SpriteSheet/SpriteSheet.h>

SpriteSheet::SpriteSheet(Image* image, Size2Df spriteSize, GLuint spacing, GLuint margin) {
	
	this->m_image = image;
	this->m_spriteSize = spriteSize;
	this->m_spacing = spacing;
	this->m_margin = margin;
	
	this->cacheSprites();
}

SpriteSheet::SpriteSheet(const string& imageName, Size2Df spriteSize, GLuint spacing, GLuint margin, GLenum filter) {
    
	this->m_image = new Image(imageName, filter);
	this->m_spriteSize = spriteSize;
	this->m_spacing = spacing;
	this->m_margin = margin;
	
	this->cacheSprites();
}

SpriteSheet::~SpriteSheet() {
    
}

void SpriteSheet::cacheSprites() {
    
	// Calculate how many sprites there are horizontally and vertically given their size and margin.
	m_columnsCount = ((m_image->GetWidth() + m_margin) / (m_spriteSize.width + m_spacing));
	m_rowCount = ((m_image->GetHeight() + m_margin) / (m_spriteSize.height + m_spacing));
	
	// Initialize the structure to hold the geometry, texture and color vertices.
	Vector2Df textureOffset;
	
	// Calculate and cache a texturedColoredQuad for each sprite in the sprite sheet.
	//for(GLuint row = 0; row < this->m_rowCount; row++) {
	for (int row = m_rowCount -1; row >= 0; row--) {
		for(int column = 0; column < m_columnsCount; column++) {
			
			// Based on the location within the spritesheet of the sprite we need, get a pixel point within the 
			// texture where the sprite texture will beign.
			Vector2Df texturePoint  = Vector2DfMake((column * (m_spriteSize.width + m_spacing) + m_margin), (row * (m_spriteSize.height + m_spacing) + m_margin));
			
			// Now make a rectangle structure that contains the position and dimensions of the image we want to grab
			// for this tile image.
			textureOffset.x = m_image->GetTextureOffset().x * m_image->GetTexture()->width + texturePoint.x;
			textureOffset.y = m_image->GetTextureOffset().y * m_image->GetTexture()->height + texturePoint.y;
			Rect2Df tileImageRect = Rect2DfMake(textureOffset.x, textureOffset.y, m_spriteSize.width, m_spriteSize.height);
			
			Image *tileImage = m_image->SubImageInRect(tileImageRect);
            
			m_cachedSprites.push_back(tileImage);
		}
	}
}

Image* SpriteSheet::GetSprite(GLuint x, GLuint y) {
	
	// Check to make sure that the requested coordinates are within the bounds of the spriteSheet
	if ( x > this->m_columnsCount-1 || y > this->m_rowCount-1 )
		return NULL;
	
	// Calculate the location within the cached sprites
	int index = (this->m_columnsCount * y) + x;
	
	// Return the image created
	return this->m_cachedSprites[ index ];
}
