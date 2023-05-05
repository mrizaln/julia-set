#include <complex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <linear_set.h>
#include <render.h>
#include <shader.h>

#define LINEAR_SET_VALUE_TYPE double

// default values
#define WIDTH              640
#define HEIGHT             480
#define ZOOM               1.0
#define CENTER             0.00 + 0.00 * I
#define POI                0.00 + 0.00 * I
#define SCROLL_SENSITIVITY 5.0
#define MOUSE_SENSITIVITY  1.0

typedef LINEAR_SET_VALUE_TYPE Complex_value;    // is the precision value for each real and imag
typedef unsigned int          Object;

// data
//----------------------------------------------------------------------------------------
Complex       g_center          = CENTER;
Complex       g_pointOfInterest = POI;
Complex_value g_zoom            = ZOOM;

/*! enum mouseCaptureMode
 *
 *  The two modes of mouse capture are:
 *      NONE  : mouse not captured;
 *      CENTER: move the origin of the Complex Plane;
 *      POI   : move point of interest (correspond to c in Julia Set)'
 */
// clang-format off
typedef enum { E_NONE, E_CENTER, E_POI } MouseCaptureMode;
MouseCaptureMode g_mouseCaptureMode = E_NONE;
// clang-format on
//----------------------------------------------------------------------------------------

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    const double multiplier = (SCROLL_SENSITIVITY / 100 + 1.0);
    g_zoom                  = (yOffset > 0 ? g_zoom * multiplier : g_zoom / multiplier);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    if ((key == GLFW_KEY_C) && action == GLFW_PRESS) {
        switch (g_mouseCaptureMode) {
        case E_CENTER:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            g_mouseCaptureMode = E_NONE;
            break;
        case E_NONE:
        case E_POI:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            g_mouseCaptureMode = E_CENTER;
            break;
        }
    }

    if ((key == GLFW_KEY_P) && action == GLFW_PRESS) {
        switch (g_mouseCaptureMode) {
        case E_POI:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            g_mouseCaptureMode = E_NONE;
            break;
        case E_NONE:
        case E_CENTER:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            g_mouseCaptureMode = E_POI;
            break;
        }
    }

    if ((key == GLFW_KEY_BACKSPACE) && action == GLFW_PRESS) {
        g_center          = CENTER;
        g_pointOfInterest = POI;
        g_zoom            = ZOOM;
    }
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
    static bool firstCapture = true;

    static double lastX = 0.0;
    static double lastY = 0.0;

    if (firstCapture)    // run once
    {
        firstCapture = false;
        lastX        = xPos;
        lastY        = yPos;
        return;
    }

    Complex* toBeModified = NULL;
    switch (g_mouseCaptureMode) {
    case E_NONE:
        firstCapture = true;
        return;
    case E_CENTER:
        toBeModified = &g_center;
        break;
    case E_POI:
        toBeModified = &g_pointOfInterest;
        break;
    }

    const double speed = 0.002 * MOUSE_SENSITIVITY * (g_mouseCaptureMode == E_POI ? 0.2 : 1.0);

    double xOffset = xPos - lastX;
    double yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    *toBeModified += (xOffset + yOffset * I) * speed / g_zoom;
}

Object buildRectangle(void)
{
    // clang-format off
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    // clang-format on

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // linking vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void render(void)
{
    GLFWwindow* window = initializeWindow(WIDTH, HEIGHT, "Julia Set");
    if (!window) {
        printf("Window creation failed: NULL\n");
        printf("Your display probably isn't running\n");
        return;
    }

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    Shader shader = createShader(
        "./resources/shaders/shader.vert",
        "./resources/shaders/shader.frag"
    );
    Object rectangle = buildRectangle();

    while (!glfwWindowShouldClose(window)) {
        // render
        // ------
        // reset color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use shader
        glUseProgram(shader);

        // update uniforms
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glUniform2f(glGetUniformLocation(shader, "uWindowSize"), width, height);
        glUniform2f(glGetUniformLocation(shader, "uCenter"), creal(g_center), cimag(g_center));
        glUniform2f(glGetUniformLocation(shader, "uPointOfInterest"), creal(g_pointOfInterest), cimag(g_pointOfInterest));
        glUniform1f(glGetUniformLocation(shader, "uZoom"), g_zoom);

        // draw
        glBindVertexArray(rectangle);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffer and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char* argv[])
{
    render();

    return 0;
}
