#pragma once

#include <map>
#include "gl/shader.h"
#include "gl/texture.h"
#include "world/block.h"

namespace yc {

class Resource {

public:

    static yc::gl::Shader OpaqueShader;
    static yc::gl::Shader TransparentShader;
    static yc::gl::Shader FloraShader;
    static yc::gl::Shader GrayScaleShader;
    static yc::gl::Shader SkyBoxShader;
    static yc::gl::Shader BlockOutlineShader;
    static yc::gl::Shader CrossHairShader;
    static yc::gl::Shader CompositeShader;
    static yc::gl::Texture GameTexure;
    static yc::gl::Texture CrossHairTexture;
    static yc::gl::Texture IconTexture;
    static yc::gl::Texture ResumeButtonTexture;
    static yc::gl::Texture HoveredResumeButtonTexture;
    static yc::gl::Texture BackButtonTexture;
    static yc::gl::Texture HoveredBackButtonTexture;

    static std::map<yc::world::BlockType, yc::gl::Texture> BlockIcons;

    static void Load();

};

}