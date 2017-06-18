#pragma once

#include "../glm/include/glm.hpp"
#include "../glm/include/gtc/matrix_transform.hpp"

/******************************************************************************************
Object model is a base class to represent the most basic form of models
its update and draw classes are pure virtual so any object derived from this must supply those function
It contains simplified matrices for orientation, position and scaling and the ability to create new model matrix before a draw is done
examples of usage can be found in CubeModel
******************************************************************************************/

class Graphics; class Shader; // FW Declarations

class ModelMatrix 
{
public:
	ModelMatrix(); // Normal Constructor
	//ModelMatrix& operator=(const ModelMatrix &other); // Copy Constructor
	virtual ~ModelMatrix();

	virtual bool Update(float deltaTime) = 0;
	virtual bool Draw(glm::mat4 cameraView, glm::mat4 cameraProjection) = 0;
	
	void storeShader(Shader* shaderProgram); // Stores a pointer to the shader
	void createModelMatrix(); // Creates a fresh new model matrix
		
	// Functions to manipulate the model matrix of the model
	glm::vec3	getPositon(); 
	void		setPosition(glm::vec3* position);
	void		setPosition(glm::vec3 position);
		
	glm::vec3	getScale();
	void		setScale(glm::vec3* scale);
	void		setScale(glm::vec3 scale);
			
	glm::vec3	getRotations();
	void		setRotations(glm::vec3* rotation);
	void		setRotations(glm::vec3 rotation);
	void		setRotationsDegrees(glm::vec3 rotation);
	
	void		setXRotation(float angleRadians);
	void		setYRotation(float angleRadians);
	void		setZRotation(float angleRadians);
	void		setXRotationDegrees(float angleXDegrees);
	void		setYRotationDegrees(float angleYDegrees);
	void		setZRotationDegrees(float angleZDegrees);
	
	void		createRotationMatrix(); // since these get altered a lot we have a function to remake the matrix
	void		createXRotationMatrix();
	void		createYRotationMatrix();
	void		createZRotationMatrix();
	
	// Rendering values
	glm::mat4	modelMatrix; //The model matrix of the model
	Shader*		shaderProgram;
	bool		deleteModel = false; // Boolean that states when the models needs to be removed
	bool		draw		= true; // Bolean to see if the model wants be drawn or not
	bool		cancelView	= false; // A boolean to disable view calculation with the MVP
	//glm::vec4	colour; // an ambiant colour or tone shade for the model, leave as white for normal
			
protected:
	// Values used for the model, translation, rotation, scaling, etc.
	glm::vec3	_position	= glm::vec3(-9999.0f, -9999.0f, -9999.0f); // Starting location of the model
	glm::vec3	_rotation	= glm::vec3(0.0f, 0.0f, 0.0f); // This rotation is in radians
	glm::vec3	_scale		= glm::vec3(1.0f, 1.0f, 1.0f);			
	
	glm::mat4	_translationMatrix;
	glm::mat4	_rotationMatrix;
	glm::mat4	_scaleMatrix;
	
	glm::mat4	_rotationXMatrix;
	glm::mat4	_rotationYMatrix;
	glm::mat4	_rotationZMatrix;
};
