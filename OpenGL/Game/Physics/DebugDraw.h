/*
 *  GLESDebugDraw.h
 *  ARR Game Engine Core
 *
 *  Created by Ariel Rosado on 2/19/11.
 *  Copyright 2011 Xaven. All rights reserved.
 *
 */
#ifndef __GLES_RENDER_H__
#define __GLES_RENDER_H__

#include <Game/Graphics/GraphicsCommon.h>
#include <Game/Physics/Box2D/Box2D.h>

struct b2AABB;

// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class DebugDraw : public b2Draw
{
	float32 mRatio;
public:
	DebugDraw();
	
	DebugDraw( float32 ratio );
	
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	
	void DrawTransform(const b2Transform& xf);
	
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
	
    void DrawString(int x, int y, const char* string, ...); 

	void DrawString(const b2Vec2& p, const char *string, ...);
	
    void DrawAABB(b2AABB* aabb, const b2Color& color);
};


#endif // __GLES_RENDER_H__
