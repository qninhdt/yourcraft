#include <array>
#include "world/chunk.h"
#include "world/world.h"
#include "graphic/block_vertex.h"
#include <iostream>
#include <random>

namespace yc::world {

const std::array<glm::ivec3, 6> directionsToCheck = {{
    { 0, 0, +1 }, // front
    { 0, 0, -1 }, // back
    { +1, 0, 0 }, // right
    { -1, 0, 0 }, // left
    { 0, +1, 0 }, // top
    { 0, -1, 0 }, // bottom
}};

Chunk::Chunk() {
    // set all blocks in chunk to air (default block type)
    std::fill(&this->blocks[0][0][0], &this->blocks[0][0][0] + Chunk::Volume, BlockData::Default);
    this->needToBuildMesh = true;
    this->firstMeshBuild = true;
}

void Chunk::prepareToBuildMesh() {
    this->needToBuildMesh = true;
}

void Chunk::setCoordinate(const std::shared_ptr<World>& world, const glm::ivec2& coord) {
    this->world = world;
    this->coord = coord;
}

glm::ivec3 Chunk::getWorldCoord() const {
    return { this->coord.x * Chunk::Length, 0, this->coord.y * Chunk::Width };
}

void Chunk::buildMeshIfNeeded() {
    if (this->needToBuildMesh) {
        buildMesh();
    }
}

void Chunk::buildMesh() {
    std::vector<uint32_t> chunkVertices;
    std::vector<uint32_t> chunkIndices;

    for (int x=0; x<Chunk::Length; ++x) {
        for (int y=0; y<Chunk::Height; ++y) {
            for (int z=0; z<Chunk::Width; ++z) {
                auto& block = this->blocks[x][y][z];
                if (block.getType() == BlockType::AIR) {
                    continue;
                }
                
                for (auto& direction: directionsToCheck) {
                    const glm::ivec3 coordToCheck { x+direction[0], y+direction[1], z+direction[2] };
                    BlockData blockToCheck;

                    if (coordToCheck.y>=0 && coordToCheck.y<Chunk::Height) {
                        if (coordToCheck.x>=0 && coordToCheck.x<Chunk::Length 
                            && coordToCheck.z>=0 && coordToCheck.z<Chunk::Width) {
                            blockToCheck = this->blocks[coordToCheck.x][coordToCheck.y][coordToCheck.z];
                        } else {
                            auto worldCoordToCheck = getWorldCoordOfBlock({ x, y, z }) + direction;
                            blockToCheck = this->world->getBlockDataIfLoadedAt(worldCoordToCheck);
                        }

                        if (blockToCheck.getType() != BlockType::AIR) {
                            continue;
                        }
                    }

                    auto vertices = yc::graphic::BlockVertex::GetVerticesFromDirection(direction);
                    
                    const uint32_t id = chunkVertices.size();

                    for (auto& vertex: vertices) {
                        vertex.moveCoordinate(x, y, z);
                        vertex.setBlockType(block.getType(), direction);

                        chunkVertices.push_back(vertex.getData());
                    }

                    chunkIndices.insert(chunkIndices.end(), {
                        id + 0, id + 1, id + 2,
                        id + 2, id + 3, id + 0
                    });
                }

            }
        }
    }

    if (this->firstMeshBuild) {
        this->mesh.init();
        this->mesh.bind();
        this->mesh.addIndices(chunkIndices);
        this->mesh.addStaticBuffer(1, chunkVertices);
        this->mesh.unbind();
        
        this->firstMeshBuild = false;
    }  else {
        this->mesh.bind();
        this->mesh.updateIndices(chunkIndices);
        this->mesh.updateStaticBuffer(0, chunkVertices);
        this->mesh.unbind();
    }

    this->needToBuildMesh = false;
}

glm::ivec3 Chunk::getWorldCoordOfBlock(const glm::ivec3& blockCoord) {
    assert(blockCoord.x>=0 && blockCoord.x<Chunk::Length);
    assert(blockCoord.y>=0 && blockCoord.y<Chunk::Height);
    assert(blockCoord.z>=0 && blockCoord.z<Chunk::Width);

    return {
        blockCoord.x + this->coord.x*Chunk::Length,
        blockCoord.y,
        blockCoord.z + this->coord.y*Chunk::Width
    };
}

BlockData Chunk::getBlockDataAt(const glm::ivec3& coord) {
    assert(coord.x>=0 && coord.x<Chunk::Length);
    assert(coord.y>=0 && coord.y<Chunk::Height);
    assert(coord.z>=0 && coord.z<Chunk::Width);

    return this->blocks[coord.x][coord.y][coord.z];
}

void Chunk::render() {
    if (this->firstMeshBuild) return;
    this->mesh.bind();
    this->mesh.draw();
    this->mesh.unbind();
}

void Chunk::setBlockData(const glm::ivec3& coord, BlockData blockData) {
    this->blocks[coord.x][coord.y][coord.z] = blockData;
    prepareToBuildMesh();
}

int32_t Chunk::DistanceTo(const glm::ivec2& chunkCoord, const glm::ivec2& coord) {
    return sqrt((chunkCoord.x-coord.x)*(chunkCoord.x-coord.x)
        + (chunkCoord.y-coord.y)*(chunkCoord.y-coord.y));
}

}