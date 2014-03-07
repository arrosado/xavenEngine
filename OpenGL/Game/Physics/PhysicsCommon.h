/*
 *  PhysicsCommon.h
 *  ARR Game Engine Core
 *
 *  Created by Ariel Rosado on 2/20/11.
 *  Copyright 2011 Xaven. All rights reserved.
 *
 */

#ifndef __PHYSICS_COMMON_H__
#define __PHYSICS_COMMON_H__

#include <Game/Physics/Box2D/Box2D.h>
#include <Game/Physics/Box2D/Dynamics/Contacts/b2Contact.h>

//Pixel to metres ratio. Box2D uses metres as the unit for measurement.
//This ratio defines how many pixels correspond to 1 Box2D "metre"
//Box2D is optimized for objects of 1x1 metre therefore it makes sense
//to define the ratio so that your most common object type is 1x1 metre.
#define PTM_RATIO 32.0
	
inline float GetRealWH(float x) {
	return x / PTM_RATIO;
}

inline float GetUnRealWH(float x) {
	return x * PTM_RATIO;
}

#endif /* __PHYSICS_COMMON_H__ */