#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define LINEAR_SET_VALUE_TYPE double
#include <linear_set.h>
#include <render.h>
#include <shader.h>


int WIDTH = 600;
int HEIGHT = 400;

unsigned int buildRectangle(void)
{
    //float vertices[] = {
    //    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
    //     1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
    //     1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
    //    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
    //};

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void render(void)
{
    GLFWwindow* window = initializeWindow(WIDTH, HEIGHT, "Mandelbrot Set"); 

    Shader shader = createShader("./resources/shaders/shader.vs", "./resources/shaders/shader.fs");
    unsigned int rectangle = buildRectangle();

    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        // -----------------------
        glUseProgram(shader);
        glBindVertexArray(rectangle);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffer and poll IO events
        // ------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


int main(int argc, char *argv[])
{
    int height = 10;
    int width = 5;
    Complex center = 0.0 + 0.0*I;

    Complex* set = generateLinearSet(width, height, 5.0, &center);
    printLinearSet(width, height, set);
    deleteLinearSet(set);

    render();

    return 0;
}
