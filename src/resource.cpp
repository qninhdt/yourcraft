#include "resource.h"

namespace yc {

yc::gl::Shader Resource::ChunkShader;
yc::gl::Texture Resource::GameTexure;

void Resource::Load() {

    Resource::ChunkShader.loadFromFile("chunk.vert", "chunk.frag");
    Resource::GameTexure.loadFromFile("resources/texture.png");
}

}