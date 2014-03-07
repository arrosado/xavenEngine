#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__

#include <string>
#include <Game/Graphics/Image/Image.h>
#include <Game/Graphics/GraphicsCommon.h>

typedef struct _PointSprite {
	GLfloat x;
	GLfloat y;
	GLfloat size;
} PointSprite;

class ParticleEmitter {
public:
	Image		*m_image;
	Vector2Df	m_sourcePosition;
	Vector2Df	m_sourcePositionVariance;
	GLfloat		m_angle;
	GLfloat		m_angleVariance;
	GLfloat		m_speed;
	GLfloat		m_speedVariance;
	Vector2Df	m_gravity;
	GLfloat		m_particleLifespan;
	GLfloat		m_particleLifespanVariance;
	Color4f		m_startColor;
	Color4f		m_startColorVariance;
	Color4f		m_finishColor;
	Color4f		m_finishColorVariance;
	GLfloat		m_particleSize;
	GLfloat		m_particleSizeVariance;
	GLuint		m_maxParticles;
	GLint		m_particleCount;
	GLfloat		m_emissionRate;
	GLfloat		m_emitCounter;
	GLuint		m_verticesID;
	GLuint		m_colorsID;
	Particle	*m_particles;
	PointSprite	*m_vertices;
	Color4f		*m_colors;
	bool		m_active;
	GLint		m_particleIndex;
	bool		m_useTexture;
	GLfloat		m_elapsedTime;
	GLfloat		m_duration;
	bool		m_blendAdditive;
	
	ParticleEmitter(const string& imageName, 
						Vector2Df position, 
						Vector2Df sourcePositionVariance,
						GLfloat	  speed,
						GLfloat	  speedVariance,
						GLfloat	  particleLifeSpan,
						GLfloat	  particleLifeSpanVariance,
						GLfloat	  angle,
						GLfloat	  angleVariance,
						Vector2Df gravity,
						Color4f	  startColor, 
						Color4f	  startColorVariance,
						Color4f	  finishColor,
						Color4f	  finishColorVariance,
						GLuint	  maxParticles,
						GLfloat	  particleSize,
						GLfloat	  particleSizeVariance,
						GLfloat	  duration,
						bool	  blendAdditive);
	
	~ParticleEmitter();
	
	void Render();
	void Update(GLfloat delta);
	bool AddParticle();
	void InitParticle(Particle* particle);
	void StopParticleEmitter();
	
};
#endif /* __PARTICLE_EMITTER_H__ */
