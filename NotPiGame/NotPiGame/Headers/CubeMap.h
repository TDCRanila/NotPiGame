#pragma once

#include "../Headers/ModelMatrix.h"

#include "../Headers/ShaderClass.h"

#include <vector>

class CubeMap : public ModelMatrix {
public:
	CubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* back, const char* front);
	~CubeMap();
	
	bool Update(float deltaTime);
	bool Draw(glm::mat4 cameraView, glm::mat4 cameraProjection);
	GLuint getTextureID();
	
private:
	GLuint m_textureID;
	GLuint m_vbo;
	std::vector<const char*> m_cubeMapFaces; // This vector stores the filepaths to the different sides
};
