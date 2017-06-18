#include "../Headers/ShaderClass.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() {
    // Empty
}

Shader::~Shader() {
    // Empty
}
	
GLuint Shader::getShaderID() {
	return this->shaderProgram;
}

void Shader::setShaderID(GLuint shaderProgram) {
	this->shaderProgram = shaderProgram;
}

ShaderManager::ShaderManager() {
	// Empty	
}

ShaderManager::~ShaderManager() {
	// Empty
}

bool ShaderManager::loadAndCreateShader(const GLchar* vertexPath, const GLchar* fragmentPath, Shader* &shader) {
	printf("\n###########################################\n");
	printf("Creating Shader\n");
	printf("Vertex: %s\n", vertexPath);
	printf("Fragment: %s\n", fragmentPath);
	// Allocate shader
	shader = new Shader();
	
	// Create the shader program
	GLuint shaderProgram = createProgram(vertexPath, fragmentPath);
	
	// Settting attribute locations
	setAttribute(shader->positionCurrentLocation, shaderProgram, "a_position");
	setAttribute(shader->positionNextLocation   , shaderProgram, "a_position_next");
	setAttribute(shader->normalLocation         , shaderProgram, "a_normal");
	setAttribute(shader->textureCoordLocation   , shaderProgram, "a_texCoord");
			
	// Setting uniform locations
	setUniform(shader->MVPLocation          , shaderProgram, "u_MVP");
	setUniform(shader->ambientLocation      , shaderProgram, "u_ambient");
	setUniform(shader->animTimeLocation		, shaderProgram, "u_animTime");
	
	// Setting uniform sampler locations
	setUniform(shader->texSamplerLocation   , shaderProgram, "s_texture");
	setUniform(shader->cubeSamplerLocation	, shaderProgram, "s_cubeMap");
	
	shader->setShaderID(shaderProgram);
	printf("\n#Succesfully created the shader program\n");
	printf("\n###########################################\n");
	return true;
}

GLuint ShaderManager::createProgram(const GLchar* vertexPath, const GLchar* fragmentPath) {
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try {
	    // Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		if (!vShaderFile.is_open()) { printf("Unable to load in the Vertex shader file: %s\n", vertexPath);}
		if (!fShaderFile.is_open()) { printf("Unable to load in the Fragment shader file: %s\n", fragmentPath); }
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();
	
	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	// Shader Program
	GLuint programObject = glCreateProgram();
	glAttachShader(programObject, vertex);
	glAttachShader(programObject, fragment);
	glLinkProgram(programObject);
	// Print linking errors if any
	glGetProgramiv(programObject, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programObject, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	
	// Returns the program object
	return programObject;
}

void ShaderManager::setUniform(GLint &value, GLuint program, const GLchar* locationName) {
	value = glGetUniformLocation(program, locationName);
	if (value == -1) { printf("Warning - Couldn't find uniform location;  %s, in shaderProgram:  %d\n", locationName, program);}
}

void ShaderManager::setAttribute(GLint &value, GLuint program, const GLchar* locationName) {
	value = glGetAttribLocation(program, locationName);
	if (value == -1) { printf("Warning - Couldn't find attribute location;  %s, in shaderProgram:  %d\n", locationName, program); }
}