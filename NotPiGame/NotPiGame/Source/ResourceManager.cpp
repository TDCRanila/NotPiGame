#include "../Headers/ResourceManager.h"

#include "../Headers/Texture.h"

ResourceManager::ResourceManager() {
	md2Manager		= new MD2Manager();
	objManager		= new OBJManager();
	shaderManager	= new ShaderManager();
}

ResourceManager::~ResourceManager() {
	// Delete all the resources that the resources manager ever loaded
	this->deleteResources();
	
	// Deleting all the different managers
	delete md2Manager;
	delete objManager;
	delete shaderManager;
}

void ResourceManager::updateResources() {
	// Check if any models need to be deleted
	for (int i = 0; i < this->objModelsCopied.size(); i++) {
		if (objModelsCopied[i]->deleteModel == true) {
			delete objModelsCopied[i];
			objModelsCopied[i] = objModelsCopied.back();
			objModelsCopied.pop_back();
	} }
	for (int i = 0; i < this->md2ModelsCopied.size(); i++) {
		if (md2ModelsCopied[i]->deleteModel == true) {
			delete md2ModelsCopied[i];
			md2ModelsCopied[i] = md2ModelsCopied.back();
			md2ModelsCopied.pop_back();
		} }
	for (int i = 0; i < this->cubeMapsCopied.size(); i++) {
		if (cubeMapsCopied[i]->deleteModel == true) {
			delete cubeMapsCopied[i];
			cubeMapsCopied[i] = cubeMapsCopied.back();
			cubeMapsCopied.pop_back();
		} }
}

bool ResourceManager::deleteResources() {
// Looping and deleting all the elements	
	if (!md2ModelsCopied.empty()) {
		for (int i = 0; i < md2ModelsCopied.size(); i++) {
			delete md2ModelsCopied[i];
			md2ModelsCopied[i] = nullptr;
	} }
	md2ModelsCopied.clear();
	
	if (!objModelsCopied.empty()) {
		for (int i = 0; i < objModelsCopied.size(); i++) {
			delete objModelsCopied[i];
			objModelsCopied[i] = nullptr;
	} }
	objModelsCopied.clear();
	
	if (!shadersCopied.empty()) {
		for (int i = 0; i < shadersCopied.size(); i++) {
			delete shadersCopied[i];
			shadersCopied[i] = nullptr;
    } }
	shadersCopied.clear();
	
	if (!cubeMapsCopied.empty()) {
		for (int i = 0; i < cubeMapsCopied.size(); i++) {
			delete cubeMapsCopied[i];
			cubeMapsCopied[i] = nullptr;
	} }
	cubeMapsCopied.clear();
	
	// Clear out the other vectors
	md2Models.clear();
	objModels.clear();
	shaders.clear();
	cubeMaps.clear();
		
	return true;
}

	// TODO: integer i needs to be a string. storageClass has a pointer and the string of the filename
	// We check if there is already a model with that string.
	// We then call the getModel function and put that integer I in the vector below and we create a copy of that model
void ResourceManager::loadAndStoreMD2(std::string modelFile, std::string textureFile, Shader* shader) {
	MD2Model* model = new MD2Model(modelFile, textureFile, this->md2Manager);
	model->storeShader(shader);
	this->md2Models.push_back(model); // Store the pointer inside of original vector
	this->md2ModelsCopied.push_back(model); // Store the pointer inside of copy vector
}

void ResourceManager::loadAndStoreOBJ(std::string modelFile, Shader* shader) {
	OBJModel* model = new OBJModel(modelFile, this->objManager);
	model->storeShader(shader);
	this->objModels.push_back(model); // Store the pointer inside of original vector
	this->objModelsCopied.push_back(model); // Store the pointer inside of copy vector
}

Shader* ResourceManager::createShader(std::string vsh, std::string fsh) {
	Shader* shader; // Create a temporary pointer
	this->shaderManager->loadAndCreateShader(vsh.c_str(), fsh.c_str(), shader);
	this->shaders.push_back(shader); // Store the pointer inside of the resources its vector, just incase we need it later
	this->shadersCopied.push_back(shader); // Store the pointer inside of copy vector
	return shader;
}

CubeMap* ResourceManager::createCubeMap(std::string right, std::string left, std::string top, std::string bottom, std::string back, std::string front, Shader* shader) {
	CubeMap* cubemap = new CubeMap(right.c_str(), left.c_str(), top.c_str(), bottom.c_str(), back.c_str(), front.c_str());
	cubemap->storeShader(shader);
	this->cubeMaps.push_back(cubemap); // Store the pointer inside of the resources its vector, just incase we need it later
	this->cubeMapsCopied.push_back(cubemap); // Store the pointer inside of copy vector
	return cubemap;
}

GLuint ResourceManager::createTexture(std::string textureFile) {
	// Create a temporary Texture class which gets destroyed at the end of the function
	Texture texture(textureFile);
	this->textures.push_back(texture.textureID); // Store the pointer inside of the resources its vector, just incase we need it later
	return texture.textureID;
}

MD2Model* ResourceManager::getModelMD2(int i) {
	MD2Model* copyModel = new MD2Model((*(md2Models[i])));
	this->md2ModelsCopied.push_back(copyModel); // Make sure the pointer doesn't get lost
	return copyModel;
}

OBJModel* ResourceManager::getModelOBJ(int i) {
	OBJModel* copyModel = new OBJModel( (*(objModels[i])) );	
	this->objModelsCopied.push_back(copyModel); // Make sure the pointer doesn't get lost
	return copyModel;
}

MD2Model* ResourceManager::getOriginalModelMD2(int i) {
	return (this->md2Models[i]);	
}

OBJModel* ResourceManager::getOriginalModelOBJ(int i) {
	return (this->objModels[i]);	
}

CubeMap*  ResourceManager::getCubeMap(int i) {
	return (this->cubeMaps[i]);
}

Shader*   ResourceManager::getShader(int i) {
	return (this->shaders[i]);
}

GLuint   ResourceManager::getTextureID(int i) {
	return (this->textures[i]);
}