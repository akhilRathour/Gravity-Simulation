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


	//toggle fullscreen
    //bool isFullscreen = false;
    bool isFullscreen = true;

    int windowedX = 100, windowedY = 100;
    //int windowedX = 0, windowedY = 0;
    int windowedWidth = 1280, windowedHeight = 720;
    void ToggleFullscreen();

};
