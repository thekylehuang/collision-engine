#include "shader.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <fmt/core.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Circle data type
struct circle {
    float x, y;
    float vx, vy;
    float radius;
    int resolution;
};

// Initial window conditions
int currentWidth = 2240;
int currentHeight = 1205;

// Prototyping functions
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
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Scaling the units so that 1.0f normalized coords is 25 meters
    float scale = 1.0f / 25.0f;
    
    // Fullscreen quad for grid
    float quad[] = {
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    // Draw a circle
    circle circle  ={.x=0.0f, .y=0.5f, .vx=0.0f, .vy=0.0f, .radius=5.0f * scale, .resolution=512};
    const float gravity = -9.80665f * scale;
    std::vector<float> circleVertices;

    // Center point (3 floats)
    circleVertices.push_back(0.0f);
    circleVertices.push_back(0.0f);
    circleVertices.push_back(0.0f);

    for (int i = 0; i <= circle.resolution + 1; ++i) {
        // The angle increment is equal to 2pi divided by number of segments.
        float angle = (i) * 2.0f * M_PI / circle.resolution;
        // Using polar coordinates
        float x = cos(angle) * circle.radius;
        float y = sin(angle) * circle.radius;
        // z coordinate will always be 0
        float z = 0.0;
        circleVertices.push_back(x);
        circleVertices.push_back(y);
        circleVertices.push_back(z);
    }

    // Quad
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Circle
    GLuint circleVAO, circleVBO;
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint circleShaderProgram = compileShaderProgram(
        "../shaders/circle_vertex.glsl",
        "../shaders/circle_fragment.glsl"
    );

    GLuint gridShaderProgram = compileShaderProgram(
        "../shaders/grid_vertex.glsl",
        "../shaders/grid_fragment.glsl"
    );

    GLuint aspectLoc = glGetUniformLocation(circleShaderProgram, "aspect");
    GLuint circlePosLoc = glGetUniformLocation(circleShaderProgram, "circlePos");
    GLuint resLocGrid = glGetUniformLocation(gridShaderProgram, "iResolution");
    GLuint resLocCircle = glGetUniformLocation(circleShaderProgram, "iResolution");

    float prevTime = glfwGetTime();

    // Frame loop
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        circle.vy += gravity * deltaTime;
        circle.y += circle.vy * deltaTime;

        if (circle.y - circle.radius < -1.0f) {
            circle.y = -1.0f + circle.radius;
            circle.vy *= -1.0f;
        }

        glClearColor(0.0627450980f, 0.0705882353f, 0.0784313725f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Grid
        glUseProgram(gridShaderProgram);
        glUniform2f(resLocGrid, (float)currentWidth, (float)currentHeight);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Circle
        glUseProgram(circleShaderProgram);
        glUniform2f(resLocCircle, (float)currentWidth, (float)currentHeight);
        glUniform2f(circlePosLoc, circle.x, circle.y);

        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circle.resolution + 2);
        glBindVertexArray(0);

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