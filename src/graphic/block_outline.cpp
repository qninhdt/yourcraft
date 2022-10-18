#include "graphic/block_outline.h"
#include "resource.h"
#include "glm/gtc/matrix_transform.hpp"

namespace yc::graphic {

const float delta = 0.002f;

const float BlockOutline::Vertices[] = {
    1-delta, 0+delta, 1+delta, 
    1-delta, 1-delta, 1+delta, 
    0+delta, 1-delta, 1+delta, 
    0+delta, 0+delta, 1+delta, 
    1-delta, 0+delta, 0-delta, 
    1-delta, 1-delta, 0-delta, 
    0+delta, 1-delta, 0-delta, 
    0+delta, 0+delta, 0-delta, 
    1+delta, 0+delta, 0+delta, 
    1+delta, 1-delta, 0+delta, 
    1+delta, 1-delta, 1-delta, 
    1+delta, 0+delta, 1-delta,
    0-delta, 0+delta, 0+delta, 
    0-delta, 1-delta, 0+delta, 
    0-delta, 1-delta, 1-delta, 
    0-delta, 0+delta, 1-delta,
    1-delta, 1+delta, 0+delta, 
    1-delta, 1+delta, 1-delta, 
    0+delta, 1+delta, 1-delta, 
    0+delta, 1+delta, 0+delta, 
    1-delta, 0-delta, 1-delta, 
    1-delta, 0-delta, 0+delta, 
    0+delta, 0-delta, 0+delta, 
    0+delta, 0-delta, 1,
};

const uint32_t BlockOutline::Indices[] = {
    0, 1, 1, 2, 2, 3, 3, 0,
    4, 5, 5, 6, 6, 7, 7, 4,
    8, 9, 9, 10, 10, 11, 11, 8,
    12, 13, 13, 14, 14, 15, 15, 12,
    16, 17, 17, 18, 18, 19, 19, 16,
    20, 21, 21, 22, 22, 23, 23, 20
};

BlockOutline::BlockOutline() {}

void BlockOutline::init() {
    this->mesh.init();
    this->mesh.bind();
    this->mesh.addIndices(&Indices[0], sizeof(Indices)/sizeof(float));
    this->mesh.addStaticBuffer(3, &Vertices[0], sizeof(Vertices)/sizeof(float));
    this->mesh.unbind();
}

void BlockOutline::render(const yc::Camera& camera, const glm::ivec3& coord) {
    yc::Resource::BlockOutlineShader.use();
    yc::Resource::BlockOutlineShader.setMat4("projection_view", camera.getProjectionViewMatrix());
    yc::Resource::BlockOutlineShader.setMat4("model",
        glm::translate(glm::mat4(1.0f), glm::vec3(coord))
    );
    this->mesh.bind();
    this->mesh.drawLine();
}

}