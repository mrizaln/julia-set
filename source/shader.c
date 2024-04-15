#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readShaderFile(const char* shaderFilePath)
{
    FILE* shaderFile = fopen(shaderFilePath, "r");
    if (!shaderFile) {
        return NULL;
    }

    fseek(shaderFile, 0, SEEK_END);
    size_t length = (size_t)ftell(shaderFile);

    fseek(shaderFile, 0, SEEK_SET);

    char* string = malloc((length + 1) * sizeof(char));
    fread(string, 1, length, shaderFile);
    string[length] = '\00';

    return string;
}

void checkCompileErrors(unsigned int shader, const char* type)
{
    printf("%s SHADER COMPILE INFO\n", type);
    printf("--------------------------------------------------------------------------------\n");
    int  success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM"))    // if not equal
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n", type);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", type);
        }
    }
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    printf("%s\n", infoLog);
    printf("--------------------------------------------------------------------------------\n\n");
}

Shader createShader(const char* vShaderPath, const char* fShaderPath)
{
    const char* vShaderCode = readShaderFile(vShaderPath);
    const char* fShaderCode = readShaderFile(fShaderPath);

    if (!vShaderCode) {
        printf("vertex shader file not found\n");
    }
    if (!fShaderCode) {
        printf("fragment shader file not found\n");
    }

    // 2. compile shaders
    //-----------------------------------------------------------------------------------
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");    // print compile error if any

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");    // print compile error if any

    // shader program
    Shader ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");    // print linking error if any

    // delete the shaders as they're linked onto our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free((void*)vShaderCode);
    free((void*)fShaderCode);

    return ID;
}
