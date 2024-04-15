#include <render.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>


GLFWwindow* initializeWindow(int width, int height, const char* windowName)
{
    // initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    GLFWwindow* window = glfwCreateWindow(width, height, windowName, NULL, NULL);
    if (!window) {
        printf("Failed to create Window\n");
        return NULL;
    }

    glfwMakeContextCurrent(window);

    // glad load all OpengGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize glad\n");
        return NULL;
    }

    // set callbacks
    //--------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    //glfwSetKeyCallback(window, key_callback);

    // glEnable(GL_DEPTH_TEST);         // depth test
    // glEnable(GL_BLEND);              // transparency

    return window;
}

void finalizeWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// window resize callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

