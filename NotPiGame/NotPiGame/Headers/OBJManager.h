#pragma once

#include "../glad/include/glad/glad.h"

#include "../Headers/tiny_obj_loader.h"
#include "../Headers/MyFiles.h"
#include "../Headers/Geometry.h"
#include "../Headers/Utilities.h"

////#include <math.h>
////#include <cassert>

#include <limits>
#include <vector>
#include <string>

struct DrawObject {
	GLuint vb;  // The Vertex buffer
	int numTriangles;
	std::vector<Triangle> triangles; // The triangles and their local positions in the model
	size_t material_id;
};

class OBJManager {
public:
	OBJManager();
	~OBJManager();
	
	static std ::string GetBaseDir(const std::string &filePath);
	static bool FileExists(const std::string &abs_filename);
	static void CheckErrors(std::string desc);
	
	// Loads in the obj file and stores the objects/shapes in a vector of struct drawObjects
	// The materials are also stored into the drawObjects struct, aswell as the textures...
	bool loadObject(float bmin[3], float bmax[3],
		std::vector<DrawObject> *drawObjects, std::vector<tinyobj::material_t>& materials,
		std::map<std::string, GLuint>& textures, const char* filename);
};