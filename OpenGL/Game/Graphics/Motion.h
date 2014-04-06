//
//  Motion.h
//  ARR Game Engine Core
//
//  Created by Ariel Rosado on 3/13/11.
//  Copyright 2011 Xaven. All rights reserved.
//
/*
 *  Motion.h
 *  Game
 *
 *  Created by Ariel Rosado on 6/29/10.
 *  Copyright 2010 Xaven. All rights reserved.
 *
 */

#ifndef __GAME_MOTION_H__
#define __GAME_MOTION_H__

#include <Game/Graphics/GraphicsCommon.h>
/*
 *  @param t Specifies the current time, between 0 and duration inclusive.
 *
 *  @param b Specifies the initial value of the animation property.
 *
 *  @param c Specifies the total change in the animation property.
 *
 *  @param d Specifies the duration of the motion.
 *
 *  @param s Specifies the amount of overshoot, where the higher the value, 
 *  the greater the overshoot.
 *
 *  @return The value of the interpolated property at the specified time.
 */ 

enum MotionType {
	EaseNone,
	EaseIn, 
	EaseOut,
	EaseInOut
};

struct IMotion {
	virtual float EaseNone(float t, float b, float c, float d) = 0;
    virtual float EaseIn(float t, float b, float c, float d) = 0;
	virtual float EaseOut(float t, float b, float c, float d) = 0;
	virtual float EaseInOut(float t, float b, float c, float d) = 0;
	virtual ~IMotion() {}
};

struct Back {
	float EaseIn(float t, float b, float c, float d, float s = 0.0f);
	float EaseOut(float t, float b, float c, float d, float s = 0.0f);
	float EaseInOut(float t, float b, float c, float d, float s = 0.0f);
};

/*
 *  @param t Specifies the current time, between 0 and duration inclusive.
 *
 *  @param b Specifies the initial value of the animation property.
 *
 *  @param c Specifies the total change in the animation property.
 *
 *  @param d Specifies the duration of the motion.
 *
 *  @param a Specifies the amplitude of the sine wave.
 *
 *  @param p Specifies the period of the sine wave.
 *
 *  @return The value of the interpolated property at the specified time.
 */

struct Elastic {
	float EaseIn(float t, float b, float c, float d, float a = 0, float p = 0);
	float EaseOut(float t, float b, float c, float d, float a = 0, float p = 0);	
	float EaseInOut(float t, float b, float c, float d, float a = 0, float p = 0);
};

struct Linear : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};

struct Bounce : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};

struct Quadratic : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};

struct Cubic : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};

struct Quartic : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};

struct Quintic : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};

struct Sinusoidal : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};

struct Exponential : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};

struct Circular : IMotion {
	float EaseNone(float t, float b, float c, float d);
	float EaseIn(float t, float b, float c, float d);
	float EaseOut(float t, float b, float c, float d);
	float EaseInOut(float t, float b, float c, float d);
};


class MotionAnimation {
private:
	
	IMotion *m_motion;
	MotionType m_type;
	
	bool m_isActive;
	float m_elapsed;
	float m_duration;
	
	float m_start_value;
	float* m_current_value;
	float m_desired_value;
	
public:
	
	MotionAnimation(IMotion* motion, MotionType type, float* curr_val, float desired_val, float duration);
	~MotionAnimation();
	
	void SetMotion(IMotion* motion);
	void SetDuration(float newVal);
	void SetDesired(float newVal);
    
	void Activate();
	void Reset();
	bool Update(float delta);
	void Finish();
};

#endif /* __GAME_MOTION_H__ */