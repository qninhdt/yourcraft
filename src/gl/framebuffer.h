#pragma once

#include "gl/mesh.h"
#include "gl/texture.h"
#include "gl/common.h"

namespace yc::gl {

class FrameBuffer {

public:

    static const float Vertices[];
    static const float TexCoords[];
    static const uint32_t Indices[];

    FrameBuffer(int32_t width, int32_t height);

    ~FrameBuffer();

    GLuint getId() const;

    void init();

    void bind();

    void bindTexture();

    void render();

private:

    Mesh mesh;
    int32_t width, height;
    GLuint id;
    GLuint texture;
    GLuint rbo;
};

}