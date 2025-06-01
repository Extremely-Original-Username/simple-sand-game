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

    cout << "Loading shaders...\n";
    const string vertexShaderSource = loadFileToString("./src/shaders/vertex.glsl");
    const string fragmentShaderSource = loadFileToString("./src/shaders/fragment.glsl");

    const char* vertexShaderSourcePtr = &vertexShaderSource[0];
    const char* fragmentShaderSourcePtr = &fragmentShaderSource[0];

    //Must happen AFTER the context is set
    cout << "Compiling shaders...\n";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourcePtr, NULL);
    glCompileShader(vertexShader);    
    checkShaderCompileErrors(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourcePtr, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
        
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLinkErrors(shaderProgram);

    //Not needed once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  

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

   unsigned int drawPlaneIndices[] = {
       0, 1, 3,   // first triangle
       1, 2, 3    // second triangle
   };  

    /*
    Helper for me:
        A VAO is basically a preset of buffer bindings to a shader. So, when you want to use a certain shader 
        with a certain group of buffers ( index, vertex, Texcoord, etc) you can setup a VAO with that, and every 
        time.you switch to it, those bindings will be restored.

        VBO is what I just called a buffer. Nothing more.

        EBO is a VBO that is used to store vertex indices instead of actual data
        https://www.reddit.com/r/opengl/comments/1eks98r/in_terms_of_vao_vbo_and_ebo_what_is_the_best_way/
   */

   //Create Vertex Array Object to hold multiple VBOs if needed
   unsigned int VAO;
   glGenVertexArrays(1, &VAO);
   glBindVertexArray(VAO);

    //Create vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);  

    //Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Copy vertex data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(drawPlaneVertices), drawPlaneVertices, GL_DYNAMIC_DRAW);

    //Link vertex data to shader vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    //Create and bind element buffer object to hold indices data
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawPlaneIndices), drawPlaneIndices, GL_DYNAMIC_DRAW); 


    cout << "Beginning main loop...\n";
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        //Use shader program
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        
        //Bind and draw the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //Update a vertex position for demonstration
        //Could be more efficient, just updating the whole buffer for now
        drawPlaneVertices[1] += 0.01f; // Move the first vertex to the right
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(drawPlaneVertices), drawPlaneVertices);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

    return 0;
}
