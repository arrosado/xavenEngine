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
#include <glm.hpp>
#include <glm/gtc/matrix_transform.inl>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/Texture2D/Texture2D.h>

struct IGameObjectComponent;

struct IGameObject {
protected:
    std::vector<IGameObjectComponent *> components;
    
public:
    vec3 position;
    vec3 rotation;
    vec3 scale = vec3(1.0f, 1.0f, 1.0f);
    
    virtual void AddComponent(IGameObjectComponent * component) = 0;
    virtual void Draw() = 0;
};

struct IGameObjectComponent {
protected:
    IGameObject *m_gameObject;
public:
    IGameObjectComponent(IGameObject *gameObject)
    {
        m_gameObject = gameObject;
    }
    
    virtual void Draw() = 0;
};

class SpriteRendererComponent: public IGameObjectComponent {
public:
    Image image;
    
    SpriteRendererComponent(IGameObject *gameObject, const string imageName) : IGameObjectComponent(gameObject)
    {
        image.texture.data = new Texture2D(RESOURCES_FOLDER + imageName, GL_LINEAR);
        image.texture.name = imageName;
        image.texture.retainCount++;
    }
    
    void Draw()
    {
        // Set image geometry
        image.vertex1.geometry = vec3(0.0f, 0.0f, 0.0f);
        image.vertex2.geometry = vec3(image.texture.data->contentSize.width, 0.0f, 0.0f);
        image.vertex3.geometry = vec3(0.0f, image.texture.data->contentSize.height, 0.0f);
        image.vertex4.geometry = vec3(image.texture.data->contentSize.width, image.texture.data->contentSize.height, 0.0f);
        
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
        
        mat4 translate = glm::translate(glm::mat4(), this->m_gameObject->position);
        mat4 rotate = glm::rotate(glm::mat4(), this->m_gameObject->rotation.x, vec3(1.0f, 0.0f, 0.0f)) *
                      glm::rotate(glm::mat4(), this->m_gameObject->rotation.y, vec3(0.0f, 1.0f, 0.0f)) *
                      glm::rotate(glm::mat4(), this->m_gameObject->rotation.z, vec3(0.0f, 0.0f, 1.0f));
        mat4 scale = glm::scale(mat4(), this->m_gameObject->scale);
        
        mat4 transform = translate * rotate * scale;
        
        image.vertex1.geometry = (transform * glm::vec4(image.vertex1.geometry, 1.0f)).xyz();
        image.vertex2.geometry = (transform * glm::vec4(image.vertex2.geometry, 1.0f)).xyz();
        image.vertex3.geometry = (transform * glm::vec4(image.vertex3.geometry, 1.0f)).xyz();
        image.vertex4.geometry = (transform * glm::vec4(image.vertex4.geometry, 1.0f)).xyz();
        
        
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

class GameObject: public IGameObject
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
        this->components.push_back(component);
    }
    
    void Draw()
    {
        std::vector<IGameObjectComponent *>::iterator it;
        for(it = components.begin(); it != components.end(); ++it) {
            /* std::cout << *it; ... */
            (*it)->Draw();
        }
    }
};



#endif
