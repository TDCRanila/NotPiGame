#include "../Headers/GUI.h"

#include "../glm/include/gtc/type_ptr.hpp"

#include "../Headers/Utilities.h"
#include "../Headers/ShaderClass.h"

GLfloat planeVertices[] = {
    // Positions        // Texture Coords
     1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // Top Right
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// Bottom Right
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,	// Top Left 
     1.0f, -1.0f, 0.0f, 0.0f, 1.0f,	// Bottom Right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	// Bottom Left
    -1.0f,  1.0f, 0.0f, 1.0f, 0.0f,	// Top Left
};

GUIComponent::GUIComponent(std::string textureFile, Shader* shader) : texture(textureFile) {
	// Setting the shader pointer
	this->shaderProgram = shader;
	
	// Creating the vbo for the plane
	glGenBuffers(1, &this->m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
#ifdef ACTIVATE_ERROR_CHECKING
	if (glGetError() != GL_NO_ERROR) { printf("Failed to create a GUI component\n"); }  
#endif
}

GUIComponent::~GUIComponent() {
	// Empty
}

bool GUIComponent::Update(float deltaTime) {
	ModelMatrix::createModelMatrix();
	return true; // return value is up to the user to decide (usually false if an error occurs)	
}

bool GUIComponent::Draw(glm::mat4 cameraView, glm::mat4 cameraProjection) {
	if (this->draw == true) {
		// Disable depth testing
		glDepthFunc(GL_NONE);
		
		// Use the shader
		glUseProgram(this->shaderProgram->getShaderID());
	
		// Creating and Sending the MVP
		glm::mat4 newCameraView = glm::mat4(1.0f);
		glm::mat4 MVP(cameraProjection * newCameraView * this->modelMatrix);
		glUniformMatrix4fv(this->shaderProgram->MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		
		// Binding the buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	
		// Size of our stride - vertex.xyz + texture.uv
		GLsizei stride = (3 + 2) * sizeof(GLfloat);
	
		// Vertex Attribute - xyz
		glVertexAttribPointer(this->shaderProgram->positionCurrentLocation, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 0));
	
		// Texture Attribute - uv
		glVertexAttribPointer(this->shaderProgram->textureCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 3));
		
		// Enable the attributes
		glEnableVertexAttribArray(this->shaderProgram->positionCurrentLocation);
		glEnableVertexAttribArray(this->shaderProgram->textureCoordLocation);
	
		// Binding the texture
		glBindTexture(GL_TEXTURE_2D, this->texture.textureID);
	
		// Draw the cube
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
		// Disabling attribute pointers
		glDisableVertexAttribArray(this->shaderProgram->positionCurrentLocation);
		glDisableVertexAttribArray(this->shaderProgram->textureCoordLocation);
	
		// Unbind the stuff
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glDepthFunc(GL_LEQUAL);
	
#ifdef ACTIVATE_ERROR_CHECKING
		if (glGetError() != GL_NO_ERROR) { printf("Failed to draw - GUI Component\n"); }
		return false;
#endif // ACTIVATE_ERROR_CHECKING
	}
	return true;
}

GUI::GUI() {
	// Empty	
}

GUI::~GUI() {
	// Empty	
}
