#include <iostream>
#include <GLFW/glfw3.h>
#include <string>
using namespace std;

const string title = "Simple Sand!";
const int windowWidth = 500;
const int windowHeight = 450;

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

    cout << "Setting context...\n";
    glfwMakeContextCurrent(window);

    cout << "Beginning main loop...\n";
    while (!glfwWindowShouldClose(window))
    {
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