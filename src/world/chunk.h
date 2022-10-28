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

    void setCoordinate(World* world, const glm::ivec2& coord);

    void buildMesh();

    void buildMeshIfNeeded();

    void prepareToBuildMesh();

    void renderOpaque();

    void renderTransparent();

    void renderFlora();

    static int32_t DistanceTo(const glm::ivec2& chunkCoord, const glm::ivec2& coord);

    BlockData getBlockDataAt(const glm::ivec3& coord);

    glm::ivec2 getCoord() const;

    glm::ivec3 getWorldCoordOfBlock(const glm::ivec3& blockCoord);

    glm::ivec3 getWorldCoord() const;

    BlockData* getChunkData();

    void setBlockData(const glm::ivec3& coord, BlockData blockData);

private:

    BlockData blocks[Length][Height][Width]; // 0.5Mb wtf?
    std::shared_ptr<yc::gl::Mesh> opaqueMesh;
    std::shared_ptr<yc::gl::Mesh> transparentMesh; 
    std::shared_ptr<yc::gl::Mesh> floraMesh;

    bool needToBuildMesh;

    glm::ivec2 coord;
    World* world;
    
};

}