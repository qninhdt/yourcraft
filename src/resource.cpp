#include "resource.h"

namespace yc {

yc::gl::Shader Resource::ChunkShader;
yc::gl::Shader Resource::SkyBoxShader;
yc::gl::Texture Resource::GameTexure;

void Resource::Load() {
    Resource::ChunkShader.loadFromFile("chunk.vert", "chunk.frag");
    Resource::SkyBoxShader.loadFromFile("skybox.vert", "skybox.frag");
    Resource::GameTexure.loadFromFile("D:/github/yourcraft/resources/texture.png");
}

}