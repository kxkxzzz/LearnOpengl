#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
out vec3 fragColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    fragColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 fragColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(fragColor, 1.0);
}
)";

int main() {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 创建GLFW窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "Neon Cube", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将OpenGL上下文设置为当前
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // 检查顶点着色器编译错误

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 检查片段着色器编译错误

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 检查程序链接错误

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 定义顶点数据，包括顶点坐标和颜色
    float vertices[] = {
        // 前面
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // 左下 红色
        0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.5f,  // 右下 紫色
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,  // 右上 白色
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,  // 左上 黄色

        // 后面
        -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f,  // 左下 橙色
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // 右下 蓝色
        0.5f, 0.5f, -0.5f, 0.5f, 0.8f, 0.9f,  // 右上 天蓝
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f  // 左上 绿色
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,  //
        4, 5, 6, 6, 7, 4,  //
        0, 3, 7, 7, 4, 0,  //
        1, 2, 6, 6, 5, 1,  //
        0, 1, 5, 5, 4, 0,  //
        2, 3, 7, 7, 6, 2  //
    };

    // 创建和绑定顶点数组对象 (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 创建和绑定顶点缓冲对象 (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 创建和绑定索引缓冲对象 (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 使用着色器程序
    glUseProgram(shaderProgram);

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 设置模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                           glm::value_ptr(model));

        // 设置投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE,
                           glm ::value_ptr(projection));

        // 设置视图矩阵
        glm::mat4 view = glm::lookAt(glm::vec3(1.5f, 1.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));

        // 获取当前时间
        float time = static_cast<float>(glfwGetTime());

        // 计算颜色
        float colors[24];

        colors[0] = (sinf(time) + 1.0f) / 2.0f;
        colors[1] = (cosf(time) + 1.0f) / 2.0f;
        colors[2] = (sinf(time * 0.5) + 1.0f) / 2.0f;

        colors[3] = (cos(time * 0.7) + 1.0f) / 2.0f;
        colors[4] = (sin(time * 1.3) + 1.0f) / 2.0f;
        colors[5] = (cos(time * 0.9) + 1.0f) / 2.0f;

        colors[6] = (sin(time * 0.2) + 1.0f) / 2.0f;
        colors[7] = (cos(time * 0.4) + 1.0f) / 2.0f;
        colors[8] = (sin(time * 0.6) + 1.0f) / 2.0f;

        colors[9] = (cos(time * 1.2) + 1.0f) / 2.0f;
        colors[10] = (sin(time * 1.4) + 1.0f) / 2.0f;
        colors[11] = (cos(time * 1.6) + 1.0f) / 2.0f;

        colors[12] = (sin(time * 0.8) + 1.0f) / 2.0f;
        colors[13] = (cos(time * 0.7) + 1.0f) / 2.0f;
        colors[14] = (sin(time * 0.5) + 1.0f) / 2.0f;

        colors[15] = (cos(time * 0.9) + 1.0f) / 2.0f;
        colors[16] = (sin(time * 0.6) + 1.0f) / 2.0f;
        colors[17] = (cos(time * 0.4) + 1.0f) / 2.0f;

        colors[18] = (cos(time * 1.8) + 1.0f) / 2.0f;
        colors[19] = (sin(time * 0.6) + 1.0f) / 2.0f;
        colors[20] = (cos(time * 1.2) + 1.0f) / 2.0f;

        colors[21] = (cos(time * 0.3) + 1.0f) / 2.0f;
        colors[22] = (sin(time * 0.5) + 1.0f) / 2.0f;
        colors[23] = (cos(time * 1.2) + 1.0f) / 2.0f;

        for (int i = 0; i < 8; i++) {
            vertices[6 * i + 3] = colors[3 * i + 0];
            vertices[6 * i + 4] = colors[3 * i + 1];
            vertices[6 * i + 5] = colors[3 * i + 2];
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // 渲染
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 交换缓冲并处理事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理和终止
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}
