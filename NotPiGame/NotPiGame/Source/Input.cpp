#include "../Headers/Input.h"

#define CheckKey(INPUT)\
	if (keys[INPUT]) { inputs[INPUT] = true;}	\
	else { inputs[INPUT] = false; }				\

Input::Input() { /* Empty */ }

Input::~Input() { /* Empty */ }

void Input::HandleKeys(bool keys[1024], int scancode, int action, int mods) {

	CheckKey(GLFW_KEY_ESCAPE);
	CheckKey(GLFW_KEY_W);
	CheckKey(GLFW_KEY_A);
	CheckKey(GLFW_KEY_S);
	CheckKey(GLFW_KEY_D);
	CheckKey(GLFW_KEY_UP);
	CheckKey(GLFW_KEY_DOWN);
	CheckKey(GLFW_KEY_LEFT);
	CheckKey(GLFW_KEY_RIGHT);

	CheckKey(GLFW_KEY_Q);
	CheckKey(GLFW_KEY_E);
	CheckKey(GLFW_KEY_F);
	CheckKey(GLFW_KEY_Z);
	CheckKey(GLFW_KEY_X);
	CheckKey(GLFW_KEY_C);
	CheckKey(GLFW_KEY_V);
	CheckKey(GLFW_KEY_R);
	CheckKey(GLFW_KEY_P);
	CheckKey(GLFW_KEY_LEFT_BRACKET);
	CheckKey(GLFW_KEY_RIGHT_BRACKET);

	CheckKey(GLFW_KEY_SPACE);
	CheckKey(GLFW_KEY_LEFT_SHIFT);
	CheckKey(GLFW_KEY_LEFT_CONTROL);
	CheckKey(GLFW_KEY_RIGHT_SHIFT);
	CheckKey(GLFW_KEY_RIGHT_CONTROL);
	CheckKey(GLFW_KEY_F1);
	CheckKey(GLFW_KEY_F2);
	CheckKey(GLFW_KEY_F3);
	CheckKey(GLFW_KEY_F4);
	CheckKey(GLFW_KEY_F4);
	CheckKey(GLFW_KEY_F5);
	CheckKey(GLFW_KEY_F6);
	CheckKey(GLFW_KEY_F7);
	CheckKey(GLFW_KEY_F8);
	CheckKey(GLFW_KEY_F9);
	CheckKey(GLFW_KEY_F10);
	CheckKey(GLFW_KEY_F11);
	CheckKey(GLFW_KEY_F12);

	CheckKey(GLFW_KEY_0);
	CheckKey(GLFW_KEY_1);
	CheckKey(GLFW_KEY_2);
	CheckKey(GLFW_KEY_3);
	CheckKey(GLFW_KEY_4);
	CheckKey(GLFW_KEY_5);
	CheckKey(GLFW_KEY_6);
	CheckKey(GLFW_KEY_7);
	CheckKey(GLFW_KEY_8);
	CheckKey(GLFW_KEY_9);


}

void Input::HandleMouseButtons(bool keys[8], int button, int action, int mods) {
	CheckKey(GLFW_MOUSE_BUTTON_LEFT);
	CheckKey(GLFW_MOUSE_BUTTON_MIDDLE);
	CheckKey(GLFW_MOUSE_BUTTON_RIGHT);

}

bool Input::GetKey(int key) {
	if (inputs[key] == true) { return true; };
}