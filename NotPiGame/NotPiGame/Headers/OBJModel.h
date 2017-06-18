#pragma once

#include "../Headers/OBJManager.h"
#include "../Headers/ModelMatrix.h"

#include <string>

// Graphics already declared in ObjectModel.h, if needed in this class
class OBJModel : public ModelMatrix
{
public:
	OBJModel();
	OBJModel(std::string modelFile, OBJManager* OM);
	~OBJModel();
		
	bool Update(float deltaTime);
	bool Draw(glm::mat4 cameraView, glm::mat4 cameraProjection);
	
	// All the data of the obj
	std::vector<DrawObject> drawObjects;
	float bmin[3], bmax[3];
	std::vector<tinyobj::material_t> materials;
	std::map<std::string, GLuint> textures;
};
