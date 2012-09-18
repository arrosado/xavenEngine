/*
 *  XGEAnimation.cpp
 *  ARR Game Engine Core
 *
 *  Created by Ariel on 11/25/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */

#include <Graphics Engine\Animation\Animation.h>

Animation::~Animation() {
    for (unsigned int i = 0; i < this->frames.size(); i++) {
        delete this->frames[i]->image;
    }
    this->frames.empty();
    delete &this->frames;
}

Animation::Animation( const string spriteSheetName, GLuint width, GLuint height )
{	
	// Initialize the array which will hold our frames
	//this->frames = new AnimationFrameCollection();
	
	// Set the default values for important properties
	this->currentFrame = 0;
	this->displayTime = 0;
    this->bounceFrame = -1;
    this->type = kAnimationType_Once;
    this->state = kAnimationState_Stopped;
	this->direction = 1;//kAnimationDirection_Forward;
	
    //float scale = 1.0f;
    float delay = 0.05f;
    GLuint spacing = 0.0f;
	// Create a new SpriteSheet with the image name to gather the frame images.
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
}

Animation::Animation( const string spriteSheetName, int numberOfRows, int numberOfColumns, float delay, GLuint width, GLuint height, GLuint spacing, float scale) {
	// Initialize the array which will hold our frames
	//this->frames = new AnimationFrameCollection();
	
	// Set the default values for important properties
	this->currentFrame = 0;
	this->displayTime = 0;
    this->bounceFrame = -1;
    this->type = kAnimationType_Once;
    this->state = kAnimationState_Stopped;
	this->direction = 1;//kAnimationDirection_Forward;
	
	// Create a new SpriteSheet with the image name to gather the frame images.
	SpriteSheet *s = new SpriteSheet(spriteSheetName, Size2DfMake(width, height), spacing, 0, GL_LINEAR);
	// Gather the frame images.
	for (int y = 0; y < numberOfRows; y++) 
		for (int x = 0; x < numberOfColumns; x++)
			this->AddFrame(s->GetSprite(x, y), delay);
	// Delete the SpriteSheet cause we no longer need it!
	delete s;
}

void Animation::AddFrame(Image* image, float delay) {
	// Create a new frame instance which will hold the frame image and delay for that image
    this->frames.push_back(new AnimationFrame());
	this->frames.back()->image = image;
    this->frames.back()->delay = delay;
}

void Animation::Reset(AnimationType type) {
	this->type = type;
	this->currentFrame = 0;
	this->state = kAnimationState_Running;
}

void Animation::Update(float delta) {
	// If the animation is not running then don't do anything
	if( this->state != kAnimationState_Running ) 
        return;
	
	// Update the timer with the delta
	this->displayTime += delta;
	
	if (this->displayTime > this->frames[this->currentFrame]->delay) { 
        this->currentFrame += this->direction;
        
        if (this->type == kAnimationType_PingPong && 
           (this->currentFrame == 0 || 
            this->currentFrame > this->frames.size() - 1 || 
            this->currentFrame == bounceFrame)) {
               this->direction = -(int)this->direction;
        } else if (this->currentFrame > this->frames.size() - 1 ||
                   this->currentFrame == bounceFrame) {
            if (this->type != kAnimationType_Repeating) {
				this->currentFrame -= 1;
                this->state = kAnimationState_Stopped;
			} else {
				this->currentFrame = 0;
			}
        }
        
        this->displayTime -= this->frames[this->currentFrame]->delay;
    }
}

void Animation::Render(Vector2Df point, float scale, float rotation) {
	this->frames[this->currentFrame]->image->Render(point, Vector2DfMake(scale, scale), rotation, true);
}

void Animation::Render(Vector2Df point) {
	this->frames[this->currentFrame]->image->Render(point, true);
}

Image* Animation::GetCurrentFrameImage() {
	// Return the image which is being used for the current frame
	return this->frames[this->currentFrame]->image;
}

GLuint Animation::GetAnimationFrameCount() {
	// Return the total number of frames in this animation
	return this->frames.size();
}

GLuint Animation::GetCurrentFrameNumber() {
	// Return the current frame within this animation
	return this->currentFrame;
}

AnimationFrame* Animation::GetFrame(GLuint frameNumber) {
	// If a frame number is reuqested outside the range that exists, return nil
	// and log an error
	if(frameNumber > this->frames.size())
		throw "WARNING: Requested frame is out of bounds";
	
	return this->frames[frameNumber];
}

void Animation::FlipAnimation(bool flipVertically, bool flipHorizontally) {
	for(unsigned int i=0; i < this->frames.size(); i++) {
		this->frames[i]->image->SetFlip(flipVertically, flipHorizontally);
	}
}
