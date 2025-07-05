#include "Window.h"
#include <iostream>

Window::Window(const std::string& title)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n";
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Get monitor and video mode for fullscreen
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD.\n";
        std::exit(EXIT_FAILURE);
    }

	unsigned int width = mode->width;
    unsigned int height = mode->height;

    glViewport(0, 0, mode->width, mode->height);
    glEnable(GL_DEPTH_TEST);  // Enable depth for 3D rendering
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Window::GetGLFWWindow()
{
    return window;
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(window);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::Clear()
{
    glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

