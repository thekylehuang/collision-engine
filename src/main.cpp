#include "shader.h"
#include "buffer_utils.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fmt/core.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Initial window conditions
int currentWidth = 1200;
int currentHeight = 800;

// Prototype for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    // GLFW Boilerplate
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(currentWidth, currentHeight, "The Kyle Huang Engine", NULL, NULL);
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
    glViewport(0,0,currentWidth,currentHeight);
    
    // Callbacks here
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Window icon
    int iconWidth, iconHeight, iconChannels;
    unsigned char* iconPixels = stbi_load("../assets/gravity-engine-icon.png", &iconWidth, &iconHeight, &iconChannels, 4);
    GLFWimage icon;
    icon.width = iconWidth;
    icon.height = iconHeight;
    icon.pixels = iconPixels;
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(iconPixels);

    // Scaling the units so that 1.0f normalized coords is 2.54 meters
    float scale = 1.0f / 2.54f;
    
    // Fullscreen quad for grid
    float gridQuadVertices[] = {
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    Buffer gridQuad = createVertexBuffer(gridQuadVertices, 6, 3);
    GLuint gridShaderProgram = compileShaderProgram(
        "../shaders/grid_vertex.glsl",
        "../shaders/grid_fragment.glsl"
    );

    // Uniform variables passed to shaders
    GLuint resLocGrid = glGetUniformLocation(gridShaderProgram, "iResolution");

    // Setup for delta time
    float prevTime = glfwGetTime();

    // Frame loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Grid
        glUseProgram(gridShaderProgram);
        glUniform2f(resLocGrid, (float)currentWidth, (float)currentHeight);
        glBindVertexArray(gridQuad.VAO);
        glDrawArrays(GL_TRIANGLES, 0, gridQuad.vertexCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

// Window Resize Function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    currentWidth = width;
    currentHeight = height;
}