#pragma once

#include "../GLFW/include/GLFW/glfw3.h"

#include <string>

struct WindowCallBacks {
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

class Input;

class Window {
public:
    Window(std::string windowName, int windowWidth, int windowHeight);
    ~Window();

    bool CreateGLFWWindow(bool makeCurrent, bool enableVSync);
    void PrintContextInfo(bool displayRenderer = true, bool displayOGLversion = true, bool displayOGLextensions = false);
    void SetWindowFlags();

    GLFWwindow* GLFWWindow;
    int windowWidth;
    int windowHeight;
    std::string windowName;

	Input* InputClass;

private:
    WindowCallBacks callbacks;
    GLFWimage images[2];
    //images[0] = load_icon("my_icon.png");
    //images[1] = load_icon("my_icon_small.png");
};