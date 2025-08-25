#include "shader.h"
#include "buffer_utils.h"
#include "circle.h"

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
int currentWidth = 1600;
int currentHeight = 800;

float scale = 1.0f / 2.54f; // Scaling the units so that 1.0f normalized coords is 2.54 meters

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Vector containing circles
std::vector<Circle> circles;

// Drag Globals
bool isDragging = false;
float dragStartX = 0.0f;
float dragStartY = 0.0f;

// Instance buffer for circle positions
GLuint instanceVBO;

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Window icon
    int iconWidth, iconHeight, iconChannels;
    unsigned char* iconPixels = stbi_load("../assets/gravity-engine-icon.png", &iconWidth, &iconHeight, &iconChannels, 4);
    GLFWimage icon;
    icon.width = iconWidth;
    icon.height = iconHeight;
    icon.pixels = iconPixels;
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(iconPixels);

    // Fullscreen quad for grid
    float gridQuadVertices[] = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    Buffer gridQuad = createVertexBuffer(gridQuadVertices, 6, 3);
    GLuint gridShaderProgram = compileShaderProgram(
        "../shaders/grid_vertex.glsl",
        "../shaders/grid_fragment.glsl"
    );

    // Circle geometry definition
    std::vector<float> templateCircleVertices;
    templateCircleVertices.push_back(0);
    templateCircleVertices.push_back(0);
    templateCircleVertices.push_back(0.0f);
    int templateCircleResolution = 64;
    float templateCircleRadius = 0.08f; // Make circles visible
    for (int i =0; i <= templateCircleResolution; ++i) {
        float angle = i * 2.0f * M_PI / templateCircleResolution;
        templateCircleVertices.push_back(cos(angle) * templateCircleRadius);
        templateCircleVertices.push_back(sin(angle) * templateCircleRadius);
        templateCircleVertices.push_back(0.0f);
    }

    // Upload our geometry definition
    GLuint templateCircleVBO, circleVAO;
    glGenVertexArrays(1, &circleVAO);
    glBindVertexArray(circleVAO);

    glGenBuffers(1, &templateCircleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, templateCircleVBO);
    glBufferData(GL_ARRAY_BUFFER, templateCircleVertices.size() * sizeof(float), templateCircleVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Upload instanced positions of all circles to the GPU
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    std::vector<float> instanceData;
    instanceData.reserve(circles.size() * 2);

    for (const Circle& c : circles) {
        instanceData.push_back(c.x);
        instanceData.push_back(c.y);
    }

    glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(float), instanceData.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);
    glBindVertexArray(0);

    GLuint circleShaderProgram = compileShaderProgram(
        "../shaders/circle_vertex.glsl",
        "../shaders/circle_fragment.glsl"
    );

    // Uniform variables passed to shaders
    GLuint resLocGrid = glGetUniformLocation(gridShaderProgram, "iResolution");
    GLuint aspectLoc = glGetUniformLocation(circleShaderProgram, "aspectRatio");
    float aspect = (float)currentWidth / (float)currentHeight;

    // Setup for delta time
    float prevTime = glfwGetTime();

    // Frame loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - prevTime;
        prevTime = currentTime;
        if (!circles.empty()) {
            for (Circle& circle : circles) {
                circle.x += circle.vx * deltaTime;
                circle.y += circle.vy * deltaTime;
            }

            // Check for collisions between all pairs of circles
            for (size_t i = 0; i < circles.size(); ++i) {
                for (size_t j = i + 1; j < circles.size(); ++j) {
                    circles[i].applyCollision(circles[j]);
                }
            }

            // Check for border collisions
            for (Circle& circle : circles) {
                // Left and right borders
                if (circle.x - circle.radius < -1.0f) {
                    circle.x = -1.0f + circle.radius;
                    circle.vx = -circle.vx;
                }
                if (circle.x + circle.radius > 1.0f) {
                    circle.x = 1.0f - circle.radius;
                    circle.vx = -circle.vx;
                }
                
                // Top and bottom borders
                if (circle.y - circle.radius < -1.0f) {
                    circle.y = -1.0f + circle.radius;
                    circle.vy = -circle.vy;
                }
                if (circle.y + circle.radius > 1.0f) {
                    circle.y = 1.0f - circle.radius;
                    circle.vy = -circle.vy;
                }
            }

            // Update instance buffer with new positions
            std::vector<float> instanceData;
            instanceData.reserve(circles.size() * 2);
            for (const Circle& c : circles) {
                instanceData.push_back(c.x);
                instanceData.push_back(c.y);
            }
            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, instanceData.size() * sizeof(float), instanceData.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // Update aspect ratio
        aspect = (float)currentWidth / (float)currentHeight;

        // Grid
        glUseProgram(gridShaderProgram);
        glUniform2f(resLocGrid, (float)currentWidth, (float)currentHeight);
        glBindVertexArray(gridQuad.VAO);
        glDrawArrays(GL_TRIANGLES, 0, gridQuad.vertexCount);

        // Instancing rendering circles
        glUseProgram(circleShaderProgram);
        glUniform1f(aspectLoc, aspect);
        glBindVertexArray(circleVAO);
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, templateCircleVertices.size() / 3, circles.size());
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

// Mouse callback function
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    float x_ndc = (xpos / (float)currentWidth) * 2.0f - 1.0f;
    float y_ndc = 1.0f - (ypos / (float)currentHeight) * 2.0f;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        isDragging = true;
        dragStartX = x_ndc;
        dragStartY = y_ndc;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        isDragging = false;
        double dx = dragStartX - x_ndc;
        double dy = dragStartY - y_ndc;

        circles.emplace_back(dragStartX, dragStartY, dx * 2.0f, dy * 2.0f, 0.08f, 1.0f);

        std::vector<float> instanceData;
        instanceData.reserve(circles.size() * 2);
        for (const Circle& c : circles) {
            instanceData.push_back(c.x);
            instanceData.push_back(c.y);
        }
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(float), instanceData.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        fmt::print("Spawned a circle at X: {} Y: {}\n", x_ndc, y_ndc);
        fmt::print("Velocity X: {}\n", dx * 2.0f);
        fmt::print("Velocity Y: {}\n", dy * 2.0f);
    }
}