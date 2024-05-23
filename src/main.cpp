#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

// #define GL_SILENCE_DEPRECATION 


int main ()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.5f, 0.0, 0.4f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {   
        // glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}