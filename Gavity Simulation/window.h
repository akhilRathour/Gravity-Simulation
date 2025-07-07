#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    Window(const std::string& title);
    ~Window();

    GLFWwindow* GetGLFWWindow();
    bool ShouldClose();
    void SwapBuffers();
    void PollEvents();
    void Clear();

    unsigned int width;
    unsigned int height;

    GLFWwindow* window;
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    

};
