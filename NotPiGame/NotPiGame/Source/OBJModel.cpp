#include "../Headers/OBJModel.h"

#include "../Headers/ShaderClass.h"
#include "../Headers/Geometry.h"
#include "../Headers/Utilities.h"

#include "../glm/include/gtc/type_ptr.hpp"

OBJModel::OBJModel() {
	// Empty
}

OBJModel::OBJModel(std::string modelFile, OBJManager* OM) {
	OM->loadObject(this->bmin, this->bmax, &this->drawObjects, this->materials, this->textures, modelFile.c_str());
}

OBJModel::~OBJModel() { 
	// Empty
}

bool OBJModel::Update(float deltaTime) {
	ModelMatrix::createModelMatrix();
		
	// Update the triangles to the world position
	for (int i = 0; i < drawObjects.size(); i++) {
		for (int k = 0; k < drawObjects[i].numTriangles; k++) {
			this->drawObjects[i].triangles[k].calculateWorldPos(this->modelMatrix);
		}
	}
	
	return true;
}

bool OBJModel::Draw(glm::mat4 cameraView, glm::mat4 cameraProjection) {
	if (this->draw == true) {
		//Activate the shader programme of the model
		glUseProgram(this->shaderProgram->getShaderID());
	
		// Creating and Sending the MVP
		glm::mat4 MVP(cameraProjection * cameraView * this->modelMatrix);
		glUniformMatrix4fv(this->shaderProgram->MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
	
		///--- Iterate over the draw object vector - to draw all the shapes ---///
		for (size_t i = 0; i < drawObjects.size(); i++) {	
			DrawObject drawObject = drawObjects[i];
	
			// Bind the a buffer
			glBindBuffer(GL_ARRAY_BUFFER, drawObject.vb);
		
			// Binding the textures that are in the materials
			if ((drawObject.material_id < materials.size())) {
				std::string diffuse_texname = materials[drawObject.material_id].diffuse_texname;
				if (textures.find(diffuse_texname) != textures.end()) {
					glBindTexture(GL_TEXTURE_2D, textures[diffuse_texname]);
				}
			}
								
			// Size of our stride - Vertex(XYZ), Texture(UV), Colour(RGB), Normal(XYZ)
			GLsizei stride = (3 + 3 + 3 + 2) * sizeof(GLfloat);
		
			// Set all the attribute pointers
			glVertexAttribPointer(this->shaderProgram->positionCurrentLocation, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 0));
			glVertexAttribPointer(this->shaderProgram->textureCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 9));	
			//glVertexAttribPointer(graphics.shaderData.normalLoc				, 3, GL_FLOAT, GL_FALSE, stride, (const void*)(sizeof(GLfloat) * 15) );
	
			// Enable the attributes
			glEnableVertexAttribArray(this->shaderProgram->positionCurrentLocation);
			glEnableVertexAttribArray(this->shaderProgram->textureCoordLocation);
			//glEnableVertexAttribArray(graphics.shaderData.normalLoc);
		
			// Draw the triangles
			glDrawArrays(GL_TRIANGLES, 0, 3 * drawObject.numTriangles);
		
			#ifdef ACTIVATE_ERROR_CHECKING
			if (glGetError() != GL_NO_ERROR) { printf("Failed to draw - OBJ Object\n"); return false; }				  
			#endif // ACTIVATE_ERROR_CHECKING
		
					// Disabling attribute pointers
			glDisableVertexAttribArray(this->shaderProgram->positionCurrentLocation);
			glDisableVertexAttribArray(this->shaderProgram->textureCoordLocation);
			//glDisableVertexAttribArray(graphics.shaderData.normalLoc);
		
			// Unbind everything
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}	
return true;
}


