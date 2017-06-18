#include "../Headers/CubeMap.h"

#include "../Headers/MyFiles.h"
#include "../Headers/Utilities.h"

#include "../glm/include/gtc/type_ptr.hpp"

GLfloat cubeVertices[] { // Vert.XYZ
	-1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

CubeMap::CubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* back, const char* front) {
	printf("\n###########################################\n");
	printf(  "#Creating Cube Map                         #\n");
	printf(  "###########################################\n");
	
	// Store all the textures into the vector
	this->m_cubeMapFaces.push_back(right);
	this->m_cubeMapFaces.push_back(left);
	this->m_cubeMapFaces.push_back(top);
	this->m_cubeMapFaces.push_back(bottom);
	this->m_cubeMapFaces.push_back(back);
	this->m_cubeMapFaces.push_back(front);
	
	// Creating the texture of the cube map
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	
	int w, h, comp;
	char* image;
	MyFiles* myFiles = new MyFiles();
	// Looping through the vector of texture path - to create cubemap textures
	for (int i = 0; i < this->m_cubeMapFaces.size(); i++) {
		image = myFiles->Load(this->m_cubeMapFaces[i], &w, &h, &comp);
		if (comp == 3) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		} else if (comp == 4) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
	}
	// Setting texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	// Unbind them all
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		
	// Creating the vbo for the cube
	glGenBuffers(1, &this->m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Cleanup
	delete myFiles;
	myFiles = nullptr;
	
#ifdef ACTIVATE_ERROR_CHECKING
	if (glGetError() != GL_NO_ERROR) { printf("Failed to create a cubemap\n"); }  
#endif
	
}

CubeMap::~CubeMap() {
	// Empty 
}

bool CubeMap::Update(float deltaTime) {
	ModelMatrix::createModelMatrix();
	
	return true; // return value is up to the user to decide (usually false if an error occurs)
}

bool CubeMap::Draw(glm::mat4 cameraView, glm::mat4 cameraProjection) {
	if (this->draw == true) {
		// Making sure that the depth mask is disabled for this render
		glDepthFunc(GL_LEQUAL);
		//glCullFace(GL_BACK);

		// Use the shader
		glUseProgram(this->shaderProgram->getShaderID());
	
		// Creating and Sending the MVP
		// First remove the translation part out of the view matrix
		// This will prevent the cube map from rotating if the player rotates
		glm::mat4 newCameraView = glm::mat4(glm::mat3(cameraView));
		glm::mat4 MVP(cameraProjection * newCameraView * this->modelMatrix);
		glUniformMatrix4fv(this->shaderProgram->MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		
		// Binding the buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	
		// Size of our stride - vertex.xyz
		GLsizei stride = (3) * sizeof(GLfloat);
	
		// Vertex Attribute - xyz
		glVertexAttribPointer(this->shaderProgram->positionCurrentLocation, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 0));
	
		// Enable the attributes
		glEnableVertexAttribArray(this->shaderProgram->positionCurrentLocation);
	
		// Binding the texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_textureID);
	
		// Draw the cube
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
		// Disabling attribute pointers
		glDisableVertexAttribArray(this->shaderProgram->positionCurrentLocation);
	
		// Unbind the stuff
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		
		// Re-Enable the depth mask for other renders 
		glDepthFunc(GL_LESS);
		//glCullFace(GL_BACK);
	
		#ifdef ACTIVATE_ERROR_CHECKING
		if (glGetError() != GL_NO_ERROR) { printf("Failed to draw - Cube Map\n"); }
		#endif // ACTIVATE_ERROR_CHECKING
	}
	return true;
}

GLuint CubeMap::getTextureID() {
	return this->m_textureID;	
}