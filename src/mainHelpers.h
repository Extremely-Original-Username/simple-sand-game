#ifndef MAIN_HELPERS_H
#define MAIN_HELPERS_H

#include <GLFW/glfw3.h>

// Callback to adjust the viewport when the window is resized
inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Allow user to escape to end the application
inline void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#endif // MAIN_HELPERS_H