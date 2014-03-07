#ifndef __GRAPHICS_COMMON_H__
#define __GRAPHICS_COMMON_H__

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
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
	GLfloat x;
	GLfloat y;
} Vector2Df;

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Vector3Df;

typedef struct {
	Vector2Df position;
	Vector2Df direction;
	Color4f color;
	Color4f deltaColor;
	GLfloat size;
	GLfloat timeToLive;
} Particle;

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
	Vector2Df origin;
	Size2Df size;
} Rect2Df;

typedef struct { 
    Vector3Df origin;
    Size3Df size;
} Rect3Df;

typedef struct {
	Texture2D* texture;
	GLuint retainCount;
	std::string name;
} Texture;

typedef struct { 
	Vector2Df tl;
	Vector2Df tr;
	Vector2Df bl;
	Vector2Df br;
} Quad2Df;

typedef struct {
	Vector3Df tl;
	Vector3Df tr;
	Vector3Df bl;
	Vector3Df br;
} Quad3Df;

typedef std::vector<Texture> TextureCollection;

typedef struct {
	Vector2Df geometryVertex;
	Color4f	  vertexColor;
	Vector2Df textureVertex;
} TexturedColoredVertex;

typedef struct {
	TexturedColoredVertex vertex1;
	TexturedColoredVertex vertex2;
	TexturedColoredVertex vertex3;
	TexturedColoredVertex vertex4;
} TexturedColoredQuad;

typedef struct {
	TexturedColoredQuad *texturedColoredQuad;
	TexturedColoredQuad *texturedColoredQuadIVA;
	GLuint textureName;
} ImageDetails;

static const Size2Df Size2DfZero = {1.0f, 1.0f};

static inline Size2Df Size2DfMake(GLfloat w, GLfloat h) { Size2Df s = { w, h }; return s; }

static const Size3Df Size3DfZero = {0.0f, 0.0f, 0.0f};

static inline Size3Df Size3DfMake(GLfloat w, GLfloat h, GLfloat d) { Size3Df s = {w, h, d}; return s; } 

static const Color4f Color4fZero = {1.0f, 1.0f, 1.0f, 1.0f};

static inline Color4f Color4fMake(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { Color4f c = {r, g, b, a}; return c; }

static const Vector2Df Vector2DfZero = {0.0f, 0.0f};

static inline Vector2Df Vector2DfMake(GLfloat x, GLfloat y) { Vector2Df v = {x, y}; return v; }

static const Vector3Df Vector3DfZero = {0.0f, 0.0f, 0.0f};

static inline Vector3Df Vector3DfMake(GLfloat x, GLfloat y, GLfloat z) { Vector3Df v = {x, y, z}; return v; }

static const Rect2Df Rect2DfZero = {Vector2DfZero, Size2DfZero};

static inline Rect2Df Rect2DfMake(GLfloat x, GLfloat y, GLfloat w, GLfloat h) { Rect2Df r = {Vector2DfMake(x, y), Size2DfMake(w, h)}; return r; }

static const Rect3Df Rect3DfZero = {Vector3DfZero, Size3DfZero};

static inline Rect3Df Rect3DfMake(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d) { Rect3Df r = { Vector3DfMake(x, y, z), Size3DfMake(w, h, d)}; return r; }


static inline Vector2Df Vector2DfMultiply(Vector2Df v, GLfloat s) { Vector2Df vect = {v.x * s, v.y * s}; return vect; }

static inline Vector2Df Vector2DfAdd(Vector2Df v1, Vector2Df v2) { Vector2Df vect = {v1.x + v2.x, v1.y + v2.y}; return vect; }

static inline Vector2Df Vector2DfSub(Vector2Df v1, Vector2Df v2) { Vector2Df v = {v1.x - v2.x, v1.y - v2.y}; return v; }

static inline GLfloat Vector2DfDot(Vector2Df v1, Vector2Df v2) { return (GLfloat) v1.x * v2.x + v1.y * v2.y; }

static inline GLfloat Vector2DfLength(Vector2Df v) { return (GLfloat) sqrtf(Vector2DfDot(v, v)); }

static inline Vector2Df Vector2DfNormalize(Vector2Df v) { return Vector2DfMultiply(v, 1.0f/Vector2DfLength(v)); }

// Loads the supplied matrix with the identity matrix
static inline void loadIdentityMatrix(float aMatrix[]) {
    aMatrix[0] = 1.0f; aMatrix[1] = 0.0f; aMatrix[2] = 0.0f;
    aMatrix[3] = 0.0f; aMatrix[4] = 1.0f; aMatrix[5] = 0.0f;
    aMatrix[6] = 0.0f; aMatrix[7] = 0.0f; aMatrix[8] = 1.0f;
}

// Applies a rotation transformation to the supplied matrix.  A translation is also applied
// which allows the rotation to take place around a specific point.
static inline void rotateMatrix(float aMatrix[], Vector2Df aRotationPoint, float aRotation) {
    // Translate into the image to the point around which rotation will occur
    aMatrix[6] = aRotationPoint.x * aMatrix[0] + aRotationPoint.y * aMatrix[3] + aMatrix[6];
    aMatrix[7] = aRotationPoint.x * aMatrix[1] + aRotationPoint.y * aMatrix[4] + aMatrix[7];
    
    // Turns the degrees passed in to radians for the calculations
    float radians = DEGREES_TO_RADIANS(aRotation);
    
    // Calculate the sin and cos for the rotation
	float cosTheta = cosf(radians);
	float sinTheta = sinf(radians);
    
    // Take a copy of the matrix as we will need this during the calculation
	float m0 = aMatrix[0];
	float m1 = aMatrix[1];
	float m2 = aMatrix[2];
	float m3 = aMatrix[3];
	float m4 = aMatrix[4];
	float m5 = aMatrix[5];
    
    // Perform the rotation transformation on the images matrix
	aMatrix[0] = cosTheta * m0 + sinTheta * m3;
	aMatrix[1] = cosTheta * m1 + sinTheta * m4;
	aMatrix[2] = cosTheta * m2 + sinTheta * m5;
	aMatrix[3] = -sinTheta * m0 + cosTheta * m3;
	aMatrix[4] = -sinTheta * m1 + cosTheta * m4;
	aMatrix[5] = -sinTheta * m2 + cosTheta * m5;   
    
    // Now reverse the translation we did to the point of rotation
    aMatrix[6] = -aRotationPoint.x * aMatrix[0] + -aRotationPoint.y * aMatrix[3] + aMatrix[6];
    aMatrix[7] = -aRotationPoint.x * aMatrix[1] + -aRotationPoint.y * aMatrix[4] + aMatrix[7];
}

// Applies the scale transformation to the matrix using the scale information provided in |aScale|
static inline void scaleMatrix(float aMatrix[], Size2Df aScale) {
    // Perform the scale transformation on the images matrix
    aMatrix[0] *= aScale.width;
    aMatrix[1] *= aScale.width;
    aMatrix[2] *= aScale.width;
    aMatrix[3] *= aScale.height;
    aMatrix[4] *= aScale.height;
    aMatrix[5] *= aScale.height;
}

// Applies the translation transformation to the matrix using |aPoint|
static inline void translateMatrix(float aMatrix[], Vector2Df aPoint) {
    aMatrix[6] = aPoint.x * aMatrix[0] + aPoint.y * aMatrix[3] + aMatrix[6];
    aMatrix[7] = aPoint.x * aMatrix[1] + aPoint.y * aMatrix[4] + aMatrix[7];
}

// Transforms the vertices in |aQuad| using |aMatrix| with the results being loaded into |aTransformedQuad|
static inline void transformMatrix(float aMatrix[], TexturedColoredQuad *aQuad, TexturedColoredQuad *aTransformedQuad) {
	aTransformedQuad->vertex1.geometryVertex.x = aQuad->vertex1.geometryVertex.x * aMatrix[0] + aQuad->vertex1.geometryVertex.y * aMatrix[3] + aMatrix[6];
	aTransformedQuad->vertex1.geometryVertex.y = aQuad->vertex1.geometryVertex.x * aMatrix[1] + aQuad->vertex1.geometryVertex.y * aMatrix[4] + aMatrix[7];
    
	aTransformedQuad->vertex2.geometryVertex.x = aQuad->vertex2.geometryVertex.x * aMatrix[0] + aQuad->vertex2.geometryVertex.y * aMatrix[3] + aMatrix[6];
	aTransformedQuad->vertex2.geometryVertex.y = aQuad->vertex2.geometryVertex.x * aMatrix[1] + aQuad->vertex2.geometryVertex.y * aMatrix[4] + aMatrix[7];
	
	aTransformedQuad->vertex3.geometryVertex.x = aQuad->vertex3.geometryVertex.x * aMatrix[0] + aQuad->vertex3.geometryVertex.y * aMatrix[3] + aMatrix[6];
	aTransformedQuad->vertex3.geometryVertex.y = aQuad->vertex3.geometryVertex.x * aMatrix[1] + aQuad->vertex3.geometryVertex.y * aMatrix[4] + aMatrix[7];
	
	aTransformedQuad->vertex4.geometryVertex.x = aQuad->vertex4.geometryVertex.x * aMatrix[0] + aQuad->vertex4.geometryVertex.y * aMatrix[3] + aMatrix[6];
	aTransformedQuad->vertex4.geometryVertex.y = aQuad->vertex4.geometryVertex.x * aMatrix[1] + aQuad->vertex4.geometryVertex.y * aMatrix[4] + aMatrix[7];
}

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