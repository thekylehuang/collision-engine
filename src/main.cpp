#include "shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <fmt/core.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Prototyping functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    // GLFW Boilerplate
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "The Kyle Huang Engine", NULL, NULL);
    if (!window) {
        fmt::print(stderr, "Failed to create window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        fmt::print(stderr, "Failed to initialize GLAD.\n");
        return -1;
    }
    glViewport(0,0,800,600);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // VAO and VBO creation and binding
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex and Fragment setup
    std::string vertexSource = readShaderFile("../shaders/vertex.glsl");
    std::string fragmentSource = readShaderFile("../shaders/fragment.glsl");
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexSourceCStr = vertexSource.c_str();
    const char* fragmentSourceCStr = fragmentSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    // Shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);


    // Loop running every frame
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

// Window Resize Function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    fmt::print("Width: {} Height: {}\n", width, height);
}