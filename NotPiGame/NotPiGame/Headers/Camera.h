#pragma once

#include "../Headers/Input.h"

#include "../glm/include/glm.hpp"
#include "../glm/include/gtc/matrix_transform.hpp"

#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
	CAMERA_ROLL_LEFT,
	CAMERA_ROLL_RIGHT,
    CAMERA_DOWN,
    CAMERA_SLOW
};

// Default User Camera values - "Camera options"
const float YAW = 0.0f;
const float PITCH = 0.0f;

const float SPEED = 100.0f;
const float SpeedSlowMultiplier = 0.25f;
const float SENSITIVTY = 0.125f;
const float SCROLLSENSITIVITY = 1.25f;

const float MINFOV = 1.0f;
const float MAXFOV = 110.f;
const float STARTFOV = 70.f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private:
	float oldMouseInputX = 0;
	float oldMouseInputY = 0;
	float newMouseInputX = 0;
	float newMouseInputY = 0;
	
	// Enum stored inside of the camera
	Camera_Movement CameraMovementEnum;
	// Input class pointer
	Input* m_input = nullptr;
	
public:
    // Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	
    // Eular Angles
    float Yaw;
    float Pitch;
	
	// Camera Stats
    float MovementSpeed;
	float MovementSpeedSlowMultiplier;
    float MouseSensitivity;
    float FOV;
	
    //Constructor, default camera settings
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = 0.0f, float pitch = 0.0f, float Fov = 90.f, float Speed = 100.f)	{
		std::cout << "####################################\n" << std::endl;
        std::cout << "#Creating Camera...                #\n" << std::endl;
        std::cout << "####################################\n" << std::endl;
		this->Position = position;
		this->Up = up;
		this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->FOV = Fov;
		this->MouseSensitivity = SENSITIVTY;
		this->MovementSpeed = Speed;
			   
        this->updateCameraVectors();
    }

	void cameraUpdate(float deltaTime) {
		//// Camera update - Keyboard
		//if (this->m_input->GetKey(KEY_D)) {
		//	this->CameraMovementEnum = CAMERA_RIGHT;
		//	this->ProcessKeyboard(deltaTime);
		//}
		//if (this->m_input->GetKey(KEY_A)) {
		//	this->CameraMovementEnum = CAMERA_LEFT;
		//	this->ProcessKeyboard(deltaTime);
		//}
		//if (this->m_input->GetKey(KEY_S)) {
		//	this->CameraMovementEnum = CAMERA_BACKWARD;
		//	this->ProcessKeyboard(deltaTime);
		//}
		//if (this->m_input->GetKey(KEY_W)) {
		//	this->CameraMovementEnum = CAMERA_FORWARD;
		//	this->ProcessKeyboard(deltaTime);
		//}
		//if (this->m_input->GetKey(KEY_LEFT_SHIFT)) {
		//	this->CameraMovementEnum = CAMERA_UP;
		//	this->ProcessKeyboard(deltaTime);
		//}
		//if (this->m_input->GetKey(KEY_LEFT_CONTROL)) {
		//	this->CameraMovementEnum = CAMERA_DOWN;
		//	this->ProcessKeyboard(deltaTime);
		//}
		
		// Camera Update - Mouse - With mouse Gliding fix		
		newMouseInputX =	m_input->MouseOffset.x;
		newMouseInputY =	m_input->MouseOffset.y;

		if ((newMouseInputX != oldMouseInputX) || (newMouseInputY != oldMouseInputY)) {
			this->ProcessMouseMovement(newMouseInputX, newMouseInputY, true);
		}
	
		oldMouseInputX =	newMouseInputX;
		oldMouseInputY =	newMouseInputY;
	}
	
	void storeInput(Input* input) {
		this->m_input = input;
	}
	
    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() {   
		glm::mat4 view = glm::lookAt(this->Position, this->Position + this->Front, this->Up);
		return view;
    }

    // Returns the projection matrix calculated
	glm::mat4 GetPerspectiveProjectionMatrix(float aspect, float nearZ, float farZ) {
		glm::mat4 projection = glm::perspective(FOV, aspect, nearZ, farZ);
		return projection;
    }

    // Returns the projection matrix calculated
	glm::mat4 GetOrthogonalProjectionMatrix(float left, float right, float bottom, float top, float nearZ, float farZ) {
		glm::mat4 ortho = glm::ortho(left, right, bottom, top, nearZ, farZ);
		return ortho;
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(float deltaTime) {
        float velocity = velocity = this->MovementSpeed * deltaTime;
	    if (this->CameraMovementEnum == CAMERA_FORWARD)   { this->Position += this->Front * velocity; }
	    if (this->CameraMovementEnum == CAMERA_BACKWARD)  { this->Position -= this->Front * velocity; }
	    if (this->CameraMovementEnum == CAMERA_LEFT)      { this->Position -= this->Right * velocity; }
	    if (this->CameraMovementEnum == CAMERA_RIGHT)     { this->Position += this->Right * velocity; }
	    if (this->CameraMovementEnum == CAMERA_UP)        { this->Position += this->Up    * velocity; }
	    if (this->CameraMovementEnum == CAMERA_DOWN)      { this->Position -= this->Up    * velocity; }
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw -= xoffset;
        this->Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->Pitch > 89.95f)
                this->Pitch = 89.95f;
            if (this->Pitch < -89.95f)
                this->Pitch = -89.95f;
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }
	
	    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset) {
		if (this->FOV >= MINFOV && this->FOV <= MAXFOV) { this->FOV -= yoffset * SCROLLSENSITIVITY; }
		if (this->FOV <= MINFOV) { this->FOV = MINFOV; }
		if (this->FOV >= MAXFOV) { this->FOV = MAXFOV; }
	}

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors() {
	    // Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);

		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));

		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}

    //Follows the target
    void FollowObject(glm::vec3 positionOfTarget, bool followX, bool followY, bool followZ) {
        if (followX == true) { this->Position.x = positionOfTarget.x; }
        if (followY == true) { this->Position.y = positionOfTarget.y; }
        if (followZ == true) { this->Position.z = positionOfTarget.z; }
    }

    //Moves the camera's position to the desired target position
    void GoToTarget(glm::vec3 positionOfTarget, bool lockX, bool lockY, bool lockZ) {
        if (lockX == true) { this->Position.x = positionOfTarget.x; }
        if (lockY == true) { this->Position.y = positionOfTarget.y; }
        if (lockZ == true) { this->Position.z = positionOfTarget.z; }
    }

    //Smoothly follows the targetted object
	void SmoothFollowObject(glm::vec3 positionOfTarget, bool followX, bool followY, bool followZ, float deltaTime) {
        float lerp = 0.10f;
        if (followX == true) { this->Position.x += (positionOfTarget.x - this->Position.x) * lerp; }
        if (followY == true) { this->Position.y += (positionOfTarget.y - this->Position.y) * lerp; }
        if (followZ == true) { this->Position.z += (positionOfTarget.z - this->Position.z) * lerp; }
    }

    //Smoothly moves the caemra to the desired location
	void SmmoothGoToTarget(glm::vec3 positionOfTarget, bool followX, bool followY, bool followZ, float deltaTime) {
        float lerp = 0.80f;
        if (followX == true) { this->Position.x += (positionOfTarget.x - this->Position.x) * lerp; }
        if (followY == true) { this->Position.y += (positionOfTarget.y - this->Position.y) * lerp; }
        if (followZ == true) { this->Position.z += (positionOfTarget.z - this->Position.z) * lerp; }
    }
};