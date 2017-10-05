#pragma once

#include <string>

class Texture {
public:
	Texture(std::string textureFile); // Set to always spit out a mimapped texture - Loading the texture from a file with stb
	~Texture();
	
	unsigned int textureID = 0;
};