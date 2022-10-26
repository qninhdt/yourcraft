#pragma once

#include "glm/glm.hpp"
#include "gl/mesh.h"
#include "camera.h"

namespace yc::graphic {

class BlockOutline {

public:

    static const float Vertices[];
    static const float Uv[];
    static const uint32_t Indices[];

    BlockOutline();

    void init();

    void render(yc::Camera* camera, const glm::ivec3& coord);

private:

    yc::gl::Mesh mesh;

};

}