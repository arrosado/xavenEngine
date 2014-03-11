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

#include <Game/Graphics/Image/Image.h>
#include <Game/Graphics/SpriteSheet/SpriteSheet.h>
#include <string>
#include <vector>
#include <assert.h>

typedef enum {
	FORWARD = 1,
	BACKWARD = -1
} AnimationDirection;

typedef enum { 
    REPEAT = 1,
    PING_PONG, 
    ONCE,
    ONCE_AND_RESTART
} AnimationType;

typedef enum { 
    AS_RUNNING = 1,
    AS_STOPPED
} AnimationState;

typedef struct {
	// The image this frame of animation will display
	Image *image;
	// How long the frame should be displayed for
	float delay;
} AnimationFrame;

typedef std::vector<AnimationFrame *> AnimationFrameCollection;

class Animation {
private:
	void AddFrame(Image* image, float delay);
	AnimationFrameCollection m_frames;
    AnimationState m_state;
    AnimationType m_type;
    AnimationDirection m_direction;
	float m_displayTime;
	int m_currentFrame;
    int m_bounceFrame;
public:
	
	Animation( const std::string spriteSheetName, GLuint width, GLuint height );
	Animation( const std::string spriteSheetName, int numberOfRows, int numberOfColumns, float delay, GLuint width, GLuint height, GLuint spacing, float scale );
	~Animation();
	
	void Reset(AnimationType type);
	void Update( float delta );
	void Render(Vector3Df point);
	void Render(Vector3Df point, Size2Df scale, Vector3Df rotation);
	AnimationDirection GetAnimationDirection();
	void SetAnimationDirection(AnimationDirection newAnimationDirection);
	AnimationType GetAnimationType();
	void SetAnimationType(AnimationType newAnimationType);
	AnimationState GetAnimationState();
	void SetAnimationState(AnimationState newAnimationState);
	void SetAnimationFramesDelay(float delay);
	int GetBounceFrame();
	void SetBounceFrame(int newBounceFrame);
	Image* GetCurrentFrameImage();
	GLuint GetAnimationFrameCount();
	GLuint GetCurrentFrameNumber();
	AnimationFrame* GetFrame( GLuint frameNumber );
	void FlipAnimation( bool vertically, bool horizontally );
};

#endif /* __ANIMATION_H__ */