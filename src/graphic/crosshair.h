#pragma once

#include <vector>
#include "gl/mesh.h"
#include "resource.h"

namespace yc::graphic {

const float Vertices[] = {
    -1, -1,
    +1, -1,
    +1, +1,
    -1, +1,
};

const float Texcoords[] = {
    0, 0,
    1, 0,
    1, 1,
    0, 1,
};

const uint32_t Indices[] = {
    0, 1, 2,
    0, 2, 3,
};

class CrossHair {

public:

    const int32_t Size = 20;

    

    CrossHair() {}

    void init(int32_t screenWidth, int32_t screenHeight) {
        yc::Resource::CrossHairShader.use();
        yc::Resource::CrossHairShader.setInt("screen_width", screenWidth);
        yc::Resource::CrossHairShader.setInt("screen_height", screenHeight);
        yc::Resource::CrossHairShader.setInt("width", yc::Resource::CrossHairTexture.getWidth());
        yc::Resource::CrossHairShader.setInt("height", yc::Resource::CrossHairTexture.getHeight());
        this->mesh.init();
        this->mesh.bind();
        this->mesh.addIndices(Indices, 6);
        this->mesh.addStaticBuffer(2, &Vertices[0], 8);
        this->mesh.addStaticBuffer(2, &Texcoords[0], 8);
    }

    void render() {
        Resource::CrossHairShader.use();
        Resource::CrossHairTexture.bind();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        this->mesh.bind();
        this->mesh.draw();
        glDisable(GL_BLEND);
    }

private:
    yc::gl::Mesh mesh;
    std::vector<float> vertices;
};

}