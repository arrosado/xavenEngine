/*
 *  Animation.h
 *  ARR Game Engine Core
 *
 *  Created by Ariel on 11/25/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <Graphics Engine\Image\Image.h>
#include <Graphics Engine\SpriteSheet\SpriteSheet.h>
#include <string>
#include <vector>

/*
typedef enum {
	kAnimationDirection_Forward = 1,
	kAnimationDirection_Backwards = -1
} AnimationDirection;
*/
typedef int AnimationDirection;

typedef enum { 
    kAnimationType_Repeating = 1,
    kAnimationType_PingPong, 
    kAnimationType_Once,
    kAnimationType_OnceAndReset,
    kAnimationType_Count
} AnimationType;

typedef enum { 
    kAnimationState_Running = 1,
    kAnimationState_Stopped,
    kAnimationState_Count
} AnimationState;

typedef struct {
	// The image this frame of animation will display
	Image *image;
	// How long the frame should be displayed for
	float delay;
} AnimationFrame;

typedef vector<AnimationFrame *> AnimationFrameCollection;

class Animation {
private:
	void AddFrame(Image* image, float delay);
public:
	AnimationFrameCollection frames;
    AnimationState state;
    AnimationType type;
    AnimationDirection direction;
	float displayTime;
	GLuint currentFrame;
    int bounceFrame;
	
	Animation( const string spriteSheetName, GLuint width, GLuint height );
	Animation( const string spriteSheetName, int numberOfRows, int numberOfColumns, float delay, GLuint width, GLuint height, GLuint spacing, float scale );
	~Animation();
	
	void Reset(AnimationType type);
	void Update( float delta );
	void Render(Vector2Df point);
	void Render(Vector2Df point, float scale, float rotation);
	Image* GetCurrentFrameImage();
	GLuint GetAnimationFrameCount();
	GLuint GetCurrentFrameNumber();
	AnimationFrame* GetFrame( GLuint frameNumber );
	void FlipAnimation( bool vertically, bool horizontally );
};

#endif /* __ANIMATION_H__ */