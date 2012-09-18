#ifndef __GRAPHICS_MANAGER_H__
#define __GRAPHICS_MANAGER_H__

#include <vector>
#include <string>
#include <Graphics Engine\Texture2D\Texture2D.h>
#include <IComponent.h>
#include <Graphics Engine\IGameCamera.h>
#include <Graphics Engine\GraphicsCommon.h>

#define kMax_Images 250
#define kMax_Textures 20

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
	GLuint textureIndices[kMax_Textures][kMax_Images];
	GLuint texturesToRender[kMax_Textures];
	GLuint imageCountForTexture[kMax_Images];
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