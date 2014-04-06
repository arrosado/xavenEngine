//
//  SpriteRendererComponent.h
//  OpenGL
//
//  Created by Ariel Rosado Rivera on 4/5/14.
//  Copyright (c) 2014 Ariel Rosado. All rights reserved.
//

#ifndef OpenGL_SpriteRendererComponent_h
#define OpenGL_SpriteRendererComponent_h

#include <Game/IGameObject.h>
#include <Game/IGameObjectComponent.h>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Graphics/Texture2D/Texture2D.h>
#include <glm/gtc/matrix_transform.inl>

class SpriteRendererComponent: public IGameObjectComponent {
public:
    Image image;
    
    SpriteRendererComponent(IGameObject *gameObject, const string imageName) : IGameObjectComponent(gameObject)
    {
        image.texture.data = new Texture2D(RESOURCES_FOLDER + imageName, GL_LINEAR);
        image.texture.name = imageName;
        image.texture.retainCount++;
        image.texture.offset = vec2(0.0f, 0.0f);
        image.size = image.texture.data->contentSize;
        image.texture.size.width = image.texture.data->maxS;
        image.texture.size.height = image.texture.data->maxT;
    }
    
    SpriteRendererComponent(IGameObject *gameObject, const string imageName, Rect2Df subImageRect) : IGameObjectComponent(gameObject)
    {
        image.texture.data = new Texture2D(RESOURCES_FOLDER + imageName, GL_LINEAR);
        image.texture.name = imageName;
        image.texture.retainCount++;
        image.texture.offset = vec2(0.0f, 0.0f);
        image.size = image.texture.data->contentSize;
        image.texture.size.width = image.texture.data->maxS;
        image.texture.size.height = image.texture.data->maxT;
        
        image.size = subImageRect.size;
        
        image.texture.offset = vec2(image.texture.data->ratio.width * subImageRect.origin.x,
                                    image.texture.data->ratio.height * subImageRect.origin.y);
        
        image.texture.size.width = (image.texture.data->ratio.width * image.size.width) + image.texture.offset.x;
        image.texture.size.height = (image.texture.data->ratio.height * image.size.height) + image.texture.offset.y;
    }
    
    void Draw()
    {
        // Set image geometry
        image.vertex1.geometry = vec3(0.0f, 0.0f, 0.0f);
        image.vertex2.geometry = vec3(image.size.width, 0.0f, 0.0f);
        image.vertex3.geometry = vec3(0.0f, image.size.height, 0.0f);
        image.vertex4.geometry = vec3(image.size.width, image.size.height, 0.0f);
        
        // Set image colors
        image.vertex1.color = Color4fWhite;
        image.vertex2.color = Color4fWhite;
        image.vertex3.color = Color4fWhite;
        image.vertex4.color = Color4fWhite;
        
        // Set texture coordinates
        image.vertex1.texture.x = image.texture.offset.x;
        image.vertex1.texture.y = image.texture.offset.y;
        
        image.vertex2.texture.x = image.texture.size.width;
        image.vertex2.texture.y = image.texture.offset.y;
        
        image.vertex3.texture.x = image.texture.offset.x;
        image.vertex3.texture.y = image.texture.size.height;
        
        image.vertex4.texture.x = image.texture.size.width;
        image.vertex4.texture.y = image.texture.size.height;
        
        mat4 translate = glm::translate(glm::mat4(), this->m_gameObject->position);
        mat4 translateToCenter = glm::mat4();
        
        if (this->m_gameObject->centered)
        {
            vec3 center = vec3(-((image.size.width * this->m_gameObject->scale.x) / 2.0f),
                               -((image.size.height * this->m_gameObject->scale.y) / 2.0f),
                               0.0f);
            
            translateToCenter = glm::translate(glm::mat4(), center);
        }
        
        mat4 rotate = glm::rotate(glm::mat4(), this->m_gameObject->rotation.x, vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(), this->m_gameObject->rotation.y, vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(), this->m_gameObject->rotation.z, vec3(0.0f, 0.0f, 1.0f));
        
        mat4 scale = glm::scale(mat4(), this->m_gameObject->scale);
        
        mat4 transform = translate * rotate * scale * translateToCenter;
        
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


#endif
