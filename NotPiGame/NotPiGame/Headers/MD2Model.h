#pragma once

#include "../Headers/MD2Manager.h"
#include "../Headers/ModelMatrix.h"

class MD2Model : public ModelMatrix
{
public:
	MD2Model();
	MD2Model(std::string modelFile, std::string textureFile, MD2Manager* m);
	~MD2Model();
	
	bool Update(float deltaTime);
	bool Draw(glm::mat4 cameraView, glm::mat4 cameraProjection);
	
	void animateModel();
	
	MD2Package getModelPackage();
	
	int getFrameState();
	void setFrameState(int numberOfState);
	
	int getFrameStep();
	void setFrameStep(int frameStep);
	
	bool getAnimate();
	void setAnimate(bool animate);
	
	bool getAnimateFinnished();
	
	float getAnimSpeed();
	void  setAnimSpeed(float animSpeed);
	
	MD2Package modelPackage;

private:
	bool m_animate			= true; // On default the model won't animate
	bool m_animFinnished	= false;
	
	int m_numberState		= 0;	 // The current frame State
	int m_frameStep			= 0;	 // Frame Step in the animation
	
	int m_previousNumberState	= 0;
	int m_previousFrameStep		= 0;
	bool m_newFrameState	= false;
	
	float m_animationTime	= 0;
	float m_animationSpeed	= 10;
};