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
int initialWidth = 800;
int initialHeight = 600;
float aspect = (float)initialWidth / (float)initialHeight;

// Prototyping functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    // GLFW Boilerplate
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(initialWidth, initialHeight, "The Kyle Huang Engine", NULL, NULL);
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
    glViewport(0,0,initialWidth,initialHeight);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Scaling the units so that 1.0f normalized coords is 25 meters
    float scale = 1.0f / 25.0f;
    
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
        float angle = (i-1) * 2.0f * M_PI / circle.resolution;
        // Using polar coordinates
        float x = cos(angle) * circle.radius;
        float y = sin(angle) * circle.radius;
        // z coordinate will always be 0
        float z = 0.0;
        circleVertices.push_back(x);
        circleVertices.push_back(y);
        circleVertices.push_back(z);
    }

    // VAO and VBO creation and binding
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
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

    GLuint aspectLoc = glGetUniformLocation(shaderProgram, "aspect");

    float prevTime = glfwGetTime();

    // Loop running every frame
    while (!glfwWindowShouldClose(window)) {
        // Time between each frame
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        // Physics running every timestep
        circle.vy += gravity * deltaTime;
        circle.y += circle.vy * deltaTime;

        // Collision checks
        if (circle.y - circle.radius < -1.0f) {
            circle.y = -1.0f + circle.radius;
            circle.vy *= -1.0f;
        }
        circleVertices[1] = circle.y;
        for (int i = 1; i <= circle.resolution + 1; ++i) {
            // The y coordinate
            int indexY = i * 3 + 1;
            // Update vertices
            float angle = (i-1) * 2.0f * M_PI / circle.resolution;
            circleVertices[indexY] = sin(angle) * circle.radius + circle.y;
            circleVertices[indexY - 1] = cos(angle) * circle.radius + circle.x;
        }
        glClearColor(0.0627450980f, 0.0705882353f, 0.0784313725f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glUniform1f(aspectLoc, aspect);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, circleVertices.size() * sizeof(float), circleVertices.data());
        glDrawArrays(GL_TRIANGLE_FAN, 0, circle.resolution + 2);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

// Window Resize Function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    aspect = (float)width / (float)height;
    fmt::print("Aspect Ratio: {}\n", aspect);
}