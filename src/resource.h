#pragma once

#include "gl/shader.h"
#include "gl/texture.h"

namespace yc {

class Resource {

public:

    static yc::gl::Shader ChunkShader;
    static yc::gl::Shader SkyBoxShader;
    static yc::gl::Shader BlockOutlineShader;
    static yc::gl::Shader CrossHairShader;
    static yc::gl::Texture GameTexure;
    static yc::gl::Texture CrossHairTexture;

    static void Load();

};

}