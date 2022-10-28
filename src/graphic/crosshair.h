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

    void init();

    void update();

    void render();

private:
    yc::gl::Mesh mesh;
    std::vector<float> vertices;
};

}