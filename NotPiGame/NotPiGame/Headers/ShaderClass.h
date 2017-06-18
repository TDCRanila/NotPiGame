#pragma once

#include "../glad/include/glad/glad.h"

class Shader
{
public:
	Shader();
	~Shader();
			
	GLuint	getShaderID();
	void	setShaderID(GLuint shaderProgram);

	// Atrributes
	GLint positionCurrentLocation; // Current Frame
	GLint positionNextLocation; // Next Frame
	GLint normalLocation;
	GLint textureCoordLocation;
	
		
	// Uniforms
	GLint MVPLocation;
	GLint ambientLocation;
	GLint animTimeLocation;
	
	// Sampler Uniforms
	GLint cubeSamplerLocation;
	GLint texSamplerLocation;

private:
	GLuint shaderProgram; // ID
};

class ShaderManager 
{
public:
	ShaderManager();
	~ShaderManager();

	// Allocates and sets the shader pointer that gets passed in
	bool loadAndCreateShader(const GLchar* vertexPath, const GLchar* fragmentPath, Shader* &shader);
	GLuint createProgram(const GLchar* vertexPath, const GLchar* fragmentPath);
	
	void setUniform(GLint &value, GLuint program, const GLchar* locationName);
	void setAttribute(GLint &value, GLuint program, const GLchar* locationName);
};