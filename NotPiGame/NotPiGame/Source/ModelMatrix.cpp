#include "../Headers/ModelMatrix.h"

////#include "../Headers/ShaderClass.h"

/******************************************************************************************
A base class, containing only information needed to position/rotate and create a model.
******************************************************************************************/

ModelMatrix::ModelMatrix() {
	// Create starting model matrix
	this->modelMatrix = glm::mat4(1.0f);
	this->createModelMatrix();
}

//ModelMatrix& ModelMatrix::operator=(const ModelMatrix &other) {
//	// Check if it is not going to assign itself
//	if (this != &other) {
//		if (p != 0) {
//			*p = *other.p;
//		}
//		else { // p is null - no memory allocated yet
//			p = new int(*other.p);
//		}
//	}
//	return *this;
//}

ModelMatrix::~ModelMatrix() {
 // Empty
}

void ModelMatrix::storeShader(Shader* shaderProgram) {
	this->shaderProgram = shaderProgram;
}

void ModelMatrix::createModelMatrix() {
	// Empty new identity matrices
	this->_translationMatrix	= glm::mat4(1.0f);
	this->_rotationMatrix		= glm::mat4(1.0f);
	this->_scaleMatrix			= glm::mat4(1.0f);
	
	// Creating the new model matrix
	createRotationMatrix();
	this->_scaleMatrix		 = glm::scale(this->_scaleMatrix, this->_scale);
	this->_translationMatrix = glm::translate(this->_translationMatrix, this->_position);
	this->modelMatrix		 = glm::mat4(this->_translationMatrix * this->_scaleMatrix  * this->_rotationMatrix);
}

#pragma region Model Matrix Fucntions

void ModelMatrix::createRotationMatrix() {
	createXRotationMatrix();
	createYRotationMatrix();
	createZRotationMatrix();
	
	glm::mat4 fullRot(this->_rotationXMatrix * this->_rotationYMatrix * this->_rotationZMatrix);
	this->_rotationMatrix = fullRot;
}

void ModelMatrix::createXRotationMatrix() {
	// X Rotation Matrix
	float x = this->_rotation.x;
	this->_rotationXMatrix = glm::mat4( 1,		 0,		 0,	 0, 
										0,  cos(x),	sin(x),	 0, 
										0, -sin(x),	cos(x),	 0, 
										0,		 0,		 0,	 1);
}

void ModelMatrix::createYRotationMatrix() {
	// Y Rotation Matrix
	float y = this->_rotation.y;
	this->_rotationYMatrix = glm::mat4( cos(y),	0, -sin(y),  0,
											 0,	1,		 0,  0,
									    sin(y),	0,	cos(y),  0,
										 	 0,	0,		 0,  1);
}
void ModelMatrix::createZRotationMatrix(){
	// Z Rotation Matrix
	float z = this->_rotation.z;
	this->_rotationZMatrix = glm::mat4(  cos(z), sin(z),  0,  0,
										-sin(z), cos(z),  0,  0,
											  0,	  0,  1,  0,
											  0,	  0,  0,  1);
}

glm::vec3 ModelMatrix::getPositon() {
	return _position;
}

void ModelMatrix::setPosition(glm::vec3* a_Pos) {
	this->_position.x = 	a_Pos->x;
	this->_position.y = 	a_Pos->y;
	this->_position.z = 	a_Pos->z;
}

void ModelMatrix::setPosition(glm::vec3 a_Pos) {
	this->_position = a_Pos;
}

glm::vec3 ModelMatrix::getScale() {
	return this->_scale;
}

void ModelMatrix::setScale(glm::vec3* scale) {
	this->_scale.x = scale->x;
	this->_scale.y = scale->y;
	this->_scale.z = scale->z;
}

void ModelMatrix::setScale(glm::vec3 scale) {
	this->_scale = scale;
}

glm::vec3 ModelMatrix::getRotations() {
	return _rotation;	
}

void ModelMatrix::setRotations(glm::vec3* a_Rots) {
	this->_rotation.x = a_Rots->x;
	this->_rotation.y = a_Rots->y;
	this->_rotation.z = a_Rots->z;
}

void ModelMatrix::setRotationsDegrees(glm::vec3 rotation) {
	setXRotationDegrees(rotation.x);
	setYRotationDegrees(rotation.y);
	setZRotationDegrees(rotation.z);
}

void ModelMatrix::setRotations(glm::vec3 a_Rots) {
	this->_rotation = a_Rots;
}

void ModelMatrix::setXRotation(float a_xRadians) {
	this->_rotation.x = a_xRadians;
}

void ModelMatrix::setYRotation(float a_yRadians) {
	this->_rotation.y = a_yRadians;
}

void ModelMatrix::setZRotation(float a_zRadians) {
	this->_rotation.z = a_zRadians;
}

void ModelMatrix::setXRotationDegrees(float angleXDegrees) {
	this->_rotation.x = glm::radians(angleXDegrees);
}

void ModelMatrix::setYRotationDegrees(float angleYDegrees) {
	this->_rotation.y = glm::radians(angleYDegrees);
}

void ModelMatrix::setZRotationDegrees(float angleZDegrees) {
	this->_rotation.z = glm::radians(angleZDegrees);
}

#pragma endregion