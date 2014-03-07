#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/GraphicsManager.h>
#include <Game/Input/InputManager.h>

GraphicsManager GraphicsManager::m_GraphicsManagerInstance;

void GraphicsManager::Init(int argc, ...) {
    
	va_list l;
	va_start(l, argc);

    Screen screen;
	screen.size.width = va_arg(l, int);// width;
	screen.size.height = va_arg(l, int);// height;

	va_end(l);

	this->m_camera = CreateGameCamera();
	this->m_camera->InitializeOpenGL(screen.size.width, screen.size.height);
	
    this->iva = (TexturedColoredVertex*)calloc(MAX_IMAGES_PER_FRAME, sizeof(TexturedColoredQuad));
	this->ivaIndices = (GLushort*)calloc(MAX_IMAGES_PER_FRAME * 6, sizeof(GLushort));
	
	this->ivaIndex = 0;
	this->renderTextureCount = 0;
	memset(imageCountForTexture, 0, MAX_IMAGES_PER_FRAME);
}

void GraphicsManager::Cleanup() {
    
	if (this->iva)
		free(this->iva);
	
	if (this->ivaIndices)
		free(this->ivaIndices);
	
	if (this->textures.size() > 0)
		this->RemoveAllTextures();
    
    if (this->m_camera)
        delete this->m_camera;
	
}

Texture2D* GraphicsManager::GenTexture(const std::string name, GLenum filter) {
	
	for (unsigned int i = 0; i < this->textures.size(); i++) 
		if (strcmp(this->textures[i].name.c_str(), name.c_str()) == 0) {
			this->textures[i].retainCount++;
			return this->textures[i].texture;
		}
	
	Texture2D *t = new Texture2D(name, filter);
	Texture texture;
	texture.name = name;
	texture.texture = t;
	texture.retainCount = 0;
	texture.retainCount++;
    
	this->textures.push_back(texture);
	return this->textures.back().texture;
}

bool GraphicsManager::RemoveTexture(const std::string name) {
	
	TextureCollection::iterator it;
	
	for (it = this->textures.begin(); it < this->textures.end(); it++) 
		if(strcmp((*it).name.c_str(), name.c_str()) == 0) {
			(*it).retainCount--;
			if ((*it).retainCount == 0) {
				delete (*it).texture;
				this->textures.erase(it);			
			}			
			return true;
		}
	
	return false;
}

bool GraphicsManager::RemoveAllTextures() {
	
	TextureCollection::iterator it;
	
	for (it = this->textures.begin(); it < this->textures.end(); it++) 
		delete (*it).texture;
	
	this->textures.clear();
	return true;
}

void GraphicsManager::AddImageDetailsToRenderQueue(ImageDetails *imageDetails) {
    
	this->CopyImageDetails(imageDetails);
	this->AddToTextureList(imageDetails->textureName);
	ivaIndex++;
	
}

void GraphicsManager::AddTexturedColoredQuadToRenderQueue(TexturedColoredQuad *TCQ, GLuint texture) {
	
	memcpy((TexturedColoredQuad*)iva + ivaIndex, TCQ, sizeof(TexturedColoredQuad));
	
	this->AddToTextureList(texture);
	
	ivaIndex++;
}

void GraphicsManager::RenderImages() {
	
	/* 
	 * Setup how textures should be rendered i.e. 
	 * how a texture with alpha should be rendered 
	 * on top of another texture.
	 */
	if (!glIsEnabled(GL_BLEND)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if (!glIsEnabled(GL_TEXTURE_2D)) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ALPHA);
	}

	if (!glIsEnabled(GL_DEPTH_TEST))
		glEnable(GL_DEPTH_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

	// Populate the vertex, texcoord and colorpointers with our interleaved vertex data
    glVertexPointer(2, GL_FLOAT, sizeof(TexturedColoredVertex), &iva[0].geometryVertex);
    glTexCoordPointer(2, GL_FLOAT, sizeof(TexturedColoredVertex), &iva[0].textureVertex);
    glColorPointer(4,GL_FLOAT,sizeof(TexturedColoredVertex), &iva[0].vertexColor);
	
	for (GLuint textureIndex = 0; textureIndex < renderTextureCount; textureIndex++) {
		glBindTexture(GL_TEXTURE_2D, this->texturesToRender[textureIndex]);
		int vertexCounter = 0;
		for (GLuint imageIndex = 0; imageIndex < imageCountForTexture[this->texturesToRender[textureIndex]]; imageIndex++) {
			GLuint index = this->textureIndices[this->texturesToRender[textureIndex]][imageIndex] * 4;
            this->ivaIndices[vertexCounter++] = index;     // Bottom left
            this->ivaIndices[vertexCounter++] = index+2;   // Top Left
            this->ivaIndices[vertexCounter++] = index+1;   // Bottom right
            this->ivaIndices[vertexCounter++] = index+1;   // Bottom right
            this->ivaIndices[vertexCounter++] = index+2;   // Top left
            this->ivaIndices[vertexCounter++] = index+3;   // Top right
		}
		// Now load the indices array with indexes into the IVA, we draw those triangles to the screen.
		glDrawElements(GL_TRIANGLES, vertexCounter, GL_UNSIGNED_SHORT, ivaIndices);
		// Clear the quad count for the current texture.
		this->imageCountForTexture[this->texturesToRender[textureIndex]] = 0;
	}
	renderTextureCount = 0;
	ivaIndex = 0;

	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);


	if (glIsEnabled(GL_BLEND))
		glDisable(GL_BLEND);

	if (glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);

	if (glIsEnabled(GL_DEPTH_TEST))
		glDisable(GL_DEPTH_TEST);

}

void GraphicsManager::CopyImageDetails(ImageDetails* imageDetails) {
    
	if (ivaIndex + 1 > MAX_IMAGES_PER_FRAME) {
		printf("ERROR - GraphicsManager: Render queue size exceded. The system requested %d images and the queue can hold up to %d images per frame. Consider increasing the default size.", ivaIndex + 1, MAX_IMAGES_PER_FRAME);
		exit(1);
	}

	imageDetails->texturedColoredQuadIVA = (TexturedColoredQuad*)this->iva + this->ivaIndex;
	memcpy(imageDetails->texturedColoredQuadIVA, imageDetails->texturedColoredQuad, sizeof(TexturedColoredQuad));
	
}

void GraphicsManager::AddToTextureList(GLuint textureName) {
    
	bool textureFound = false;
	for (GLuint index = 0; index < this->renderTextureCount; index++) {
		if (texturesToRender[index] == textureName) {
			textureFound = true;
			break;
		}
	}
	
	if (!textureFound) 
		texturesToRender[renderTextureCount++] = textureName;
	
	textureIndices[textureName][imageCountForTexture[textureName]] = ivaIndex;
	
	imageCountForTexture[textureName] += 1;
}

void GraphicsManager::ResizeView(int w, int h) {
	this->m_camera->InitializeOpenGL(w, h);
}

void GraphicsManager::PrepareNewFrame() {
    this->m_camera->PrepareNewFrame(); 
}

void GraphicsManager::RenderNewFrame() {
	this->RenderImages();
}

void GraphicsManager::Update(double delta) {
    this->m_camera->Update(delta);
}

void GraphicsManager::HandleEvents() {
	
	bool up, down, left, right, camera;

	float SPEED = 10.0f;

	InputManager * i = InputManager::Instance();

	up = i->GetKeyState('8');
	down = i->GetKeyState('2');
	right = i->GetKeyState('6');
	left = i->GetKeyState('4');
	camera = i->GetKeyState('c');

	if (i->GetKeyState('z'))
		this->m_camera->Reset();

	if (up || down || right || left)
		this->m_camera->Move(right && left ? 0.0f : right ? -SPEED/2 : left ? SPEED/2 : 0.0f,
							 0.0f, 
							 down && up ? 0.0 : up ? SPEED : down ? -SPEED : 0.0f);

	int m_x = 0;
	int m_y = 0;
	int speed = 10;
	int direction = 1;

	switch(i->GetJoystick()->Speed.Upward) {
	case GameJoyStick::eJoyStickSpeeds::Slow:
		m_y = direction * speed * 1;
		break;
	case GameJoyStick::eJoyStickSpeeds::Normal:
		m_y = direction * speed * 2;
		break;
	case GameJoyStick::eJoyStickSpeeds::Moderate:
		m_y = direction * speed * 3;
		break;
    case GameJoyStick::eJoyStickSpeeds::Fast:
        m_y = direction * speed * 4;
        break;
    default:
        break;
	}

	direction = -1;

	switch(i->GetJoystick()->Speed.Downward) {
	case GameJoyStick::eJoyStickSpeeds::Slow:
		m_y = direction * speed * 1;
		break;
	case GameJoyStick::eJoyStickSpeeds::Normal:
		m_y = direction * speed * 2;
		break;
	case GameJoyStick::eJoyStickSpeeds::Moderate:
		m_y = direction * speed * 3;
		break;
	case GameJoyStick::eJoyStickSpeeds::Fast:
		m_y = direction * speed * 4;
		break;
    default:
        break;
	}

	direction = -1;

	switch(i->GetJoystick()->Speed.Leftward) {
	case GameJoyStick::eJoyStickSpeeds::Slow:
		m_x = direction * speed * 1;
		break;
	case GameJoyStick::eJoyStickSpeeds::Normal:
		m_x = direction * speed * 2;
		break;
	case GameJoyStick::eJoyStickSpeeds::Moderate:
		m_x = direction * speed * 3;
		break;
	case GameJoyStick::eJoyStickSpeeds::Fast:
		m_x = direction * speed * 4;
		break;
    default:
        break;
	}

	direction = 1;

	switch(i->GetJoystick()->Speed.Rightward) {
	case GameJoyStick::eJoyStickSpeeds::Slow:
		m_x = direction * speed * 1;
		break;
	case GameJoyStick::eJoyStickSpeeds::Normal:
		m_x = direction * speed * 2;
		break;
	case GameJoyStick::eJoyStickSpeeds::Moderate:
		m_x = direction * speed * 3;
		break;
	case GameJoyStick::eJoyStickSpeeds::Fast:
		m_x = direction * speed * 4;
		break;
    default:
        break;
	}

	if (camera)
		this->m_camera->Rotate(m_y, m_x, 0.0f);
}