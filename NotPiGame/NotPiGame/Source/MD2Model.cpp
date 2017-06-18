#include "../Headers/MD2Model.h"

#include "../Headers/ShaderClass.h"
#include "../Headers/Utilities.h"

#include "../glm/include/gtc/type_ptr.hpp"

MD2Model::MD2Model() {
	// Empty	
}

MD2Model::MD2Model(std::string modelFile, std::string textureFile, MD2Manager* MD2M) {
	MD2M->loadMD2(modelFile.c_str(), textureFile.c_str(), modelPackage);
}

MD2Model::~MD2Model() {

}

bool MD2Model::Update(float deltaTime) {
	ModelMatrix::createModelMatrix();
	
	// Add the deltaTime to the animation time if we the model needs to animate
	if (m_animate == true) {
		if (m_animationTime < 1) {
			m_animationTime += deltaTime * m_animationSpeed;
		}
	} else {
		m_animationTime = 0;
	}
	return true;
}

bool MD2Model::Draw(glm::mat4 cameraView, glm::mat4 cameraProjection) {
	if (this->draw == true) {
		//Activate the shader programme of the model
		glUseProgram(this->shaderProgram->getShaderID());
			
		// Create a buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->modelPackage.vb);
	
		// Creating the MVP before sending it to the shader
		glm::mat4 MVP;
		if (this->cancelView == true) {
			MVP = glm::mat4(cameraProjection * glm::mat4(1.0f) * this->modelMatrix);
		}
		else {
			MVP = glm::mat4(cameraProjection * cameraView * this->modelMatrix);
		}
		glUniformMatrix4fv(this->shaderProgram->MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform1f(this->shaderProgram->animTimeLocation, m_animationTime);
		
		// Texture binding
		glBindTexture(GL_TEXTURE_2D, this->modelPackage.textureID);
						
		// Size of our stride - Normal(XYZ), Texture(XY), Vertex(XYZ)
		GLsizei stride = (3 + 2 + 3) * sizeof(GLfloat);
			
		// Calculate the current frame and the next frame offsets
		GLsizei currentFrameOffset;
		GLsizei nextFrameOffset;
		currentFrameOffset  = ((this->modelPackage.header.numTriangles * 3 * stride) * (m_frameStep));
		nextFrameOffset     = ((this->modelPackage.header.numTriangles * 3 * stride) * (m_frameStep + 1));
			
		// Animate the model
		animateModel();
	
		// Set all the attribute pointers - Normal(XYZ), Texture(XY), CurrentFrame(XYZ), NextFrame(XYZ)
		//glVertexAttribPointer(graphics.shaderData.normalLoc, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 0) );
		glVertexAttribPointer(this->shaderProgram->textureCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (const void*)((sizeof(GLfloat) * 3) + currentFrameOffset));
		glVertexAttribPointer(this->shaderProgram->positionCurrentLocation, 3, GL_FLOAT, GL_FALSE, stride, (const void*)((sizeof(GLfloat) * 5) + currentFrameOffset));
		glVertexAttribPointer(this->shaderProgram->positionNextLocation, 3, GL_FLOAT, GL_FALSE, stride, (const void*)((sizeof(GLfloat) * 5) + nextFrameOffset));
	
		// Enable the attributes
		glEnableVertexAttribArray(this->shaderProgram->positionCurrentLocation);
		glEnableVertexAttribArray(this->shaderProgram->positionNextLocation);
		//glEnableVertexAttribArray(graphics.shaderData.normalLoc);
		glEnableVertexAttribArray(this->shaderProgram->textureCoordLocation);
	
		// Draw the triangles
		glDrawArrays(GL_TRIANGLES, this->modelPackage.frameStates[m_numberState]->offsetAllFrames *   
			3 * this->modelPackage.header.numTriangles, 
			3 * this->modelPackage.header.numTriangles);
	
#ifdef ACTIVATE_ERROR_CHECKING
		if (glGetError() != GL_NO_ERROR) { printf("Failed to draw - MD2 Object\n"); return false; }
#endif
	
		// Disabling attribute pointers
		glDisableVertexAttribArray(this->shaderProgram->positionCurrentLocation);
		glDisableVertexAttribArray(this->shaderProgram->positionNextLocation);
		//glDisableVertexAttribArray(graphics.shaderData.normalLoc);
		glDisableVertexAttribArray(this->shaderProgram->textureCoordLocation);
	
		// Unbind everything
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
		return true;
}

void MD2Model::setFrameState(int numberOfFrame) {
	this->m_previousFrameStep = m_frameStep;
	this->m_previousNumberState = m_numberState;
	this->m_newFrameState = true;
	if (this->modelPackage.frameStates.size() < numberOfFrame) {
		printf("Warning - Setting invalid number of frame, there is no: %d\n", m_numberState);
		// Sets the value back to 0
		numberOfFrame = 0;
	}
	this->m_numberState = numberOfFrame;
	this->m_animFinnished = false;
	this->m_frameStep = 0;
	// printf("Now using FrameState: %s\n", this->modelPackage.frameStates[m_numberState]->nameFrameState.c_str());
}

int MD2Model::getFrameState() {
	return this->m_numberState;
}

void MD2Model::animateModel() {
	if (m_animationTime >= 1) {
		m_frameStep++;
		m_animFinnished = false;
		if (m_frameStep + 1 >= this->modelPackage.frameStates[m_numberState]->amountOfFrames) {
			m_animFinnished = true;
			m_frameStep = 0;
		}
		m_animationTime = 0;
	}
}

int MD2Model::getFrameStep() {
	return this->m_frameStep;
}
void MD2Model::setFrameStep(int frameStep) {
	this->m_frameStep = frameStep;
}

bool MD2Model::getAnimate() {
	return this->m_animate;
}

void MD2Model::setAnimate(bool animate) {
	this->m_animate = animate;	
}
	
bool MD2Model::getAnimateFinnished() {
	return this->m_animFinnished;	
}

float MD2Model::getAnimSpeed() {
	return this->m_animationSpeed;
}
void  MD2Model::setAnimSpeed(float animSpeed) {
	this->m_animationSpeed = animSpeed;
}