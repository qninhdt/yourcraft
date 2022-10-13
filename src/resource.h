#pragma once

#include "gl/shader.h"
#include "gl/texture.h"

namespace yc {

class Resource {

public:

    static yc::gl::Shader ChunkShader;
    static yc::gl::Texture GameTexure;

    static void Load();

};

}