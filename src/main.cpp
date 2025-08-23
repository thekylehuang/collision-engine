#include <iostream>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void click_for_color(GLFWwindow *window, int button, int action, int mods);

// Define clear color
float clear_r = 0.0f;
float clear_g = 0.0f;
float clear_b = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "The Kyle Huang Engine", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, click_for_color);

    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0,0,800,600);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Game Loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(clear_r, clear_g, clear_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

// Resize Function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Escape window function
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Click to random color function
void click_for_color(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::random_device rd;
        std::mt19937 gen (rd());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        clear_r = dist(gen);
        clear_g = dist(gen);
        clear_b = dist(gen);
    }
}