#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "gl/shader.h"
#include "util/file.h"

namespace yc::gl {

const std::string shaderFolder = "./resources/shaders/";

Shader::Shader() {

}

void Shader::use() {
    glUseProgram(this->id);
}

void Shader::loadFromFile(const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath) {
    
    std::string vertexShaderSrc = yc::util::readTextFromFile(shaderFolder + vertexShaderPath);
    std::string fragmentShaderSrc = yc::util::readTextFromFile(shaderFolder + fragmentShaderPath);

    const char* vertexShaderSrcC = vertexShaderSrc.c_str();
    const char* fragmentShaderSrcC = fragmentShaderSrc.c_str();
    
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSrcC, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrcC, NULL);

    GL_CHECK(glCompileShader(vertexShader));
    GL_CHECK(glCompileShader(fragmentShader));

    checkCompileErrors(vertexShader);
    checkCompileErrors(fragmentShader);

    this->id = glCreateProgram();
    glAttachShader(this->id, vertexShader);
    glAttachShader(this->id, fragmentShader);
    glLinkProgram(this->id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cout << "Loaded shaders " << vertexShaderPath << " and " << fragmentShaderPath << '\n';
}

void Shader::checkCompileErrors(unsigned int shader) {
    int success;
    char infoLog[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "OpenGL shader error: " << "\n" << infoLog << "\n";
    }
}

GLuint Shader::getUniform(const std::string& name) {
    return glGetUniformLocation(this->id, name.c_str());
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(getUniform(name), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(getUniform(name), value);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) {
    glUniform2f(getUniform(name), value.x, value.y);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) {
    glUniform3f(getUniform(name), value.x, value.y, value.z);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) {
    glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUArray(const std::string& name, const std::vector<uint32_t>& value) {
    glUniform1uiv(getUniform(name), value.size(), value.data());
}

}
