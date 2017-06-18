#pragma once

////#include "../Headers/MyFiles.h"

#include "../glad/include/glad/glad.h"
#include "../glm/include/common.hpp"

#include <fstream>
#include <vector>
#include <string>

class FrameState; //FW Declaration

#pragma region Storage

// This is our 3D point class.  This will be used to store the vertices of our model.
struct Vector3 {
	float x, y, z;
};

// This is our 2D point class.  This will be used to store the UV coordinates.
struct Vector2 {
	float x, y;
};

#pragma endregion

#pragma region MD2
// These are the needed defines for the max values when loading .MD2 files
#define MD2_MAX_TRIANGLES		4096
#define MD2_MAX_VERTICES		2048
#define MD2_MAX_TEXCOORDS		2048
#define MD2_MAX_FRAMES			512
#define MD2_MAX_SKINS			32
#define MD2_MAX_FRAMESIZE		(MD2_MAX_VERTICES * 4 + 128)

// This stores the speed of the animation between each key frame
#define kAnimationSpeed			5.0f

// This holds the header information that is read in at the beginning of the file
struct MD2Header { 
	int magic;						// This is used to identify the file = 844121161
	int version;					// The version number of the file (Must be 8)
	int skinWidth;					// The skin width in pixels
	int skinHeight;					// The skin height in pixels
	int frameSize;					// The size in bytes the frames are
	
	int numSkins;					// The number of skins associated with the model
	int numVertices;				// The number of vertices (constant for each frame)
	int numTexCoords;				// The number of texture coordinates
	int numTriangles;				// The number of faces (polygons)
	int numGlCommands;				// The number of GL commands
	int numFrames;					// The number of animation frames
	
	int offsetSkins;				// The offset in the file for the skin data
	int offsetTexCoords;			// The offset in the file for the texture data
	int offsetTriangles;			// The offset in the file for the face data
	int offsetFrames;				// The offset in the file for the frames data
	int offsetGlCommands;			// The offset in the file for the GL commands data
	int offsetEnd;					// The end of the file offset
};

// This stores the normals and vertices for the frames
struct MD2Vertex {
	unsigned char vertex[3];
	unsigned char normal;
};

struct MD2Triangle {
	unsigned short int vertex[3];
	unsigned short int st[3];
} ;

// This stores UV coordinates
struct MD2TexCoord {
	unsigned short int u, v;
};

// This stores the frames vertices after they have been transformed
struct MD2Frame {
	float scale[3];
	float translate[3];
	unsigned char strName[16];
	MD2Vertex *vertices;
};

// This stores a skin name
struct MD2Skin {
	unsigned char skinName[64];
};

struct MD2Package {
	MD2Header		header;
	GLuint			vb;
	GLuint			textureID;
	
	MD2Skin*		skins;
	MD2TexCoord*	texCoords;
	MD2Triangle*	triangles;
	MD2Frame*		frames;
	std::vector<FrameState*> frameStates; // Types of frames
};

class Frame { // Frames in a frame state /type
public:
	Frame(std::string nameFrame);
	~Frame();

	std::string nameFrame; // Walking01, Walking02
	
};

class FrameState { // Types of frame states in a MD2
public:
	FrameState(std::string nameFrameState);
	~FrameState();

	std::string nameFrameState; // Walking, Idle, Jump
	std::vector<std::string> nameFrame; // Frame of this type
	int amountOfFrames;
	int offsetAllFrames;
};


#pragma endregion Contains structs and classes

class MD2Manager 
{
public:
	MD2Manager();
	~MD2Manager();
	
	bool loadMD2(const char* md2Filepath, const char* texFilePath, MD2Package &package);
};