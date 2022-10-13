#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "gl/common.h"

namespace yc::gl {

class Shader {

public:
    Shader();
    void loadFromFile(const std::string& vertexShaderPath, 
        const std::string& fragmentShaderPath);
    void use();

    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, glm::vec2 value);
    void setVec3(const std::string& name, glm::vec3 value);
    void setMat4(const std::string& name, glm::mat4 value);
    void setUArray(const std::string& name, const std::vector<uint32_t>& value);

private:
    GLuint id;

    void checkCompileErrors(GLuint shader);
    GLuint getUniform(const std::string& name);
};

}