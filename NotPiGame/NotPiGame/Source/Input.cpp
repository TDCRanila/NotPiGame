#include "../Headers/Input.h"

#define CheckKey(INPUT)\
	if (keys[INPUT]) { inputs[INPUT] = true;}	\
	else { inputs[INPUT] = false; }				\

Input::Input() { /* Empty */ }

Input::~Input() { /* Empty */ }

void Input::HandleKeys(bool keys[1024], int scancode, int action, int mods) {

	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);

	CheckKey(GLFW_KEY_ESCAPE);


	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_ESCAPE);


	CheckKey(GLFW_KEY_ESCAPE);


}

bool Input::GetKey(int key) {
	if (inputs[key] == true) { return true; };
}