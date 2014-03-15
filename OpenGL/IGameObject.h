//
//  IGameObject.h
//  OpenGL
//
//  Created by Betsy Serrano on 3/14/14.
//  Copyright (c) 2014 Ariel Rosado. All rights reserved.
//

#ifndef OpenGL_IGameObject_h
#define OpenGL_IGameObject_h

#include <string>
#include <vector>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/Texture2D/Texture2D.h>

struct TransformComponent;

struct IGameObjectComponent {
public:
    TransformComponent *transform;
    virtual void Draw() = 0;
};

class TransformComponent : public IGameObjectComponent {
public:
    vec3 position;
    vec3 rotation;
    vec3 scale;
    
    void Draw() {}
};


class SpriteRendererComponent: public IGameObjectComponent {
public:
    Image image;
    
    SpriteRendererComponent(const string imageName)
    {
        image.texture.data = new Texture2D(RESOURCES_FOLDER + imageName, GL_LINEAR);
        image.texture.name = imageName;
        image.texture.retainCount++;
        
        
        GLfloat x = this->transform->position.x;
        GLfloat y = this->transform->position.y;
        GLfloat z = this->transform->position.z;
        
        // Set image geometry
        image.vertex1.geometry.x = x;
        image.vertex1.geometry.y = x;
        image.vertex1.geometry.z = z;
        
        image.vertex2.geometry.x = x + image.texture.data->contentSize.width;
        image.vertex2.geometry.y = y;
        image.vertex2.geometry.z = z;
        
        image.vertex3.geometry.x = x;
        image.vertex3.geometry.y = y + image.texture.data->contentSize.height;
        image.vertex3.geometry.z = z;
        
        image.vertex4.geometry.x = x + image.texture.data->contentSize.width;
        image.vertex4.geometry.y = y + image.texture.data->contentSize.height;
        image.vertex4.geometry.z = z;
        
        // Set image colors
        image.vertex1.color = Color4fWhite;
        image.vertex2.color = Color4fWhite;
        image.vertex3.color = Color4fWhite;
        image.vertex4.color = Color4fWhite;
        
        // Set texture coordinates
        image.vertex1.texture.x = 0.0f;
        image.vertex1.texture.y = 0.0f;
        
        image.vertex2.texture.x = image.texture.data->maxS;
        image.vertex2.texture.y = 0.0f;
        
        image.vertex3.texture.x = 0.0f;
        image.vertex3.texture.y = image.texture.data->maxT;
        
        image.vertex4.texture.x = image.texture.data->maxS;
        image.vertex4.texture.y = image.texture.data->maxT;
    }
    
    void Draw()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ALPHA);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glBindTexture(GL_TEXTURE_2D, image.texture.data->name);
        
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex) , &image.vertex1.geometry);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &image.vertex1.texture);
        glColorPointer(4, GL_FLOAT, sizeof(Vertex), &image.vertex1.color);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
};

struct IGameObject {
protected:
    std::vector<IGameObjectComponent *> components;
    
public:
    virtual void AddComponent(IGameObjectComponent * component) = 0;
    virtual void Draw();
};

class GameObject: TransformComponent, public IGameObject
{
public:
    
    GameObject()
    {
    }
    
    ~GameObject()
    {
    }
    
    void AddComponent(IGameObjectComponent * component)
    {
        component->transform = this;
        
        this->components.push_back(component);
    }
    
    void Draw()
    {
        std::vector<IGameObjectComponent *>::iterator it;
        for(it = components.begin(); it != components.end(); ++it) {
            /* std::cout << *it; ... */
            ((IGameObjectComponent)*it).Draw();
        }
    }
};



#endif
