#pragma once

#include "world/block.h"
#include "gl/mesh.h"

namespace yc::world {

class Chunk {

public:

    static const size_t Width = 16;
    static const size_t Length = 16;
    static const size_t Height = 256;
    static const size_t Volume = Width * Length * Height;

    Chunk(const glm::ivec3& coordinate);

    void loadBlock();

    void buildMesh();

    void rebuildMesh();

    void render();

private:

    BlockData blocks[Length][Height][Width]; // 0.5Mb wtf?
    yc::gl::Mesh mesh;

    glm::ivec2 coordinate;

};

}