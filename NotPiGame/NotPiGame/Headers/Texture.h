#pragma once

#include "../glad/include/glad/glad.h"

#include <string>

class Texture {
public:
	Texture(std::string textureFile); // Set to always spit out a mimapped texture - Loading the texture from a file with stb
	~Texture();
	
	GLuint textureID = 0;
};