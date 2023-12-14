#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>

#include "shaders.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "checkError.h"

constexpr int SCR_WIDTH = 800, SCR_HEIGHT = 600;
const char* VShaderSrc = "../../../shader/AdvancedOpenGL/p4/shader.vs";
const char* FShaderSrc = "../../../shader/AdvancedOpenGL/p4/blending.fs";
const char* cubeTextureSrc = "../../../img/metal.png";
const char* planeTextureSrc = "../../../img/marble.jpg";
const char* windowTextureSrc = "../../../img/pinkWindow.png";

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);
void bindTexture(GLuint textureID, const int bias = 0);

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

    glEnable(GL_DEPTH_TEST);

    Shader shader(VShaderSrc, FShaderSrc);

    float cubeVertices[] = {
        // positions          // texture Coords
        // back
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f,  //
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  //
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  //
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  //
        // front
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  //
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,  //
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,  //
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
        // left
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  //
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,  //
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  //
        // right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  //
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,  //
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  //
        // bottom
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  //
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  //
        // top
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  //
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,  //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  //
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  //
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f  //
    };
    float planeVertices[] = {
        // positions          // texture Coords
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,  //
        -5.0f, -0.5f, 5.0f,  0.0f, 0.0f,  //
        5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,  //

        5.0f,  -0.5f, 5.0f,  2.0f, 0.0f,  //
        5.0f,  -0.5f, -5.0f, 0.0f, 2.0f,  //
        -5.0f, -0.5f, -5.0f, 2.0f, 2.0f  //
    };

    float windowVertices[] = {
        // positions         // texture Coords
        0.0f, 0.5f,  0.0f, 0.0f, 0.0f,  //
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f,  //
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,  //

        0.0f, 0.5f,  0.0f, 0.0f, 0.0f,  //
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,  //
        1.0f, 0.5f,  0.0f, 1.0f, 0.0f  //
    };

    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    {
        shader.use();
        GLuint aPosLoc = glGetAttribLocation(shader.ID, "aPos");
        GLuint aTexCoordsLoc = glGetAttribLocation(shader.ID, "aTexCoords");
        glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(aTexCoordsLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(aPosLoc);
        glEnableVertexAttribArray(aTexCoordsLoc);
    }
    glBindVertexArray(0);

    GLuint planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    {
        shader.use();
        GLuint aPosLoc = glGetAttribLocation(shader.ID, "aPos");
        GLuint aTexCoordsLoc = glGetAttribLocation(shader.ID, "aTexCoords");
        glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(aTexCoordsLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(aPosLoc);
        glEnableVertexAttribArray(aTexCoordsLoc);
    }
    glBindVertexArray(0);

    GLuint windowVAO, windowVBO;
    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);

    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), windowVertices, GL_STATIC_DRAW);
    {
        shader.use();
        GLuint aPosLoc = glGetAttribLocation(shader.ID, "aPos");
        GLuint aTexCoordsLoc = glGetAttribLocation(shader.ID, "aTexCoords");
        glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(aTexCoordsLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(aPosLoc);
        glEnableVertexAttribArray(aTexCoordsLoc);
    }
    glBindVertexArray(0);

    GLuint cubeTexture = loadTexture(cubeTextureSrc);
    GLuint planeTexture = loadTexture(planeTextureSrc);
    GLuint windowTexture = loadTexture(windowTextureSrc);

    shader.use();
    shader.setInt("texture0", 0);

    std::vector<glm::vec3> windows{glm::vec3(-1.5f, 0.0f, -0.48f), glm::vec3(1.5f, 0.0f, 0.51f),
                                   glm::vec3(0.0f, 0.0f, 0.7f), glm::vec3(-0.3f, 0.0f, -2.3f),
                                   glm::vec3(0.5f, 0.0f, -0.6f)};

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++) {
            float distance = glm::length(camera.Position - windows[i]);
            sorted[distance] = windows[i];
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render
        glm::mat4 model(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // glCullFace(GL_FRONT);
        glBindVertexArray(cubeVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        bindTexture(cubeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDisable(GL_CULL_FACE);
        // window
        glBindVertexArray(windowVAO);
        bindTexture(windowTexture);
        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
            auto [dist, vec] = *it;
            model = glm::mat4(1.0f);
            model = glm::translate(model, vec);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // plane
        glBindVertexArray(planeVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        bindTexture(planeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glCheckError();
    glfwTerminate();
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

unsigned int loadTexture(char const* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void bindTexture(GLuint textureID, const int bias) {
    glActiveTexture(GL_TEXTURE0 + bias);
    glBindTexture(GL_TEXTURE_2D, textureID);
}