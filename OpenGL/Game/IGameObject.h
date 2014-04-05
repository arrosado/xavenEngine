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
#include <IGameObjectComponent.h>
#include <glm/gtc/matrix_transform.inl>

//struct IGameObjectComponent;

struct IGameObject {
protected:
    std::vector<IGameObjectComponent *> components;
    
public:
    vec3 position;
    vec3 rotation;
    vec3 scale;
    bool centered;
    
    virtual void AddComponent(IGameObjectComponent * component) = 0;
    virtual void Draw() = 0;
};

#endif
