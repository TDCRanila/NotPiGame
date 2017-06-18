#include "../Headers/Graphics.h"

#include <iostream>

bool m_keys[1024];

Window::Window(std::string windowName, int windowWidth, int windowHeight) :
    windowName(windowName),
    windowWidth(windowWidth),
    windowHeight(windowHeight)
{
    // Empty
}

Window::~Window() {
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
    glfwSetCursorPosCallback(this->GLFWWindow, callbacks.mouse_callback);
    glfwSetScrollCallback(this->GLFWWindow, callbacks.scroll_callback);
    //glfwSetInputMode(this->GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    std::cout << "Succesfully created a GLFW Window" << std::endl << std::endl;

    return true;
}

void Window::PrintContextInfo(bool displayRenderer, bool displayOGLversion, bool displayOGLextensions) {
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    if (displayRenderer) { printf("Renderer: %s - Vendor: %s\n", renderer, vendor); }
    if (displayOGLversion) { printf("OpenGL Version: %s\n\n", version); }
    if (displayOGLextensions) { printf("Extensions: %s\n", extensions); }
}

void Window::SetWindowFlags() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
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
}

void WindowCallBacks::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Empty
}

void WindowCallBacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Empty
}