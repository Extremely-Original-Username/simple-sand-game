#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glext.h>
#include <string>
#include <cstdlib>
#include "mainHelpers.h"
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <unistd.h>

using namespace std;

const string title = "Simple Sand!";
const int gridWidth = 100;
const int gridHeight = 90;

int windowWidth = 500;
int windowHeight = 450;

//Entry point function
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
    const string computeShaderSource = loadFileToString("./src/shaders/compute.glsl");

    const char* vertexShaderSourcePtr = &vertexShaderSource[0];
    const char* fragmentShaderSourcePtr = &fragmentShaderSource[0];
    const char* computeShaderSourcePtr = &computeShaderSource[0];

    //Must happen AFTER the context is set
    cout << "Compiling shaders...\n";

    //Create rendering pipeline program
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

    //Query the shader program for uniforms
    int shaderProgramResolutionUniformLocation = glGetUniformLocation(shaderProgram, "MainWindowResolution");
    int sandGridResolutionUniformLocation = glGetUniformLocation(shaderProgram, "SandGridResolution");
    int SandGridTextureUniformLocation = glGetUniformLocation(shaderProgram, "SandGridTexture");

    //Set constant uniforms
    glUseProgram(shaderProgram);
    glUniform2f(sandGridResolutionUniformLocation, gridWidth, gridHeight);

    //Create compute shader program
    unsigned int computeShader;
computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderSourcePtr, NULL);
    glCompileShader(computeShader);
    checkShaderCompileErrors(computeShader);

    unsigned int computeShaderProgram;
    computeShaderProgram = glCreateProgram();
    glAttachShader(computeShaderProgram, computeShader);
    glLinkProgram(computeShaderProgram);
    checkProgramLinkErrors(computeShaderProgram);
    
    //Not needed once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  
    glDeleteShader(computeShader);

    cout << "Initialising data...\n";
    bool sandGrid [gridWidth][gridHeight];
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            sandGrid[x][y] = rand() % 5 == 0;
        }
    }

    float drawPlaneVertices[] = {
        1.0f,  1.0f, 0.0f,  // top right
        1.0f, -1.0f, 0.0f,  // bottom right
       -1.0f, -1.0f, 0.0f,  // bottom left
       -1.0f,  1.0f, 0.0f   // top left 
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

    //Generate texture for the sand grid
    cout << "Generating textures...\n";
    unsigned int sandGridTexture;
    glGenTextures(1, &sandGridTexture);
    /*TODO: Do I need both?*/
    glBindTexture(GL_TEXTURE_2D, sandGridTexture);
    glBindImageTexture(0, sandGridTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8);

    float sandGridData[gridWidth * gridHeight];
    //TODO: REMOVE THIS AND JUST USE 1D ARRAY
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            sandGridData[y * gridWidth + x] = sandGrid[x][y] ? 1.0f : 0.0f;
        }
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, gridWidth, gridHeight, 0, GL_RED, GL_FLOAT, sandGridData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    cout << "Beginning main loop...\n";
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        /* Render here */
        //Use rendering pipeline shader program
        glUniform2f(shaderProgramResolutionUniformLocation, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        
        //Bind and draw the EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /*Compute sand*/
        

        // Bind the texture to the compute shader
        glUseProgram(computeShaderProgram);

        // Dispatch the compute shader
        glDispatchCompute(gridWidth, gridHeight, 1);

        // Ensure the compute shader has completed before rendering
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        /* Poll for and process events */
        glfwPollEvents();
        //sleep(0.01);
    }

    glfwTerminate();
    return 0;
}

// Callback to adjust the viewport when the window is resized
inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}
