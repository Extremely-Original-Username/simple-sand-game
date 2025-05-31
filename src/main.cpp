#include <iostream>
#include <GLFW/glfw3.h>
#include <string>
#include <cstdlib>
#include "mainHelpers.h"
using namespace std;

const string title = "Simple Sand!";
const int windowWidth = 500;
const int windowHeight = 450;
const int gridWidth = 100;
const int gridHeight = 90;

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

    cout << "Initialising data...\n";
    bool sandGrid [gridWidth][gridHeight];
    for (int x = 0; x < gridWidth; x++)
    {
        for (int y = 0; y < gridHeight; y++)
        {
            sandGrid[x][y] = rand() % 3 == 0;
        }
    }
    

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
