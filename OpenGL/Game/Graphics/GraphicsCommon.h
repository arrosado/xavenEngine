#ifndef __GRAPHICS_COMMON_H__
#define __GRAPHICS_COMMON_H__

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#ifdef WIN32
#include <gl/glut.h>
#define PATH_SEPARATOR "\\"
#else
#include <glut.h>
#define PATH_SEPARATOR "/"
#endif

#define RESOURCES_FOLDER "." PATH_SEPARATOR "Resources" PATH_SEPARATOR

#define MAX_IMAGES_PER_FRAME 1601
#define MAX_TEXTURES_PER_APP 20


#define MAX_MAP_WIDTH 200
#define MAX_MAP_HEIGHT 200

// Macro which returns a random value between -1 and 1
#define RANDOM_MINUS_1_TO_1() ((rand() / (GLfloat)0x3fffffff )-1.0f)

// Macro which returns a random number between 0 and 1
#define RANDOM_0_TO_1() ((rand() / (GLfloat)0x7fffffff ))

// Macro which converts degrees into radians
#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)

#define RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

/****************** Graphic Types Definitions ****************/

using namespace glm;

class Texture2D;

typedef enum {
	kGameScreenOrientation_Portrait           = 0,
	kGameScreenOrientation_PortraitUpsideDown = 180,
	kGameScreenOrientation_LandscapeLeft      = 90,
	kGameScreenOrientation_LandscapeRight     = 270
} GameScreenOrientation;

typedef struct {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;
} Color4f;

static const Color4f Color4fWindowsBlue = {0.39f, 0.58f, 0.920f, 1.0f};
static const Color4f Color4fWhite = {1.0f, 1.0f, 1.0f, 1.0f};
static const Color4f Color4fBlack = {0.0f, 0.0f, 0.0f, 1.0f};
static const Color4f Color4fGreen = {0.0f, 1.0f, 0.0f, 1.0f};
static const Color4f Color4fRed = {1.0f, 0.0f, 0.0f, 1.0f};
static const Color4f Color4fBlue = {0.0f, 0.0f, 1.0f, 1.0f};
static const Color4f Color4fGray = {0.5f, 0.5f, 0.5f, 1.0f};

typedef struct {
	GLfloat width;
	GLfloat height;
} Size2Df;

typedef struct {
    GLfloat width;
    GLfloat height;
    GLfloat depth;
} Size3Df;

typedef struct {
	Texture2D* data;
	GLuint retainCount;
	std::string name;
} Texture;

typedef std::vector<Texture> TextureCollection;

typedef struct {
	vec3 geometry;
	Color4f	color;
	vec2 texture;
} Vertex;

typedef struct {
	Vertex vertex1;
	Vertex vertex2;
	Vertex vertex3;
	Vertex vertex4;
    Texture texture;
} Image;

static const Size2Df Size2DfZero = {1.0f, 1.0f};

static inline Size2Df Size2DfMake(GLfloat w, GLfloat h) { Size2Df s = { w, h }; return s; }

static const Size3Df Size3DfZero = {0.0f, 0.0f, 0.0f};

static inline Size3Df Size3DfMake(GLfloat w, GLfloat h, GLfloat d) { Size3Df s = {w, h, d}; return s; } 

static const Color4f Color4fZero = {1.0f, 1.0f, 1.0f, 1.0f};

static inline Color4f Color4fMake(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { Color4f c = {r, g, b, a}; return c; }

/* GLUT functions */
static inline void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

static inline void __gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat m[4][4];
    GLfloat sine, cotangent, deltaZ;
    GLfloat radians = fovy / 2 * 3.14 / 180;
    
    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
    {
        return;
    }
    cotangent = cos(radians) / sine;
    
    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1;
    m[3][2] = -2 * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixf(&m[0][0]);
}

// This is a modified version of the function of the same name from 
// the Mesa3D project ( http://mesa3d.org/ ), which is  licensed
// under the MIT license, which allows use, modification, and 
// redistribution.
static inline void __gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
							 GLfloat centerx, GLfloat centery, GLfloat centerz,
							 GLfloat upx, GLfloat upy, GLfloat upz)
{
	GLfloat m[16];
	GLfloat x[3], y[3], z[3];
	GLfloat mag;
	
	/* Make rotation matrix */
	
	/* Z vector */
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	mag = sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
	if (mag) {			/* mpichler, 19950515 */
		z[0] /= mag;
		z[1] /= mag;
		z[2] /= mag;
	}
	
	/* Y vector */
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;
	
	/* X vector = Y cross Z */
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];
	
	/* Recompute Y = Z cross X */
	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];
	
	/* mpichler, 19950515 */
	/* cross product gives area of parallelogram, which is < 1.0 for
	 * non-perpendicular unit-length vectors; so normalize x, y here
	 */
	
	mag = sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
	if (mag) {
		x[0] /= mag;
		x[1] /= mag;
		x[2] /= mag;
	}
	
	mag = sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
	if (mag) {
		y[0] /= mag;
		y[1] /= mag;
		y[2] /= mag;
	}
	
#define M(row,col)  m[col*4+row]
	M(0, 0) = x[0];
	M(0, 1) = x[1];
	M(0, 2) = x[2];
	M(0, 3) = 0.0;
	M(1, 0) = y[0];
	M(1, 1) = y[1];
	M(1, 2) = y[2];
	M(1, 3) = 0.0;
	M(2, 0) = z[0];
	M(2, 1) = z[1];
	M(2, 2) = z[2];
	M(2, 3) = 0.0;
	M(3, 0) = 0.0;
	M(3, 1) = 0.0;
	M(3, 2) = 0.0;
	M(3, 3) = 1.0;
#undef M
	glMultMatrixf(m);
	
	/* Translate Eye to Origin */
	glTranslatef(-eyex, -eyey, -eyez);
	
}

#endif /* __GRAPHICS_COMMON_H__ */