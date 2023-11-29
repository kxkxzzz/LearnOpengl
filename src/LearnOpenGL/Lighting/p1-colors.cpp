#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shaders.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "checkError.h"

constexpr int SCR_WIDTH = 800, SCR_HEIGHT = 600;
const char* vertexShaderSrc = "../../../shader/Lighting/p1/shader.vs";
const char* fragmentShaderSrc = "../../../shader/Lighting/p1/shader.fs";
const char* lightFragmentShaderSrc = "../../../shader/Lighting/p1/light.fs";
const char* textureSrc = "../../../img/container.jpg";

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader cubeShader(vertexShaderSrc, fragmentShaderSrc);
    Shader lightShader(vertexShaderSrc, lightFragmentShaderSrc);

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
        0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //
        -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,  //

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,  //
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f  //
    };

    GLuint VAO[2], VBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    // a cube
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // light
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    {
        cubeShader.use();
        GLuint aPosLoc = glGetAttribLocation(cubeShader.ID, "aPos");
        glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(aPosLoc);
    }
    {
        lightShader.use();
        GLuint aPosLoc = glGetAttribLocation(lightShader.ID, "aPos");
        glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(aPosLoc);
    }

    cubeShader.use();
    GLuint cubeModelLoc = glGetUniformLocation(cubeShader.ID, "model");
    GLuint cubeProjectionLoc = glGetUniformLocation(cubeShader.ID, "projection");

    glm::mat4 cubeModel(1.0f), cubeProjection(1.0f);
    cubeProjection =
        glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(cubeModelLoc, 1, GL_FALSE, glm::value_ptr(cubeModel));
    glUniformMatrix4fv(cubeProjectionLoc, 1, GL_FALSE, glm::value_ptr(cubeProjection));

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render
        glm::mat4 view(1.0f);
        view = camera.GetViewMatrix();
        // cube
        cubeShader.use();
        GLuint cubeViewLoc = glGetUniformLocation(cubeShader.ID, "view");
        glUniformMatrix4fv(cubeViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        cubeShader.setMat4("view", view);
        cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // // light
        // lightShader.use();
        // GLuint lightViewLoc = glGetUniformLocation(lightShader.ID, "view");
        // glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // glBindVertexArray(VAO[1]);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}