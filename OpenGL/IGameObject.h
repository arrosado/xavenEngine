//
//  IGameObject.h
//  OpenGL
//
//  Created by Betsy Serrano on 3/14/14.
//  Copyright (c) 2014 Ariel Rosado. All rights reserved.
//

#ifndef OpenGL_IGameObject_h
#define OpenGL_IGameObject_h

#include <vector>
#include <Game/Graphics/GraphicsCommon.h>

struct IGameObjectComponent {
    
};

class TransformComponent : IGameObjectComponent {
public:
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

struct IGameObject {
protected:
    std::vector<IGameObjectComponent*> components;
public:
    virtual void AddComponent(const IGameObjectComponent * component) = 0;
};

class GameObject: IGameObject
{
public:
    void AddComponent(IGameObjectComponent * component)
    {
        this->components.push_back(component);
    }
};



#endif
