#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glext.h>
#include <string>
#include <cstdlib>
#include "mainHelpers.h"
#include <GLES3/gl3.h>

using namespace std;

const string title = "Simple Sand!";
const int windowWidth = 500;
const int windowHeight = 450;
const int gridWidth = 10;
const int gridHeight = 9;

const char *vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

int main() {
    cout << "Starting...\n";
    if (!glfwInit()){
        return -1;
    }

    cout << "Creating window...\n";
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, &title[0], NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    cout << "Setting context...\n";
    glfwMakeContextCurrent(window);

    //Must happen AFTER the context is set
    cout << "Compiling shaders...\n";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);    
    checkShaderCompileErrors(vertexShader);
    

    cout << "Initialising data...\n";
    bool sandGrid [gridWidth][gridHeight];
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            sandGrid[x][y] = rand() % 3 == 0;
        }
    }

    float drawPlaneVertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left 
   };

   unsigned int drawPlaneindices[] = {
       0, 1, 3,   // first triangle
       1, 2, 3    // second triangle
   };  

    //Create vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);  

    //Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Copy verted data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(drawPlaneVertices), drawPlaneVertices, GL_DYNAMIC_DRAW);


    cout << "Beginning main loop...\n";
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

    return 0;
}
