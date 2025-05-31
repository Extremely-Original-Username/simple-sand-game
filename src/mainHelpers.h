#ifndef MAIN_HELPERS_H
#define MAIN_HELPERS_H

#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>

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

// Function to check for shader compilation errors
inline void checkShaderCompileErrors(unsigned int shader)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

#endif // MAIN_HELPERS_H