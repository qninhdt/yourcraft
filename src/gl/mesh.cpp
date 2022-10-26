#include "gl/mesh.h"
#include <iostream>
namespace yc::gl {

Mesh::Mesh() {}

void Mesh::init() {
    glGenVertexArrays(1, &this->vao);
}

void Mesh::bind() {
    glBindVertexArray(this->vao);
}

void Mesh::unbind() {
    glBindVertexArray(0);
}

void Mesh::draw() {
    if (indicesCount>0) {
        glDrawElements(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0);
    }
}

void Mesh::drawLine() {
    if (indicesCount>0) {
        glDrawElements(GL_LINES, this->indicesCount, GL_UNSIGNED_INT, 0);
    }
}

void Mesh::addIndices(const std::vector<uint32_t>& indices) {
    addIndices(indices.data(), indices.size());
}

void Mesh::addIndices(const uint32_t* indices, size_t indicesSize) {
    this->indicesCount = indicesSize;

    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicesCount * sizeof(uint32_t),
        indices, GL_STATIC_DRAW);
}

void Mesh::updateIndices(const uint32_t* indices, size_t indicesSize) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    if (indicesSize > this->indicesCount) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(uint32_t),
            indices, GL_STATIC_DRAW);
    } else {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicesSize * sizeof(uint32_t),
            indices);        
    }
    this->indicesCount = indicesSize;
}

void Mesh::updateIndices(const std::vector<uint32_t>& indices) {
    updateIndices(indices.data(), indices.size());
}

template<typename T>
void Mesh::addBuffer(size_t size, T* data, size_t dataSize, GLuint dataType, GLuint drawType) {
    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(T), data, drawType);

    size_t bufferId = this->buffers.size();
    glEnableVertexAttribArray(bufferId);

    switch (dataType) {
        case GL_UNSIGNED_INT:
        case GL_INT:
            glVertexAttribIPointer(bufferId, size, dataType, 0, (void*) 0);
            break;  
        case GL_FLOAT:
            glVertexAttribPointer(bufferId, size, dataType, GL_FALSE, 0, (void*) 0);
            break;
    }

    this->buffers.push_back(vbo);
    this->bufferSizes.push_back(dataSize);
}

template<typename T>
void Mesh::updateBuffer(size_t index, T* data, size_t dataSize, GLuint dataType, GLuint drawType) {
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[index]);

    if (dataSize > this->bufferSizes[index]) {
        glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(T), data, drawType);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize * sizeof(T), data);
    }

    this->bufferSizes[index] = dataSize;
}

void Mesh::addStaticBuffer(size_t size, const float* data, size_t dataSize) {
    addBuffer(size, data, dataSize, GL_FLOAT, GL_STATIC_DRAW);
}

void Mesh::addStaticBuffer(size_t size, const std::vector<float>& data) {
    addStaticBuffer(size, data.data(), data.size());
}

void Mesh::updateStaticBuffer(size_t index, const float* data, size_t dataSize){
    updateBuffer(index, data, dataSize, GL_FLOAT, GL_STATIC_DRAW);
}

void Mesh::updateStaticBuffer(size_t index, const std::vector<float>& data){
    updateStaticBuffer(index, data.data(), data.size());
}

void Mesh::addStaticBuffer(size_t size, const uint32_t* data, size_t dataSize) {
    addBuffer(size, data, dataSize, GL_UNSIGNED_INT, GL_STATIC_DRAW);
}

void Mesh::addStaticBuffer(size_t size, const std::vector<uint32_t>& data) {
    addStaticBuffer(size, data.data(), data.size());
}

void Mesh::updateStaticBuffer(size_t index, const uint32_t* data, size_t dataSize){
    updateBuffer(index, data, dataSize, GL_UNSIGNED_INT, GL_STATIC_DRAW);
}

void Mesh::updateStaticBuffer(size_t index, const std::vector<uint32_t>& data){
    updateStaticBuffer(index, data.data(), data.size());
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &this->vao);
    for (GLuint vbo: this->buffers) {
        glDeleteBuffers(1, &vbo);
    }
    glDeleteBuffers(1, &this->ebo);
}

}