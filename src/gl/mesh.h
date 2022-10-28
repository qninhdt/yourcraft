#pragma once

#include <vector>
#include "gl/common.h"

namespace yc::gl {

class Mesh {

public:

    Mesh();
    ~Mesh();
    void init();
    void bind();
    void unbind();
    void draw();
    void drawLine();
    void addIndices(const std::vector<uint32_t>& indices);
    void addIndices(const uint32_t* indices, size_t indicesSize);
    void updateIndices(const std::vector<uint32_t>& indices);
    void updateIndices(const uint32_t* indices, size_t indicesSize);
    void updateStaticBuffer(size_t index, const std::vector<float>& data);
    void updateStaticBuffer(size_t index, const float* data, size_t dataSize);
    void addStaticBuffer(size_t size, const float* data, size_t dataSize);
    void addStaticBuffer(size_t size, const std::vector<float>& data);
    void updateStaticBuffer(size_t index, const std::vector<uint32_t>& data);
    void updateStaticBuffer(size_t index, const uint32_t* data, size_t dataSize);
    void addStaticBuffer(size_t size, const uint32_t* data, size_t dataSize);
    void addStaticBuffer(size_t size, const std::vector<uint32_t>& data);

private:
    GLuint vao;
    GLuint ebo;
    size_t indicesCount;
    std::vector<GLuint> buffers;
    std::vector<size_t> bufferSizes;

    template<typename T>
    void addBuffer(size_t size, T* data, size_t dataSize, GLuint dataType, GLuint drawType);

    template<typename T>
    void updateBuffer(size_t index, T* data, size_t dataSize, GLuint dataType, GLuint drawType);
};

}