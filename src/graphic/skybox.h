#pragma once

#include "gl/common.h"
#include "camera.h"

namespace yc::graphic {

class SkyBox {

public:

    SkyBox();
    void init();
    void render(yc::Camera* camera);

private:

    static const float Vertices[];

    GLuint vao, vbo;
    GLuint texture;
};

}