//
//  GameObject.h
//  OpenGL
//
//  Created by Ariel Rosado Rivera on 4/5/14.
//  Copyright (c) 2014 Ariel Rosado. All rights reserved.
//

#ifndef OpenGL_GameObject_h
#define OpenGL_GameObject_h

#include <vector>
#include <glm.hpp>
#include <IGameObject.h>

class GameObject: public IGameObject
{
public:
    
    GameObject()
    {
		this->scale = vec3(1.0f, 1.0f, 1.0f);
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
            (*it)->Draw();
        }
    }
};




#endif
