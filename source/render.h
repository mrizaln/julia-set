#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* initializeWindow(int width, int height, const char* windowName);
void        finalizeWindow(GLFWwindow* window);
void        framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif /* ifndef RENDER_H */
