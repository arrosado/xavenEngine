#ifndef __GRAPHICS_MANAGER_H__
#define __GRAPHICS_MANAGER_H__

#include <vector>
#include <string>
#include <Game/IComponent.h>
#include <Game/Graphics/IGameCamera.h>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/Texture2D/Texture2D.h>

class GraphicsManager : public IComponent {
public:
	static GraphicsManager* Instance() {
		return &m_GraphicsManagerInstance;
	}
	
	Size2Df screen;
    IGameCamera *m_camera;
	void Init(int argc, ...); 
	
	
	// Texture management methods.
	Texture2D* GenTexture(const std::string name, GLenum filter);
	bool RemoveTexture(const std::string name);
	bool RemoveAllTextures();
	
	// Image management methods.
	void AddImageDetailsToRenderQueue(ImageDetails *imageDetails);
	void AddTexturedColoredQuadToRenderQueue(TexturedColoredQuad *TCQ, GLuint texture);
	void RenderImages();
	
	void ResizeView(int w, int h);
	void PrepareNewFrame();
	void RenderNewFrame();
	void HandleEvents();
    void Update(double delta);
	
	void Cleanup();
	void Quit() {}
	
private:
	static GraphicsManager m_GraphicsManagerInstance;

	// Image management queue variables.
	TexturedColoredVertex *iva;
	GLushort *ivaIndices;
	GLuint textureIndices[MAX_TEXTURES_PER_APP][MAX_IMAGES_PER_FRAME];
	GLuint texturesToRender[MAX_TEXTURES_PER_APP];
	GLuint imageCountForTexture[MAX_IMAGES_PER_FRAME];
	GLuint renderTextureCount;
	GLushort ivaIndex;
	
	TextureCollection textures;
	
	// Image management queue methods
	void CopyImageDetails(ImageDetails *imageDetails);
	void AddToTextureList(GLuint textureName);
	
protected:
	GraphicsManager() {};
};

#endif /* __GRAPHICS_MANAGER_H__ */