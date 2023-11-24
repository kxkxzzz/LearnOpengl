#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr int SCR_WIDTH = 800, SCR_HEIGHT = 600;
const char* vertexShaderSrc = "../../../shader/Lighting/p1/shader.vs";
const char* fragmentShaderSrc = "../../../shader/Lighting/p1/shader.fs";
const char* lightFragmentShaderSrc = "../../../shader/Lighting/p1/light.fs";
const char* textureSrc = "../../../img/container.jpg";

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

int main() {

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "color", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to load glad!" << std::endl;
        return -1;
    }

    Shader shader(vertexShaderSrc, fragmentShaderSrc);
    Shader lightShader(vertexShaderSrc, lightFragmentShaderSrc);

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}