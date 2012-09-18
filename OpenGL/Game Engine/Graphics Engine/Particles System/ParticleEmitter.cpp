/*
 *  ParticleEmitter.cpp
 *  ARR Game Engine Core
 *
 *  Created by Ariel Rosado on 12/4/09.
 *  Copyright 2009 Xaven. All rights reserved.
 *
 */

#include <Graphics Engine\Particles System\ParticleEmitter.h>

ParticleEmitter::ParticleEmitter(
				const string& imageName, 
				Vector2Df position, 
				Vector2Df sourcePositionVariance,
				GLfloat speed,
				GLfloat speedVariance,
				GLfloat	particleLifeSpan,
				GLfloat	particleLifeSpanVariance,
				GLfloat	angle,
				GLfloat	angleVariance,
				Vector2Df gravity,
				Color4f	startColor, 
				Color4f	startColorVariance,
				Color4f	finishColor,
				Color4f	finishColorVariance,
				GLuint	maxParticles,
				GLfloat	particleSize,
				GLfloat	particleSizeVariance,
				GLfloat	duration,
				bool blendAdditive) {

	// If the texture name is not null, then allocate the texture for the points.  If the texture name is null
	// and we do not assign a texture, then a quad will be drawn instead	
	// Create a new texture which is going to be used as the texture for the point sprites
	this->m_image = new Image(imageName, GL_NEAREST);
	
	// Take the parameters which have been passed into this particle emitter and set the emitters
	// properties
	this->m_sourcePosition = position;
	this->m_sourcePositionVariance = sourcePositionVariance;
	this->m_speed = speed;
	this->m_speedVariance = speedVariance;
	this->m_particleLifespan = particleLifeSpan;
	this->m_particleLifespanVariance = particleLifeSpanVariance;
	this->m_angle = angle;
	this->m_angleVariance = angleVariance;
	this->m_gravity = gravity;
	this->m_startColor = startColor;
	this->m_startColorVariance = startColorVariance;
	this->m_finishColor = finishColor;
	this->m_finishColorVariance = finishColorVariance;
	this->m_maxParticles = maxParticles;
	this->m_particleSize = particleSize;
	this->m_particleSizeVariance = particleSizeVariance;
	this->m_emissionRate = this->m_maxParticles/this->m_particleLifespan;
	this->m_duration = duration;
	this->m_blendAdditive = blendAdditive;
	
	// Allocate the memory necessary for the particle emitter arrays
	this->m_particles = (Particle *)malloc( sizeof(Particle) * this->m_maxParticles);
	this->m_vertices = (PointSprite *)malloc( sizeof(PointSprite) * this->m_maxParticles);
	this->m_colors = (Color4f *)malloc( sizeof(Color4f) * this->m_maxParticles);

	// If one of the arrays cannot be allocated, then report a warning and return nil
	if(!(this->m_particles && this->m_vertices && this->m_colors)) {
		
		if(this->m_particles)
			free(this->m_particles);
		if(this->m_vertices)
			free(this->m_vertices);
		if(this->m_colors)
			free(this->m_colors);
		
		throw "WARNING: ParticleEmitter - Not enough memory";
	}
	
	// Reset the memory used for the particles array using zeros
	memset( this->m_particles, 0, sizeof(Particle) * this->m_maxParticles);
	memset( this->m_vertices, 0, sizeof(PointSprite) * this->m_maxParticles);
	memset( this->m_colors,	0, sizeof(Color4f) * this->m_maxParticles);
	

	// Generate the VBO's
	//glGenBuffers(1, &this->m_uiVerticesID);
	//glGenBuffers(1, &this->m_uiColorsID);
	
	// By default the particle emitter is active when created
	this->m_active = false;
	
	// Set the particle count to zero
	this->m_particleCount = 0;
	
	// Reset the elapsed time
	this->m_elapsedTime = 0;

}

ParticleEmitter::~ParticleEmitter() {
	// Release the memory we are using for our vertex and particle arrays etc
	free(this->m_vertices);
	free(this->m_colors);
	free(this->m_particles);
	//glDeleteBuffers(1, &this->m_uiVerticesID);
	//glDeleteBuffers(1, &this->m_uiColorsID);
	delete this->m_image;
}

void ParticleEmitter::Render() {
	/*
	// ToDo: verify all this code which is commented!!!
	// Enable texturing
	glEnable(GL_TEXTURE_2D);
	
	// Enable texturing and bind the texture to be used as the point sprite
	glBindTexture(GL_TEXTURE_2D, this->m_image->textureName);

	// Enable and configure blending
	glEnable(GL_BLEND);
	
	// Change the blend function used if blendAdditive has been set
	if(this->m_blendAdditive) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	} else {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	// Enable and configure point sprites which we are going to use for our particles
	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	
	// Enable vertex arrays and bind to the vertices VBO which has been created
	glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, this->m_uiVerticesID);
	
	// Configure the vertex pointer which will use the vertices VBO
	glVertexPointer(2, GL_FLOAT, sizeof(PointSprite), &this->m_vertices[0]);
	
	// Enable the point size array
	glEnableClientState(GL_POINT_SIZE_ARRAY);
	
	// Configure the point size pointer which will use the currently bound VBO.  PointSprite contains
	// both the location of the point as well as its size, so the config below tells the point size
	// pointer where in the currently bound VBO it can find the size for each point
	glPointSizePointer(GL_FLOAT,sizeof(PointSprite),(GLvoid*) (sizeof(GL_FLOAT)*2));
	
	// Enable the use of the color array
	glEnableClientState(GL_COLOR_ARRAY);
	
	// Bind to the color VBO which has been created
	//glBindBuffer(GL_ARRAY_BUFFER, this->m_uiColorsID);
	
	// Configure the color pointer specifying how many values there are for each color and their type
	glColorPointer(4, GL_FLOAT, 0, &this->m_colors[0]);
	
	// Now that all of the VBOs have been used to configure the vertices, pointer size and color
	// use glDrawArrays to draw the points
	glDrawArrays(GL_POINTS, 0, this->m_particleIndex);
	
	// Unbind the current VBO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Disable the client states which have been used incase the next draw function does 
	// not need or use them
	glDisableClientState(GL_POINT_SPRITE);
	glDisableClientState(GL_POINT_SIZE_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_POINT_SPRITE);
	*/
}

void ParticleEmitter::Update(GLfloat delta) {

	// If the emitter is active and the emission rate is greater than zero then emit
	// particles
	if(this->m_active && this->m_emissionRate != 0) {
		
		float rate = 0.1f/this->m_emissionRate;
		this->m_emitCounter += delta;
		
		while(this->m_particleCount < this->m_maxParticles && this->m_emitCounter > rate) {
			this->AddParticle();
			this->m_emitCounter -= rate;
		}
		
		this->m_elapsedTime += delta;
		if(this->m_duration != -1 && this->m_duration < this->m_elapsedTime)
			this->StopParticleEmitter();
	}
	
	// Reset the particle index before updating the particles in this emitter
	this->m_particleIndex = 0;
	
	// Loop through all the particles updating their location and color
	while(this->m_particleIndex < this->m_particleCount) {
		
		// Get the particle for the current particle index
		Particle *currentParticle = &this->m_particles[this->m_particleIndex];
		
		// If the current particle is alive then update it
		if(currentParticle->timeToLive > 0) {
			
			// Calculate the new direction based on gravity
			currentParticle->direction = Vector2DfAdd(currentParticle->direction, this->m_gravity);
			// Calculate the new position based on direction
			currentParticle->position = Vector2DfAdd(currentParticle->position, currentParticle->direction);
			
			// Reduce the life span of the particle
			currentParticle->timeToLive -= delta;
			
			// Place the position of the current particle into the vertices array
			this->m_vertices[this->m_particleIndex].x = currentParticle->position.x;
			this->m_vertices[this->m_particleIndex].y = currentParticle->position.y;
			
			// Place the size of the current particle in the size array
			this->m_vertices[this->m_particleIndex].size = currentParticle->size;
			
			// Update the particles color
			currentParticle->color.red += (currentParticle->deltaColor.red * delta);
			currentParticle->color.green += (currentParticle->deltaColor.green * delta);
			currentParticle->color.blue += (currentParticle->deltaColor.blue * delta);
			currentParticle->color.alpha += (currentParticle->deltaColor.alpha * delta);
			
			// Place the color of the current particle into the color array
			this->m_colors[this->m_particleIndex] = currentParticle->color;
			
			// Update the particle counter
			this->m_particleIndex++;
		} else {
			
			// As the particle is not alive anymore replace it with the last active particle 
			// in the array and reduce the count of particles by one.  This causes all active particles
			// to be packed together at the start of the array so that a particle which has run out of
			// life will only drop into this clause once
			if(this->m_particleIndex != this->m_particleCount-1)
				this->m_particles[this->m_particleIndex] = this->m_particles[this->m_particleCount-1];
			this->m_particleCount--;
		}
	}
	
	// Now we have updated all the particles, update the VBOs with the arrays we have just updated
	// So this goes like... Bind vertices array.
	//glBindBuffer(GL_ARRAY_BUFFER, this->m_verticesID);
	// Send vertices data to the graphics card.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(PointSprite)*this->m_maxParticles, this->m_vertices, GL_DYNAMIC_DRAW);
	// Bind colors array.
	//glBindBuffer(GL_ARRAY_BUFFER, this->m_colorsID);
	// Send the color data to the graphics card.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Color4f)*this->m_maxParticles, this->m_colors, GL_DYNAMIC_DRAW);
	// Unbind the currently bound array to stop any other commands from modifying the color array.
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool ParticleEmitter::AddParticle() {

	// If we have already reached the maximum number of particles then do nothing
	if(this->m_particleCount == this->m_maxParticles)
		return false;
	
	// Take the next particle out of the particle pool we have created and initialize it
	Particle *particle = &this->m_particles[this->m_particleCount];
	this->InitParticle(particle);
	
	// Increment the particle count
	this->m_particleCount++;

	// Return true to show that a particle has been created
	return true;

}

void ParticleEmitter::InitParticle(Particle* particle) {
	// Init the position of the particle.  This is based on the source position of the particle emitter
	// plus a configured variance.  The RANDOM_MINUS_1_TO_1 macro allows the number to be both positive
	// and negative
	particle->position.x = this->m_sourcePosition.x + this->m_sourcePositionVariance.x * RANDOM_MINUS_1_TO_1();
	particle->position.y = this->m_sourcePosition.y + this->m_sourcePositionVariance.y * RANDOM_MINUS_1_TO_1();
	
	// Init the direction of the particle.  The newAngle is calculated using the angle passed in and the
	// angle variance.
	float newAngle = (GLfloat)DEGREES_TO_RADIANS(this->m_angle + this->m_angleVariance * RANDOM_MINUS_1_TO_1());
	
	// Create a new Vector2f using the newAngle
	Vector2Df vector = Vector2DfMake(cosf(newAngle), sinf(newAngle));
	
	// Calculate the vectorSpeed using the speed and speedVariance which has been passed in
	float vectorSpeed = this->m_speed + this->m_speedVariance * RANDOM_MINUS_1_TO_1();
	
	// The particles direction vector is calculated by taking the vector calculated above and
	// multiplying that by the speed
	particle->direction = Vector2DfMultiply(vector, vectorSpeed);
	
	// Calculate the particle size using the particleSize and variance passed in
	particle->size = this->m_particleSize + this->m_particleSizeVariance * RANDOM_MINUS_1_TO_1();
	
	// Calculate the particles life span using the life span and variance passed in
	particle->timeToLive = this->m_particleLifespan + this->m_particleLifespanVariance * RANDOM_MINUS_1_TO_1();
	
	// Calculate the color the particle should have when it starts its life.  All the elements
	// of the start color passed in along with the variance as used to calculate the start color.
	Color4f start = {0, 0, 0, 0};
	start.red = this->m_startColor.red + this->m_startColorVariance.red * RANDOM_MINUS_1_TO_1();
	start.green = this->m_startColor.green + this->m_startColorVariance.green * RANDOM_MINUS_1_TO_1();
	start.blue = this->m_startColor.blue + this->m_startColorVariance.blue * RANDOM_MINUS_1_TO_1();
	start.alpha = this->m_startColor.alpha + this->m_startColorVariance.alpha * RANDOM_MINUS_1_TO_1();
	
	// Calculate the color the particle should be when its life is over.  This is done the same
	// way as the start color above.
	Color4f end = {0, 0, 0, 0};
	end.red = this->m_finishColor.red + this->m_finishColorVariance.red * RANDOM_MINUS_1_TO_1();
	end.green = this->m_finishColor.green + this->m_finishColorVariance.green * RANDOM_MINUS_1_TO_1();
	end.blue = this->m_finishColor.blue + this->m_finishColorVariance.blue * RANDOM_MINUS_1_TO_1();
	end.alpha = this->m_finishColor.alpha + this->m_finishColorVariance.alpha * RANDOM_MINUS_1_TO_1();
	
	// Calculate the delta which is to be applied to the particles color during each cycle of its
	// life. The delta calculation uses the life space of the particle to make sure that the 
	// particles color will transition from the start to end color during its life time.
	particle->color = start;
	particle->deltaColor.red = (end.red - start.red) / particle->timeToLive;
	particle->deltaColor.green = (end.green - start.green) / particle->timeToLive;
	particle->deltaColor.blue = (end.blue - start.blue) / particle->timeToLive;
	particle->deltaColor.alpha= (end.alpha - start.alpha) / particle->timeToLive;
}

void ParticleEmitter::StopParticleEmitter() {
	this->m_active = false;
	this->m_elapsedTime = 0;
	this->m_emitCounter = 0;
}
