#include "resource.h"

namespace yc {

yc::gl::Shader Resource::OpaqueShader;
yc::gl::Shader Resource::TransparentShader;
yc::gl::Shader Resource::FloraShader;
yc::gl::Shader Resource::GrayScaleShader;
yc::gl::Shader Resource::SkyBoxShader;
yc::gl::Shader Resource::BlockOutlineShader;
yc::gl::Shader Resource::CrossHairShader;
yc::gl::Shader Resource::CompositeShader;
yc::gl::Texture Resource::GameTexure;
yc::gl::Texture Resource::CrossHairTexture;

void Resource::Load() {
    Resource::OpaqueShader.loadFromFile("opaque.vert", "opaque.frag");
    Resource::TransparentShader.loadFromFile("transparent.vert", "transparent.frag");
    Resource::FloraShader.loadFromFile("flora.vert", "flora.frag");
    Resource::GrayScaleShader.loadFromFile("grayscale.vert", "grayscale.frag");
    Resource::SkyBoxShader.loadFromFile("skybox.vert", "skybox.frag");
    Resource::BlockOutlineShader.loadFromFile("block_outline.vert", "block_outline.frag");
    Resource::CrossHairShader.loadFromFile("crosshair.vert", "crosshair.frag");
    Resource::CompositeShader.loadFromFile("composite.vert", "composite.frag");
    Resource::GameTexure.loadFromFile("./resources/texture.png");
    Resource::CrossHairTexture.loadFromFile("./resources/crosshair.png");
}

}