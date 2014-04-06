//
//  IGameObjectComponent.h
//  OpenGL
//
//  Created by Ariel Rosado Rivera on 4/5/14.
//  Copyright (c) 2014 Ariel Rosado. All rights reserved.
//

#ifndef OpenGL_IGameObjectComponent_h
#define OpenGL_IGameObjectComponent_h

extern struct IGameObject;

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


#endif
