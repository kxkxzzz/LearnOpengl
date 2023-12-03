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
const char* vertexShaderSrc = "../../../shader/Lighting/p6/shader.vs";
const char* fragmentShaderSrc = "../../../shader/Lighting/p6/shader.fs";
const char* lightVertexShaderSrc = "../../../shader/Lighting/p6/light.vs";
const char* lightFragmentShaderSrc = "../../../shader/Lighting/p6/light.fs";
const char* textureSrc = "../../../img/container.jpg";

// camera
// Camera camera(glm::vec3(3.7342, 2.0652f, -1.6181f), glm::vec3(0.0f, 1.0f, 0.0f), 144.3973f,
// -26.8f);
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
    Shader lightShader(lightVertexShaderSrc, lightFragmentShaderSrc);

    float vertices[] = {
        // positions         // normals    // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  //
        0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,  //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,  //
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,  //
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,  //

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  //
        0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  //
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,  //
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  //

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  //
        -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,  //
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,  //
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,  //
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,  //

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  //
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  //

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,  //
        0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,  //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,  //
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,  //
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,  //
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,  //

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  //
        0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,  //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  //
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,  //
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,  //
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f  //
    };

    glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                 glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                 glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                 glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                 glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
    glm::vec3 pointLightPositions[] = {glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
                                       glm::vec3(-4.0f, 2.0f, -12.0f),
                                       glm::vec3(0.0f, 0.0f, -3.0f)};

    GLuint VAO[2], VBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    // a cube
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    {
        cubeShader.use();
        GLuint aPosLoc = glGetAttribLocation(cubeShader.ID, "aPos");
        glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(aPosLoc);
        GLuint aNormalLoc = glGetAttribLocation(cubeShader.ID, "aNormal");
        glVertexAttribPointer(aNormalLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(aNormalLoc);

        GLuint aTexCoordsLoc = glGetAttribLocation(cubeShader.ID, "aTexCoords");
        glVertexAttribPointer(aTexCoordsLoc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (void*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(aTexCoordsLoc);
    }

    GLuint diffuseMap = loadTexture("../../../img/woodBox.png");
    GLuint specularMap = loadTexture("../../../img/woodBoxSpecular.png");
    cubeShader.use();
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);

    // light
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    {
        lightShader.use();
        GLuint aPosLoc = glGetAttribLocation(lightShader.ID, "aPos");
        glVertexAttribPointer(aPosLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(aPosLoc);
    }

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render
        glm::mat4 view(1.0f);
        view = camera.GetViewMatrix();
        glm::mat4 cubeModel(1.0f), Projection(1.0f);
        Projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT,
                                      0.1f, 100.0f);
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

        glm::vec3 lightAmbient(0.2f), lightDiffuse(0.5f), lightSpecular(1.0f);

        cubeShader.use();
        // directional light
        cubeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cubeShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        cubeShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        cubeShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        cubeShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLights[0].constant", 1.0f);
        cubeShader.setFloat("pointLights[0].linear", 0.09f);
        cubeShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        cubeShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        cubeShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLights[1].constant", 1.0f);
        cubeShader.setFloat("pointLights[1].linear", 0.09f);
        cubeShader.setFloat("pointLights[1].quadratic", 0.032f);
        // spotLight
        cubeShader.setVec3("spotLight.position", camera.Position);
        cubeShader.setVec3("spotLight.direction", camera.Front);
        cubeShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        cubeShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        cubeShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("spotLight.constant", 1.0f);
        cubeShader.setFloat("spotLight.linear", 0.09f);
        cubeShader.setFloat("spotLight.quadratic", 0.032f);
        cubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        cubeShader.setFloat("material.shininess", 64.0f);
        cubeShader.setVec3("viewPos", camera.Position);
        // cubeShader.setMat4("model", cubeModel);
        cubeShader.setMat4("projection", Projection);
        cubeShader.setMat4("view", view);

        glCheckError();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO[0]);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // light
        lightShader.use();
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightModel);
        lightShader.setMat4("projection", Projection);
        lightShader.setMat4("view", view);
        glBindVertexArray(VAO[1]);
        for (unsigned int i = 0; i < 2; i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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