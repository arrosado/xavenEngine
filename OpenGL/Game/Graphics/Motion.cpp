//
//  Motion.cpp
//  ARR Game Engine Core
//
//  Created by Ariel Rosado on 3/13/11.
//  Copyright 2011 Xaven. All rights reserved.
//

#include <Game/Graphics/Motion.h>
#include <cstdlib>
#include <math.h>


MotionAnimation::MotionAnimation(IMotion* motion, MotionType type, float* curr_val, float desired_val, float duration) {
	/*
	 * Reset the object position.
	 */
	this->m_motion = motion;
	this->m_type = type;
	this->m_elapsed = 0.0f;
	this->m_duration = duration;
	this->m_current_value = curr_val;
	this->m_desired_value = desired_val;
	this->Reset();
}
MotionAnimation::~MotionAnimation() {
	if (this->m_motion != 0)
		delete this->m_motion;
}
void MotionAnimation::SetMotion(IMotion* motion) {
	if (this->m_motion != 0)
		delete this->m_motion;
	this->m_motion = motion;
}
void MotionAnimation::SetDuration(float newVal) {
	this->m_duration = newVal;
}
void MotionAnimation::SetDesired(float newVal) {
	this->m_desired_value = newVal;
}
void MotionAnimation::Activate() {
	this->m_isActive = true;
}
void MotionAnimation::Reset() {
	/*
	 * Only reset the animation variables. Don't
	 * reset the current position, or desired position
	 * variabes.
	 */
	this->m_isActive = false;
	this->m_elapsed = 0.0f;
	this->m_duration = 0.0f;
	
	this->m_desired_value = *(this->m_current_value);
	this->m_start_value = *(this->m_current_value);
}
bool MotionAnimation::Update(float delta) {
	if (this->m_isActive) {
		if (this->m_elapsed > this->m_duration || this->m_motion == 0) {
			this->Finish();
		} else {
			this->m_elapsed += delta;
			
			if ( *(this->m_current_value) != this->m_desired_value)
				switch (this->m_type) {
					case EaseNone:
						*(this->m_current_value) = this->m_motion->EaseNone(this->m_elapsed, this->m_start_value, this->m_desired_value - this->m_start_value, this->m_duration);
					case EaseIn:
						*(this->m_current_value) = this->m_motion->EaseIn(this->m_elapsed, this->m_start_value, this->m_desired_value - this->m_start_value, this->m_duration);
						break;
					case EaseOut:
						*(this->m_current_value) = this->m_motion->EaseOut(this->m_elapsed, this->m_start_value, this->m_desired_value - this->m_start_value, this->m_duration);
						break;
					case EaseInOut:
						*(this->m_current_value) = this->m_motion->EaseInOut(this->m_elapsed, this->m_start_value, this->m_desired_value - this->m_start_value, this->m_duration);
						break;
				};
            
		}
	}
	return this->m_isActive;
}
void MotionAnimation::Finish() {
	/* 
	 * Set the current value to the desired
	 * value and stop the animation sequence.
	 */
	this->m_isActive = false;
	this->m_elapsed = 0.0f;
	this->m_duration = 0.0f;
	*(this->m_current_value) = this->m_desired_value;
}




float Back::EaseIn(float t, float b, float c, float d, float s) {
	if (!s)
		s = 1.70158;
	
	return c * (t /= d) * t * ((s + 1) * t - s) + b;
}
float Back::EaseOut(float t, float b, float c, float d, float s) {
	if (!s)
		s = 1.70158;
	
	return c * ((t = t / d - 1) * t * ((s + 1) * t + s) + 1) + b;
}
float Back::EaseInOut(float t, float b, float c, float d, float s) {
	if (!s)
		s = 1.70158; 
	
	if ((t /= d / 2) < 1)
		return c / 2 * (t * t * (((s *= (1.525)) + 1) * t - s)) + b;
	
	return c / 2 * ((t -= 2) * t * (((s *= (1.525)) + 1) * t + s) + 2) + b;
}


float Elastic::EaseIn(float t, float b, float c, float d, float a, float p) {
	if (t == 0)
		return b;
	
	if ((t /= d) == 1)
		return b + c;
	
	if (!p)
		p = d * 0.3;
	
	float s;
	if (!a || a < std::fabs(c))
	{
		a = c;
		s = p / 4;
	}
	else
	{
		s = p / (2 * 3.14159) * std::asin(c / a);
	}
	
	return -(a * std::pow(2, 10 * (t -= 1)) *
		std::sin((t * d - s) * (2 * 3.14159) / p)) + b;
}
float Elastic::EaseOut(float t, float b, float c, float d, float a, float p) {
	if (t == 0)
		return b;
	
	if ((t /= d) == 1)
		return b + c;
	
	if (!p)
		p = d * 0.3;
	
	float s;
	if (!a || a < std::fabs(c))
	{
		a = c;
		s = p / 4;
	}
	else
	{
		s = p / (2 * 3.14159) * std::asin(c / a);
	}
	
	return a * std::pow(2, -10 * t) *
		std::sin((t * d - s) * (2 * 3.14159) / p) + c + b;
}
float Elastic::EaseInOut(float t, float b, float c, float d, float a, float p) {
	if (t == 0)
		return b;
	
	if ((t /= d / 2) == 2)
		return b + c;
	
	if (!p)
		p = d * (0.3 * 1.5);
	
	float s;
	if (!a || a < std::fabs(c))
	{
		a = c;
		s = p / 4;
	}
	else
	{
		s = p / (2 * 3.14159) * std::asin(c / a);
	}
	
	if (t < 1)
	{
		return -0.5 * (a * std::pow(2, 10 * (t -= 1)) *
			std::sin((t * d - s) * (2 * 3.14159) / p)) + b;
	}
	
	return a * std::pow(2, -10 * (t -= 1)) *
		std::sin((t * d - s) * (2 * 3.14159) / p) * 0.5 + c + b;
}



float Linear::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Linear::EaseIn(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Linear::EaseOut(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Linear::EaseInOut(float t, float b, float c, float d) {
	return c * t / d + b;	
}


float Bounce::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Bounce::EaseIn(float t, float b, float c, float d) { 
	return c - this->EaseOut(d - t, 0, c, d) + b; 
}
float Bounce::EaseOut(float t, float b, float c, float d) {
	if ((t /= d) < (1 / 2.75))
		return c * (7.5625 * t * t) + b;
	else if (t < (2 / 2.75))
		return c * (7.5625 * (t -= (1.5 / 2.75)) * t + 0.75) + b;
	else if (t < (2.5 / 2.75))
		return c * (7.5625 * (t -= (2.25 / 2.75)) * t + 0.9375) + b;
	else 
		return c * (7.5625 * (t -= (2.625 / 2.75)) * t + 0.984375) + b;	
}
float Bounce::EaseInOut(float t, float b, float c, float d) {
	if (t < d/2)
		return this->EaseIn(t * 2, 0, c, d) * 0.5 + b;
	else 
		return this->EaseOut(t * 2 - d, 0, c, d) * 0.5 + c * 0.5 + b;
}


float Quadratic::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Quadratic::EaseIn(float t, float b, float c, float d) {
	t /= d;
	return c*t*t + b;
}
float Quadratic::EaseOut(float t, float b, float c, float d) {
	t /= d;
	return -c * t * (t-2) + b;
}
float Quadratic::EaseInOut(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t * (t-2) - 1) + b;
}


float Cubic::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Cubic::EaseIn(float t, float b, float c, float d) {
	t /= d;
	return c*t*t*t+b;
}
float Cubic::EaseOut(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c*(t*t*t + 1) + b;
}
float Cubic::EaseInOut(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t*t + b;
	t -= 2;
	return c/2*(t*t*t + 2) + b;
}


float Quartic::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Quartic::EaseIn(float t, float b, float c, float d) {
	t /= d;
	return c*t*t*t*t + b;
}
float Quartic::EaseOut(float t, float b, float c, float d) {
	t /= d;
	t--;
	return -c * (t*t*t*t - 1) + b;
}
float Quartic::EaseInOut(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t*t*t + b;
	t -= 2;
	return -c/2 * (t*t*t*t - 2) + b;
}


float Quintic::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Quintic::EaseIn(float t, float b, float c, float d) {
	t /= d;
	return c*t*t*t*t*t + b;
}
float Quintic::EaseOut(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c*(t*t*t*t*t + 1) + b;
}
float Quintic::EaseInOut(float t, float b, float c, float d) {
	t /= d/2;
	if (t < 1) return c/2*t*t*t*t*t + b;
	t -= 2;
	return c/2*(t*t*t*t*t + 2) + b;
}


float Sinusoidal::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Sinusoidal::EaseIn(float t, float b, float c, float d) {
	return -c * std::cos(t / d * (3.14159 / 2)) + c + b;
}
float Sinusoidal::EaseOut(float t, float b, float c, float d) {
	return c * std::sin(t / d * (3.14159 / 2)) + b;
}
float Sinusoidal::EaseInOut(float t, float b, float c, float d) {
	return -c / 2 * (std::cos(3.14159*t / d) - 1) + b;
}


float Exponential::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Exponential::EaseIn(float t, float b, float c, float d) { 
	return c * std::pow(2.0f, 10 * (t / d - 1)) + b;
}
float Exponential::EaseOut(float t, float b, float c, float d) { 
	return c * (-std::pow(2, -10 * t / d) + 1) + b;
}
float Exponential::EaseInOut(float t, float b, float c, float d) { 
	t /= d/2; 
	if (t < 1) return c / 2 * std::pow(2, 10 * (t - 1)) + b;
	t--; 
	return c / 2 * (-std::pow(2, -10 * t) + 2) + b;
}


float Circular::EaseNone(float t, float b, float c, float d) {
	return c * t / d + b;
}
float Circular::EaseIn(float t, float b, float c, float d) { 
	t /= d; 
	return -c * (std::sqrt(1 - t*t) - 1) + b; 
}
float Circular::EaseOut(float t, float b, float c, float d) { 
	t /= d; 
	t--; 
	return c * std::sqrt(1 - t*t) + b;
}
float Circular::EaseInOut(float t, float b, float c, float d) { 
	t /= d/2; 
	if (t < 1) return -c / 2 * (std::sqrt(1 - t*t) - 1) + b;
	t -= 2; 
	return c / 2 * (std::sqrt(1 - t*t) + 1) + b;
}

