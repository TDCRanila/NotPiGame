#include "../Headers/Graphics.h"

#include "../Headers/Input.h"

#include <iostream>

bool m_keys[1024];

Window::Window(std::string windowName, int windowWidth, int windowHeight) :
    windowName(windowName),
    windowWidth(windowWidth),
    windowHeight(windowHeight)
{
	InputClass = new Input();
}

Window::~Window() {
	delete InputClass;
	InputClass = nullptr;

    std::cout << "Destroying OpenGL Window - " << windowName << std::endl;
    glfwDestroyWindow(this->GLFWWindow);
}

bool Window::CreateGLFWWindow(bool makeCurrent, bool enableVSync) {
    // Make the window not resizable
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create the window
    this->GLFWWindow = glfwCreateWindow(this->windowWidth, this->windowHeight, this->windowName.c_str(), NULL, NULL);
    //glfwSetWindowIcon(this->GLFWWindow, 2, images);

    // Setting callbacks
    glfwSetErrorCallback(callbacks.error_callback);
    glfwSetKeyCallback(this->GLFWWindow, callbacks.key_callback);
	glfwSetMouseButtonCallback(this->GLFWWindow, callbacks.mousebutton_callback);
    glfwSetCursorPosCallback(this->GLFWWindow, callbacks.mouse_callback);
    glfwSetScrollCallback(this->GLFWWindow, callbacks.scroll_callback);
    glfwSetInputMode(this->GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Check if the window is valid
    if (!GLFWWindow) {
        return false;
    }

    // Make this window the current context
    if (makeCurrent) {
        glfwMakeContextCurrent(this->GLFWWindow);
    }

    // Enable VSync for this window
    if (enableVSync) {
        glfwSwapInterval(1);
    }

	//
	glfwSetWindowUserPointer(this->GLFWWindow, InputClass);

    std::cout << "Succesfully created a GLFW Window" << std::endl << std::endl;

    return true;
}

void Window::PrintContextInfo(bool displayRenderer, bool displayOGLversion, bool displayOGLextensions) {
    //const GLubyte* renderer = glGetString(GL_RENDERER);
    //const GLubyte* vendor = glGetString(GL_VENDOR);
    //const GLubyte* version = glGetString(GL_VERSION);
    //const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    //if (displayRenderer) { printf("Renderer: %s - Vendor: %s\n", renderer, vendor); }
    //if (displayOGLversion) { printf("OpenGL Version: %s\n\n", version); }
    //if (displayOGLextensions) { printf("Extensions: %s\n", extensions); }
}

void Window::SetWindowFlags() {
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
}

void WindowCallBacks::error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void WindowCallBacks::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Keys
    if ((key >= 0) && (key < 1024)) {
        if (action == GLFW_PRESS) { m_keys[key] = true; }
        else if (action == GLFW_RELEASE) { m_keys[key] = false; }
    }

	Input* inputClass = reinterpret_cast<Input*>(glfwGetWindowUserPointer(window));
	inputClass->HandleKeys(m_keys, scancode, action, mods);

}

void WindowCallBacks::mousebutton_callback(GLFWwindow* window, int button, int action, int mods) {
	// Keys
	if ((button >= 0) && (button < 8)) {
		if (action == GLFW_PRESS) { m_keys[button] = true; }
		else if (action == GLFW_RELEASE) { m_keys[button] = false; }
	}

	Input* inputClass = reinterpret_cast<Input*>(glfwGetWindowUserPointer(window));
	inputClass->HandleMouseButtons(m_keys, button, action, mods);
}

void WindowCallBacks::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	Input* inputClass = reinterpret_cast<Input*>(glfwGetWindowUserPointer(window));

	inputClass->MousePos.x = static_cast<float>(xpos);
	inputClass->MousePos.y = static_cast<float>(ypos);

	inputClass->MouseOffset.x = static_cast<float>(inputClass->MousePosOld.x - xpos);
	inputClass->MouseOffset.y = static_cast<float>(inputClass->MousePosOld.y - ypos);

	inputClass->MousePosOld.x = static_cast<float>(xpos);
	inputClass->MousePosOld.y = static_cast<float>(ypos);

}

void WindowCallBacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Empty
}