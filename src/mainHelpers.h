#ifndef MAIN_HELPERS_H
#define MAIN_HELPERS_H

#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
using namespace std;

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
        cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

// Function to check for shader linking errors
inline void checkProgramLinkErrors(unsigned int program)
{
    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
    }
}

//Function to load text from a file
inline string loadFileToString(string filename){
    ifstream file(filename);

    if (!file.is_open()) cerr << "Failed to open file: " << filename << "\n";

    ifstream infile { filename };
    string file_contents { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };

    return file_contents;
}

#endif // MAIN_HELPERS_H