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
    built = false;
}

void Chunk::loadBlock() {
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> distr(3, 20);

    for (int x=0; x<Chunk::Length; ++x) {
        for (int z=0; z<Chunk::Width; ++z) {
            int height = abs(sqrt(abs(x-8)+abs(z-8))*30);
            // int height = 1;
            for (int y=0;y<height;++y) {
                this->blocks[x][y][z].setType(BlockType::DIRT);
            }
        }
    }
}

void Chunk::setCoordinate(const std::shared_ptr<World>& world, const glm::ivec2& coord) {
    this->world = world;
    this->coord = coord;
}

void Chunk::buildMesh() {
    std::vector<uint32_t> chunkVertices;
    std::vector<uint32_t> chunkIndices;

    for (int x=0; x<Chunk::Length; ++x) {
        for (int y=0; y<Chunk::Height; ++y) {
            for (int z=0; z<Chunk::Width; ++z) {
                auto& block = this->blocks[x][y][z];
                if (this->blocks[x][y][z].getType() == BlockType::AIR) {
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

    this->mesh.init();
    this->mesh.bind();

    this->mesh.addIndices(chunkIndices);
    this->mesh.addStaticBuffer(1, chunkVertices);

    this->mesh.unbind();

    this->built = true;
}

glm::ivec3 Chunk::getWorldCoordOfBlock(const glm::ivec3& blockCoord) {
    assert(blockCoord.x>=0 && blockCoord.x<Chunk::Length);
    assert(blockCoord.y>=0 && blockCoord.y<Chunk::Height);
    assert(blockCoord.z>=0 && blockCoord.z<Chunk::Width);

    return { this->coord.x * Chunk::Length + blockCoord.x, blockCoord.y, this->coord.y * Chunk::Width + blockCoord.z };
}

BlockData Chunk::getBlockDataAt(const glm::ivec3& coord) {
    assert(coord.x>=0 && coord.x<Chunk::Length);
    assert(coord.y>=0 && coord.y<Chunk::Height);
    assert(coord.z>=0 && coord.z<Chunk::Width);

    return this->blocks[coord.x][coord.y][coord.z];
}

void Chunk::render() {
    if (!this->built) return;
    this->mesh.bind();
    this->mesh.draw();
    this->mesh.unbind();
}

int32_t Chunk::DistanceTo(const glm::ivec2& chunkCoord, const glm::ivec2& coord) {
    return sqrt((chunkCoord.x-coord.x)*(chunkCoord.x-coord.x)
        + (chunkCoord.y-coord.y)*(chunkCoord.y-coord.y));
}

}