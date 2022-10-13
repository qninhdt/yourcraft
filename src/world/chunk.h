#pragma once

#include "world/block.h"
#include "gl/mesh.h"

namespace yc::world {

class World;

class Chunk {

public:

    static const int32_t Width = 16;
    static const int32_t Length = 16;
    static const int32_t Height = 256;
    static const int32_t Volume = Width * Length * Height;

    Chunk();

    void setCoordinate(const std::shared_ptr<World>& world, const glm::ivec2& coord);

    void loadBlock();

    void buildMesh();

    void rebuildMesh();

    void render();

    BlockData getBlockDataAt(const glm::ivec3& coord);

    glm::ivec3 getWorldCoordOfBlock(const glm::ivec3& blockCoord);

private:

    BlockData blocks[Length][Height][Width]; // 0.5Mb wtf?
    yc::gl::Mesh mesh;

    glm::ivec2 coord;
    std::shared_ptr<World> world;
};

}