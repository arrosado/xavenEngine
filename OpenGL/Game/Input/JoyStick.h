//
//  JoyStick.h
//  ARR Game Engine Core
//
//  Created by Ariel Rosado on 3/13/11.
//  Copyright 2011 Xaven. All rights reserved.
//


#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdio.h>
#include <Game/Graphics/GraphicsCommon.h>


typedef struct sJoyStick {
	
private:
	
	/* Define a structure to hold the 
	 * thredshold needed to achive a
	 * JoyStick level.
	 */
	typedef struct sThredshold {
		float Up; // +Y = iPhone portrait upper side.
		float Down; // -Y = iPhone portrait homebutton side.
		float Right; // +X = iPhone portrait right side.
		float Left; // -X = iPhone portrait left side.
		float Forward; // +Z = iPhone closer to the user.
		float Backward; // -Z = iPhone farther to the user.
	} Thredshold;
	
	/* This Joystick has 4 levels, The 
	 * level1 thredshold structure defines 
	 * the minimum thredshold to start moving.
	 */
	Thredshold level1;
	Thredshold level2;
	Thredshold level3;
	Thredshold level4;
	
	/* Keep the calibration coordinates to recalculate 
	 * the calibration in the case that the controller
	 * sensibility changes.
	 */
	Vector3Df m_calibrationCoords;
	
	/* Keeps the JoyStick sensibility, This sensibility
	 * is used to calculate the joystick calibration.
	 */
	unsigned int m_sensibility;
	
	/* Keeps Track of the initialization process and 
	 * gives users track of initialization. Its also used
	 * to trigger recalibration.
	 */
	bool m_isInitialized;
	
public:

	typedef enum eJoyStickSpeeds {
		Stopped,
		Slow,
		Normal,
		Moderate,
		Fast
	} JoyStickSpeed;

private:
	
	typedef struct sJoyStickSpeeds {
		JoyStickSpeed Upward;
		JoyStickSpeed Downward;
		JoyStickSpeed Rightward;
		JoyStickSpeed Leftward;
		JoyStickSpeed Forward;
		JoyStickSpeed Backward;
		void Reset() {
			this->Upward = Stopped;
			this->Downward = Stopped;
			this->Rightward = Stopped;
			this->Leftward = Stopped;
			this->Forward = Stopped;
			this->Backward = Stopped;
		};
	} JoyStickSpeeds;
	
	
public:

	JoyStickSpeeds Speed;
	
	void Initialize(Vector3Df point, unsigned int sensibility) {
		printf("Initializing Joystick....\n");
		this->m_sensibility = sensibility;
		this->Calibrate(point.x, point.y, point.z);	
		this->Speed.Reset();
		this->m_isInitialized = true;
		printf("Joystick Initialized!\n");
	};
	
	bool IsInitialized() {
		return this->m_isInitialized;
	};
	
	void Recalibrate() {
		printf("Sending Recalibrate Message!\n");
		this->m_isInitialized = false;
	}
	
	void Calibrate(const float x, const float y, const float z) {
		printf("Calibrating to x = %f y = %f z = %f\n", x, y , z);
		/* Save coordinates for recalibration if the sensibility changes. */
		this->m_calibrationCoords.x = x;
		this->m_calibrationCoords.y = y;
		this->m_calibrationCoords.z = z;
		
		/* Start Calibrating Controller. */
		
		/* Calculate the level1 thredshold. */
		this->level1.Up		  = y + this->m_sensibility;
		this->level1.Down	  = y - this->m_sensibility;
		this->level1.Right	  = x + this->m_sensibility;
		this->level1.Left	  = x - this->m_sensibility;
		this->level1.Forward  = z + this->m_sensibility;
		this->level1.Backward = z - this->m_sensibility;
		
		/* Calculate the level2 thredshold. */
		this->level2.Up		  = y + (this->m_sensibility*2);
		this->level2.Down	  = y - (this->m_sensibility*2);
		this->level2.Right    = x + (this->m_sensibility*2);
		this->level2.Left	  = x - (this->m_sensibility*2);
		this->level2.Forward  = z + (this->m_sensibility*2);
		this->level2.Backward = z - (this->m_sensibility*2);
		
		/* Calculate the level3 thredshold. */
		this->level3.Up		  = y + (this->m_sensibility*3);
		this->level3.Down	  = y - (this->m_sensibility*3);
		this->level3.Right    = x + (this->m_sensibility*3);
		this->level3.Left	  = x - (this->m_sensibility*3);
		this->level3.Forward  = z + (this->m_sensibility*3);
		this->level3.Backward = z - (this->m_sensibility*3);
		
		/* Calculate the level4 thredshold. */
		this->level4.Up		  = y + (this->m_sensibility*4);
		this->level4.Down	  = y - (this->m_sensibility*4);
		this->level4.Right	  = x + (this->m_sensibility*4);
		this->level4.Left	  = x - (this->m_sensibility*4);
		this->level4.Forward  = z + (this->m_sensibility*4);
		this->level4.Backward = z - (this->m_sensibility*4);
		printf("Calibration Done!\n\n");
	};
	
	void IncreaseSencibility() {
		this->m_sensibility++;
		this->Calibrate(this->m_calibrationCoords.x, this->m_calibrationCoords.y, this->m_calibrationCoords.z);
		printf("Sensibility Increase %d\n", this->m_sensibility);
	};
	
	void DecreaseSencibility() {
		this->m_sensibility--;
		this->Calibrate(this->m_calibrationCoords.x, this->m_calibrationCoords.y, this->m_calibrationCoords.z);
		printf("Sensibility Decrease %d\n", this->m_sensibility);
	};
	
	void Update(Vector3Df p) {
		this->Speed.Reset();
		
		/* Check if its moving upward. */
		if (this->CanMoveUp(p.y)){
			/* Update the upward speed. */
			if (p.y > this->level4.Up) 
				this->Speed.Upward = Fast;
			else if (p.y > this->level3.Up) 
				this->Speed.Upward = Moderate;
			else if (p.y > this->level2.Up) 
				this->Speed.Upward = Normal;
			else 
				this->Speed.Upward = Slow;
		} else if (this->CanMoveDown(p.y)) { /* If its not moving upward, check if its moving downward. */
			/* Update the downward speed. */
			if (p.y < this->level4.Down) 
				this->Speed.Downward = Fast;
			else if (p.y < this->level3.Down) 
				this->Speed.Downward = Moderate;
			else if (p.y < this->level2.Down) 
				this->Speed.Downward = Normal;
			else 
				this->Speed.Downward = Slow;
		}
		
		/* Check if its moving to the rightward. */
		if (this->CanMoveRight(p.x)) {
			/* Update the rightward speed. */
			if (p.x > this->level4.Right) 
				this->Speed.Rightward = Fast;
			else if (p.x > this->level3.Right) 
				this->Speed.Rightward = Moderate;
			else if (p.x > this->level2.Right)
				this->Speed.Rightward = Normal;
			else
				this->Speed.Rightward = Slow;
		} else if (this->CanMoveLeft(p.x)) { /* If its not moving rightward, check if its moving leftward. */
			/* Update the leftward speed. */
			if (p.x < this->level4.Left) 
				this->Speed.Leftward = Fast;
			else if (p.x < this->level3.Left)
				this->Speed.Leftward = Moderate;
			else if (p.x < this->level2.Left)
				this->Speed.Leftward = Normal;
			else 
				this->Speed.Leftward = Slow;
		}
		
		/* Check if its moving forward. */
		if (this->CanMoveForward(p.z)) {
			/* Update the forward speed. */
			if (p.z > this->level4.Forward) 
				this->Speed.Forward = Fast;
			else if (p.z > this->level3.Forward) 
				this->Speed.Forward = Moderate;
			else if (p.z > this->level2.Forward)
				this->Speed.Forward = Normal;
			else 		
				this->Speed.Forward = Slow;
		} else if (this->CanMoveBackward(p.z)) { /* If its not moving forward, check if its moving backward. */
			/* Update the backward speed. */
			if (p.z < this->level4.Backward)
				this->Speed.Backward = Fast;
			else if (p.z < this->level3.Backward)
				this->Speed.Backward = Moderate;
			else if (p.z < this->level2.Backward)
				this->Speed.Backward = Normal;
			else
				this->Speed.Backward = Slow;
		}
	};
	
	bool CanMoveUp(float y)		{ return (y > this->level1.Up);	};
	bool CanMoveDown(float y)	{ return (y < this->level1.Down); };
	bool CanMoveRight(float x)	{ return (x > this->level1.Right); };
	bool CanMoveLeft(float x)	{ return (x < this->level1.Left); };
	bool CanMoveForward(float z) { return (z > this->level1.Forward); };
	bool CanMoveBackward(float z) { return (z < this->level1.Backward); };
	
} GameJoyStick;


#endif 