#include "../Headers/Texture.h"

#include "../glad/include/glad/glad.h"

#include "../Headers/MyFiles.h"
#include "../Headers/Utilities.h"

Texture::Texture(std::string textureFile) {
	// Temporary utilities
	MyFiles *myFiles = new MyFiles;
	int w, h, comp;

	// Loading in the tecture wth stbi
	char* image = myFiles->Load(textureFile.c_str(), &w, &h, &comp);
	if (!image) {
		fprintf(stderr, "Unable to load texture: %s \n", textureFile.c_str());
		exit(1);
	}
					
	/// Generate the textures calling the openGL functions and store them in the texture map.
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
					
	// Giving the image to openGL
	if (comp == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	} else if (comp == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
					
	// Texture Parameters And Mimaps
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					
	
	// Clean Up
	glBindTexture(GL_TEXTURE_2D, 0);
	free(image);
	delete myFiles;
	myFiles = nullptr;
	
#ifdef ACTIVATE_ERROR_CHECKING
	if (glGetError() != GL_NO_ERROR) { printf("Failed to create texture \n"); }
#endif // ACTIVATE_ERROR_CHECKING
}

Texture::~Texture() {
	
}