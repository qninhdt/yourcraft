#include "resource.h"

namespace yc {

yc::gl::Shader Resource::OpaqueShader;
yc::gl::Shader Resource::TransparentShader;
yc::gl::Shader Resource::SkyBoxShader;
yc::gl::Shader Resource::BlockOutlineShader;
yc::gl::Shader Resource::CrossHairShader;
yc::gl::Texture Resource::GameTexure;
yc::gl::Texture Resource::CrossHairTexture;

void Resource::Load() {
    Resource::OpaqueShader.loadFromFile("opaque.vert", "opaque.frag");
    Resource::TransparentShader.loadFromFile("transparent.vert", "transparent.frag");
    Resource::SkyBoxShader.loadFromFile("skybox.vert", "skybox.frag");
    Resource::BlockOutlineShader.loadFromFile("block_outline.vert", "block_outline.frag");
    Resource::CrossHairShader.loadFromFile("crosshair.vert", "crosshair.frag");
    Resource::GameTexure.loadFromFile("E:/github/yourcraft/resources/texture.png");
    Resource::CrossHairTexture.loadFromFile("E:/github/yourcraft/resources/crosshair.png");
}

}