#include <shader.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define NODE_VALUE_TYPE char
typedef struct _Node {
    NODE_VALUE_TYPE value;
    struct _Node* next;
} Node;

Node* createNewNode(NODE_VALUE_TYPE value)
{
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;
    return node;
}

char* readShaderFile(const char* shaderFilePath)
{
    FILE* shaderFile = fopen(shaderFilePath, "r");
    if (!shaderFile)
        return NULL;

    Node* head = createNewNode(fgetc(shaderFile));
    int length = 1;

    char tmp;
    Node* left = head;
    Node* right = NULL;
    while ((tmp = (char)fgetc(shaderFile)) != EOF)
    {
        ++length;
        right = createNewNode(tmp);
        left->next = right;
        left = right;
    }

    char* fileContent = malloc(sizeof(char)*(length+1));
    Node* probe = head;
    for (int i = 0; i < length; ++i)
    {
        fileContent[i] = probe->value;
        Node* tmp = probe;
        probe = probe->next;
        free(tmp);
    }
    fileContent[length] = '\00';

    return fileContent;
}

void checkCompileErrors(unsigned int shader, const char* type)
{
    printf("%s SHADER COMPILE INFO\n", type);
    printf("--------------------------------------------------------------------------------\n");
    int success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM"))   // if not equal
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n", type);
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", type);
    }
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    printf("%s\n", infoLog);
    printf("--------------------------------------------------------------------------------\n\n");
}

Shader createShader(const char* vShaderPath, const char* fShaderPath)
{
    const char* vShaderCode = readShaderFile(vShaderPath);
    const char* fShaderCode = readShaderFile(fShaderPath);

    if (!vShaderCode) printf("vertex shader file not found\n");
    if (!fShaderCode) printf("fragment shader file not found\n");

    // 2. compile shaders
        //-----------------------------------------------------------------------------------
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");   // print compile error if any

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");   // print compile error if any

    // shader program
    Shader ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");  // print linking error if any

    // delete the shaders as they're linked onto our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free((void*)vShaderCode);
    free((void*)fShaderCode);

    return ID;
}

