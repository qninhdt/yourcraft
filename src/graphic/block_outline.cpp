#include "graphic/block_outline.h"
#include "resource.h"
#include "glm/gtc/matrix_transform.hpp"

namespace yc::graphic {

const float BlockOutline::Vertices[] = {
    1, 0, 1,
    1, 1, 1,
    0, 1, 1,
    0, 0, 1,
    1, 0, 0,
    1, 1, 0,
    0, 1, 0,
    0, 0, 0,
    1, 0, 0,
    1, 1, 0,
    1, 1, 1,
    1, 0, 1,
    0, 0, 0,
    0, 1, 0,
    0, 1, 1,
    0, 0, 1,
    1, 1, 0,
    1, 1, 1,
    0, 1, 1,
    0, 1, 0,
    1, 0, 1,
    1, 0, 0,
    0, 0, 0,
    0, 0, 1,
};

const float BlockOutline::Uv[] = {
    1, 0,
    1, 1,
    0, 1,
    0, 0,
    0, 0,
    0, 1,
    1, 1,
    1, 0,
    1, 0,
    1, 1,
    0, 1,
    0, 0,
    0, 0,
    0, 1,
    1, 1,
    1, 0,
    1, 1,
    1, 0,
    0, 0,
    0, 1,
    0, 0,
    0, 1,
    1, 1,
    1, 0,
};

const uint32_t BlockOutline::Indices[] = {
    0, 1, 2,
    0, 2, 3,
    4, 5, 6,
    4, 6, 7,
    8, 9, 10,
    8, 10, 11,
    12, 13, 14,
    12, 14, 15,
    16, 17, 18,
    16, 18, 19,
    20, 21, 22,
    20, 22, 23
};

BlockOutline::BlockOutline() {}

void BlockOutline::init() {
    this->mesh.init();
    this->mesh.bind();
    this->mesh.addIndices(&Indices[0], sizeof(Indices)/sizeof(float));
    this->mesh.addStaticBuffer(3, &Vertices[0], sizeof(Vertices)/sizeof(float));
    this->mesh.addStaticBuffer(2, &Uv[0], sizeof(Uv)/sizeof(float));
    this->mesh.unbind();
}

void BlockOutline::render(yc::Camera* camera, const glm::ivec3& coord) {
    yc::Resource::BlockOutlineShader.use();
    yc::Resource::BlockOutlineShader.setMat4("projection_view", camera->getProjectionViewMatrix());
    yc::Resource::BlockOutlineShader.setMat4("model",
        glm::translate(glm::mat4(1.0f), glm::vec3(coord))
    );
    this->mesh.bind();
    this->mesh.draw();
}

}