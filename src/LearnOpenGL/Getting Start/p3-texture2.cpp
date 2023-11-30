#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include "shaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

constexpr int SCR_WIDTH = 800, SCR_HEIGHT = 600;
const char* vertexShaderSrc = "../../../shader/GettingStart/p3-texture/shader1.vs";
const char* fragmentShaderSrc = "../../../shader/GettingStart/p3-texture/shader1.fs";
const char* textureSrc = "../../../img/container.jpg";
const char* textureSrc2 = "../../../img/awesomeface.png";

void bufferSizeCallBack(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processinput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "texture", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        std::cout << "Failed to create a window!";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, bufferSizeCallBack);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to load glad!" << std::endl;
        return -1;
    }

    Shader shaderPro(vertexShaderSrc, fragmentShaderSrc);

    // float vertices[] = {
    //     //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
    //     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下
    //     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
    // };
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
        1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下
        -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
    };

    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // std::cout << glGetAttribLocation(shaderPro.ID, "aColo");

    glVertexAttribPointer(glGetAttribLocation(shaderPro.ID, "aPos"), 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)0);
    glVertexAttribPointer(glGetAttribLocation(shaderPro.ID, "aColor"), 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(glGetAttribLocation(shaderPro.ID, "aTexCoord"), 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(glGetAttribLocation(shaderPro.ID, "aPos"));
    glEnableVertexAttribArray(glGetAttribLocation(shaderPro.ID, "aColor"));
    glEnableVertexAttribArray(glGetAttribLocation(shaderPro.ID, "aTexCoord"));

    unsigned int texture[2];
    glGenTextures(2, texture);
    // glGenTextures(1, &texture[0]);
    // glGenTextures(1, &texture[1]);

    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(textureSrc, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         data);

            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(textureSrc2, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load image" << std::endl;
        }
        stbi_image_free(data);
    }

    shaderPro.use();
    glUniform1i(glGetUniformLocation(shaderPro.ID, "ourTexture1"), 0);
    glUniform1i(glGetUniformLocation(shaderPro.ID, "ourTexture2"), 1);

    while (!glfwWindowShouldClose(window)) {

        processinput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture[0]);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture[1]);

        shaderPro.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}