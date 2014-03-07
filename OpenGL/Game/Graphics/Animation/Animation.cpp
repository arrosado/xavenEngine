/*
 *  XGEAnimation.cpp
 *  ARR Game Engine Core
 *
 *  Created by Ariel on 11/25/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */

#include <Animation.h>

Animation::~Animation() {
    for (unsigned int i = 0; i < m_frames.size(); i++) {
        delete m_frames[i]->image;
    }
    m_frames.empty();
    delete &m_frames;
}

Animation::Animation( const string spriteSheetName, GLuint width, GLuint height ) {	

	// Set the default values for important properties
	m_currentFrame = 0;
	m_displayTime = 0;
    m_bounceFrame = -1;
    m_type = AnimationType::ONCE;
    m_state = AnimationState::AS_STOPPED;
	m_direction = AnimationDirection::FORWARD;
	
    //float scale = 1.0f;
    float delay = 0.05f;
    GLuint spacing = 0.0f;
	// Create a new SpriteSheet with the image name to gather the frame images.
    printf("Path is %s\n", spriteSheetName.c_str());
	SpriteSheet *s = new SpriteSheet(spriteSheetName, Size2DfMake(width, height), spacing, 0, GL_LINEAR);	
	// Calculate the number of rows and the number of columns that the sprite sheet has.
	int numberOfColumns = s->GetColumns();
	int numberOfRows = s->GetRows();

	// Gather the frame images.
	for (int y = 0; y < numberOfRows; y++) 
		for (int x = 0; x < numberOfColumns; x++)
			this->AddFrame(s->GetSprite(x, y), delay);

	// Delete the SpriteSheet cause we no longer need it!
	delete s;
	
	assert(m_frames.size() != 0 && "The animation must have at least one frame.");
}

Animation::Animation( const string spriteSheetName, int numberOfRows, int numberOfColumns, float delay, GLuint width, GLuint height, GLuint spacing, float scale) {

	// Set the default values for important properties
	m_currentFrame = 0;
	m_displayTime = 0;
    m_bounceFrame = -1;
    m_type = AnimationType::ONCE;
    m_state = AnimationState::AS_STOPPED;
	m_direction = AnimationDirection::FORWARD;
	
	// Create a new SpriteSheet with the image name to gather the frame images.
	SpriteSheet *s = new SpriteSheet(spriteSheetName, Size2DfMake(width, height), spacing, 0, GL_LINEAR);
	// Gather the frame images.
	for (int y = 0; y < numberOfRows; y++) 
		for (int x = 0; x < numberOfColumns; x++)
			this->AddFrame(s->GetSprite(x, y), delay);
	// Delete the SpriteSheet cause we no longer need it!
	delete s;

	assert(m_frames.size() != 0  && "The animation must have at least one frame.");
}

void Animation::AddFrame(Image* image, float delay) {
	// Create a new frame instance which will hold the frame image and delay for that image
    m_frames.push_back(new AnimationFrame());
	m_frames.back()->image = image;
    m_frames.back()->delay = delay;
}

void Animation::Reset(AnimationType type) {
	m_type = type;
	m_currentFrame = (m_direction == AnimationDirection::FORWARD) ? 0 : this->GetAnimationFrameCount() - 1;
	m_state = AnimationState::AS_RUNNING;
}

void Animation::Update(float delta) {
	// If the animation is not running then don't do anything
	if(m_state != AnimationState::AS_RUNNING) 
        return;
	
	// Update the timer with the delta
	m_displayTime += delta;
	
	if (m_displayTime > m_frames[m_currentFrame]->delay) { 
		m_currentFrame += m_direction;
        
        if (m_type == AnimationType::PING_PONG && (m_currentFrame == 0 || 
												 m_currentFrame == m_frames.size() - 1 || 
												 m_currentFrame == m_bounceFrame)) {
               m_direction = (m_direction == FORWARD) ? BACKWARD : FORWARD;
        } else if (m_currentFrame > m_frames.size() - 1 || m_currentFrame == m_bounceFrame) {

			switch(m_type) {
			case AnimationType::ONCE_AND_RESTART:
				m_state = AnimationState::AS_STOPPED;
			case AnimationType::REPEAT:
				m_currentFrame = (m_direction == FORWARD) ? 0 : this->GetAnimationFrameCount() - 1;
				break;
			case AnimationType::ONCE:
				m_currentFrame =  (m_direction == FORWARD) ? this->GetAnimationFrameCount() - 1 : 0;
				m_state = AnimationState::AS_STOPPED;
				break;
            default:
                break;
			};
        }
        
        m_displayTime -= m_frames[m_currentFrame]->delay;
    }
}

void Animation::Render(Vector2Df point, float scale, float rotation) {
	m_frames[m_currentFrame]->image->Render(point, Size2DfMake(scale, scale), rotation, true);
}

void Animation::Render(Vector2Df point) {
	m_frames[m_currentFrame]->image->Render(point, true);
}

void Animation::SetAnimationFramesDelay(float delay) {
	for (int i = 0; i < m_frames.size(); i++)
		m_frames[i]->delay = delay;
}

Image* Animation::GetCurrentFrameImage() {
	// Return the image which is being used for the current frame
	return m_frames[m_currentFrame]->image;
}

GLuint Animation::GetAnimationFrameCount() {
	// Return the total number of frames in this animation
	return (int)m_frames.size();
}

GLuint Animation::GetCurrentFrameNumber() {
	// Return the current frame within this animation
	return m_currentFrame;
}

AnimationFrame* Animation::GetFrame(GLuint frameNumber) {
	// If a frame number is reuqested outside the range that exists, return nil
	// and log an error
	if(frameNumber > m_frames.size())
		throw "WARNING: Requested frame is out of bounds";
	
	return m_frames[frameNumber];
}

void Animation::FlipAnimation(bool flipVertically, bool flipHorizontally) {
	for(unsigned int i=0; i < m_frames.size(); i++) {
		m_frames[i]->image->SetFlip(flipVertically, flipHorizontally);
	}
}

AnimationDirection Animation::GetAnimationDirection() {
	return m_direction;
}

void Animation::SetAnimationDirection(AnimationDirection newAnimationDirection) {
	switch(newAnimationDirection) { 
	case AnimationDirection::BACKWARD:
		
		m_currentFrame = this->GetAnimationFrameCount() - 1;

		break;
	case AnimationDirection::FORWARD:

		m_currentFrame = 0;

		break;
	};

	m_direction = newAnimationDirection;
}

AnimationType Animation::GetAnimationType() {
	return m_type;
}

void Animation::SetAnimationType(AnimationType newAnimationType) {
	m_type = newAnimationType;
}

AnimationState Animation::GetAnimationState() {
	return m_state;
}

void Animation::SetAnimationState(AnimationState newAnimationState) {
	m_state = newAnimationState;
}

int Animation::GetBounceFrame() {
	return m_bounceFrame;
}

void Animation::SetBounceFrame(int newBounceFrame) {
	m_bounceFrame = newBounceFrame;
}
