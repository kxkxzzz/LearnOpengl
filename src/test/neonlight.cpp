#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
out vec3 fragNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    fragNormal = mat3(transpose(inverse(model))) * aNormal;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 fragNormal;
out vec4 FragColor;
uniform vec3 frontColor;
uniform vec3 backColor;
uniform vec3 rightColor;
uniform vec3 leftColor;
uniform vec3 topColor;
uniform vec3 bottomColor;
void main() {
    // 计算光照，这里使用法线向量来确定颜色
    vec3 color;

    if (fragNormal.z > 0.9) { // front
        color = frontColor;
    } else if (fragNormal.z < -0.9) { // back
        color = backColor;
    } else if (fragNormal.x > 0.9) { // right
        color = rightColor;
    } else if (fragNormal.x < -0.9) { // left
        color = leftColor;
    } else if (fragNormal.y > 0.9) { // top
        color = topColor;
    } else { // bottom
        color = bottomColor;
    }

    FragColor = vec4(color, 1.0);
}
)";

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 创建GLFW窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "Colored Cube", nullptr, nullptr);
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

    // 检查顶点着色器编译错误...

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 检查片段着色器编译错误...

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 检查程序链接错误...

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 定义顶点数据，包括顶点坐标和法线向量
    float vertices[] = {
        // 前面
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  //
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  //
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  //
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  //
        // 后面
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  //
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  //
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  //
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  //
        // 右面
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  //
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  //
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  //
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  //
        // 左面
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  //
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  //
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  //
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  //
        // 顶面
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  //
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  //
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  //
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  //
        // 底面
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  //
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  //
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,  //
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f  //
    };

    unsigned int indices[] = {
        0,  1,  2,  2,  3,  0,  //
        4,  5,  6,  6,  7,  4,  //
        8,  9,  10, 10, 11, 8,  //
        12, 13, 14, 14, 15, 12,  //
        16, 17, 18, 18, 19, 16,  //
        20, 21, 22, 22, 23, 20  //
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

    // 获取 uniform 变量的位置
    GLint frontColorLoc = glGetUniformLocation(shaderProgram, "frontColor");
    GLint backColorLoc = glGetUniformLocation(shaderProgram, "backColor");
    GLint rightColorLoc = glGetUniformLocation(shaderProgram, "rightColor");
    GLint leftColorLoc = glGetUniformLocation(shaderProgram, "leftColor");
    GLint topColorLoc = glGetUniformLocation(shaderProgram, "topColor");
    GLint bottomColorLoc = glGetUniformLocation(shaderProgram, "bottomColor");

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
                           glm::value_ptr(projection));

        // 设置视图矩阵
        glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 1.5f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));

        // 获取当前时间
        float time = static_cast<float>(glfwGetTime());

        // 计算颜色
        glUniform3f(frontColorLoc, (sin(time) + 1.0f) / 2.0f, (cos(time) + 1.0f) / 2.0f,
                    (sin(time * 0.5) + 1.0f) / 2.0f);
        glUniform3f(backColorLoc, (cos(time * 0.7) + 1.0f) / 2.0f, (sin(time * 1.3) + 1.0f) / 2.0f,
                    (cos(time * 0.9) + 1.0f) / 2.0f);
        glUniform3f(rightColorLoc, (sin(time * 0.2) + 1.0f) / 2.0f, (cos(time * 0.4) + 1.0f) / 2.0f,
                    (sin(time * 0.6) + 1.0f) / 2.0f);
        glUniform3f(leftColorLoc, (cos(time * 1.2) + 1.0f) / 2.0f, (sin(time * 1.4) + 1.0f) / 2.0f,
                    (cos(time * 1.6) + 1.0f) / 2.0f);
        glUniform3f(topColorLoc, (sin(time * 0.8) + 1.0f) / 2.0f, (cos(time * 0.7) + 1.0f) / 2.0f,
                    (sin(time * 0.5) + 1.0f) / 2.0f);
        glUniform3f(bottomColorLoc, (cos(time * 0.9) + 1.0f) / 2.0f,
                    (sin(time * 0.6) + 1.0f) / 2.0f, (cos(time * 0.4) + 1.0f) / 2.0f);

        // 渲染
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 交换缓冲并处理事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 释放缓存
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}
