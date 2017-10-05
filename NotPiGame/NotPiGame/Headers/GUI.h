#pragma once

#include "../Headers/ModelMatrix.h"
#include "../Headers/Texture.h"

#include <vector>

class Shader;

class GUIComponent : public ModelMatrix {
public:
	GUIComponent(std::string textureFile, Shader* shader);
	~GUIComponent();
	
	bool Update(float deltaTime);
	bool Draw(glm::mat4 cameraView, glm::mat4 cameraProjection);
	
	Texture texture;
	
private:
	unsigned int m_vbo;	
};

// Manages all the GUI stuff
class GUI {
public:
	GUI();
	~GUI();
	
	std::vector<GUIComponent> components;
		
};