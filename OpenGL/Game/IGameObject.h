//
//  IGameObject.h
//  OpenGL
//
//  Created by Betsy Serrano on 3/14/14.
//  Copyright (c) 2014 Ariel Rosado. All rights reserved.
//

#ifndef IGameObject_H
#define IGameObject_H

#include <vector>
#include <Game/Graphics/GraphicsCommon.h>
#include <Game/IGameObjectComponent.h>

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
