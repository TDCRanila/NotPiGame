#pragma once

//#include "../glad/include/glad/glad.h"

//#include "../Headers/MD2Manager.h"
//#include "../Headers/OBJManager.h"
#include "../Headers/CubeMap.h"

#include <string>
#include <vector>

class OBJModel; class MD2Model; class MD2Manager; class OBJManager; //FW Declaration

/********************************************************************/
/*This is a class that stores a pointer and string of the filePaths */
/********************************************************************/
class ModelStorage {
	ModelStorage(ModelMatrix* p, std::string meshFile, std::string texFile = "") : p(p) , meshFile(meshFile) {}
	~ModelStorage() { p = nullptr; }
	
	ModelMatrix* p = nullptr;
	std::string	name		= "";
	std::string	meshFile	= "";
	std::string texFile		= "";
};


/******************************************/
/*This is wrapper around all the managers */
/******************************************/
class ResourceManager {
public:
	ResourceManager(); // Allocates all the managers
	~ResourceManager(); // Destroy all the managers and its stored pointers
	
	void updateResources();	// Check what needs to be deleted
	bool deleteResources(); // Deletes all the resources stored inside of the vectors
	
	void loadAndStoreMD2(std::string modelFile, std::string textureFile, Shader* shader); // Creates and returns a pointer to the MD2model that get loaded in
	void loadAndStoreOBJ(std::string modelFile, Shader* shader); // Creates and returns a pointer to the MD2model that get loaded in
	Shader*		 createShader(std::string vsh, std::string fsh); // Loads in the shaders and creates them returning a pointer when finnished
	CubeMap*	 createCubeMap(std::string r, std::string l, std::string t, std::string bo, std::string ba, std::string f, Shader* s); // Creates a cubemap and returns a pointer to the class
	GLuint       createTexture(std::string); // Creates a texture and returns a pointer when finnished
	
	// Returns a pointer that points to a copy of the model
	MD2Model* getModelMD2(int i);
	OBJModel* getModelOBJ(int i);
	MD2Model* getOriginalModelMD2(int i);
	OBJModel* getOriginalModelOBJ(int i);
	
	CubeMap*  getCubeMap(int i);	// Just returns the pointer to the cubemap - no need for multiple cubemaps
	Shader*   getShader(int i);		// This function doesn't create a copy
	GLuint	  getTextureID(int i);	// Just returns a texture ID
	
	// Vectors of the original objects ever loaded in - doesn't contain pointers to copied objects
	std::vector<MD2Model*>	md2Models;
	std::vector<OBJModel*>	objModels;
	std::vector<Shader*>	shaders;    // Shader Class Pointers
	std::vector<CubeMap*>	cubeMaps;   // CubeMap Class Pointers
	std::vector<GLuint>		textures;   // TextureIDS
	
	// Vectors of pointers of all the original and copied objects
	std::vector<MD2Model*>	md2ModelsCopied;
	std::vector<OBJModel*>	objModelsCopied;
	std::vector<Shader*>	shadersCopied; 
	std::vector<CubeMap*>	cubeMapsCopied;
	
private:
	MD2Manager*		md2Manager      = nullptr;
	OBJManager*		objManager      = nullptr;
	ShaderManager*	shaderManager   = nullptr;
};