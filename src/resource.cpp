#include "resource.h"

namespace yc {

yc::gl::Shader Resource::ChunkShader;
yc::gl::Shader Resource::SkyBoxShader;
yc::gl::Shader Resource::BlockOutlineShader;
yc::gl::Shader Resource::CrossHairShader;
yc::gl::Texture Resource::GameTexure;
yc::gl::Texture Resource::CrossHairTexture;

void Resource::Load() {
    Resource::ChunkShader.loadFromFile("chunk.vert", "chunk.frag");
    Resource::SkyBoxShader.loadFromFile("skybox.vert", "skybox.frag");
    Resource::BlockOutlineShader.loadFromFile("block_outline.vert", "block_outline.frag");
    Resource::CrossHairShader.loadFromFile("crosshair.vert", "crosshair.frag");
    Resource::GameTexure.loadFromFile("D:/github/yourcraft/resources/texture.png");
    Resource::CrossHairTexture.loadFromFile("D:/github/yourcraft/resources/crosshair.png");
}

}