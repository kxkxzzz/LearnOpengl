#ifndef SHADERTOY_IMPORT
#define SHADERTOY_IMPORT

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders.h"

class ShaderToyImporter {
public:
    ShaderToyImporter(const std::string& ShaderToySrc);
    ShaderToyImporter(const std::string& VertSrc, const std::string& FragSrc);
    ~ShaderToyImporter();
    void Draw();
    void init();

private:
    static const float vertices[];
    std::string VShaderSrc, FShaderSrc;
    GLuint VAO, VBO;
    Shader shader;
};

const float ShaderToyImporter::vertices[] = {
    // positions   // texCoords
    -1.0f, 1.0f,  0.0f, 1.0f,  //
    -1.0f, -1.0f, 0.0f, 0.0f,  //
    1.0f,  -1.0f, 1.0f, 0.0f,  //

    -1.0f, 1.0f,  0.0f, 1.0f,  //
    1.0f,  -1.0f, 1.0f, 0.0f,  //
    1.0f,  1.0f,  1.0f, 1.0f  //
};

ShaderToyImporter::ShaderToyImporter(const std::string& ShaderToySrc)
    : VShaderSrc("../ShaderToy/shader.vert"),
      FShaderSrc(ShaderToySrc),
      shader(VShaderSrc.c_str(), FShaderSrc.c_str()) {
    init();
}

ShaderToyImporter::ShaderToyImporter(const std::string& VertSrc, const std::string& FragSrc)
    : VShaderSrc(VertSrc), FShaderSrc(FragSrc), shader(VShaderSrc.c_str(), FShaderSrc.c_str()) {
    init();
}

ShaderToyImporter::~ShaderToyImporter() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ShaderToyImporter::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    shader.use();
    GLuint aPosLoc = glGetAttribLocation(shader.ID, "aPos");
    GLuint aTexCoordsLoc = glGetAttribLocation(shader.ID, "aTexCoords");
    glEnableVertexAttribArray(aPosLoc);
    glVertexAttribPointer(aPosLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aTexCoordsLoc);
    glVertexAttribPointer(aTexCoordsLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void*)(2 * sizeof(float)));
}

void ShaderToyImporter::Draw() {
    shader.use();
    shader.setVec3("iResolution", glm::vec3(800, 600, 0));
    shader.setFloat("iTime", (float)glfwGetTime());
    shader.setVec3("iMouse", glm::vec3(0, 0, 0));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

#endif