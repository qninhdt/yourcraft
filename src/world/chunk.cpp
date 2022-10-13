#include <array>
#include "world/chunk.h"
#include "graphic/block_vertex.h"
#include <iostream>
namespace yc::world {

const std::array<glm::ivec3, 6> directionsToCheck = {{
    { 0, 0, +1 }, // front
    { 0, 0, -1 }, // back
    { +1, 0, 0 }, // right
    { -1, 0, 0 }, // left
    { 0, +1, 0 }, // top
    { 0, -1, 0 }, // bottom
}};

Chunk::Chunk(const glm::ivec3& coordinate): coordinate(coordinate) {
    // set all blocks in chunk to air (default block type)
    std::fill(&this->blocks[0][0][0], &this->blocks[0][0][0] + Chunk::Volume, BlockData::Default);
}

void Chunk::loadBlock() {
    for (int x=0; x<Chunk::Length; ++x) {
        for (int z=0; z<Chunk::Width; ++z) {
            int height = sqrt(x*x+z*z);
            for (int y=0;y<height;++y) {
                this->blocks[x][y][z].setType(BlockType::DIRT);
            }
        }
    }
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
                
                for (auto& d: directionsToCheck) {
                    const glm::ivec3 coordToCheck { x+d[0], y+d[1], z+d[2] };

                    if (coordToCheck.x>=0 && coordToCheck.x<Chunk::Length 
                        && coordToCheck.y>=0 && coordToCheck.y<Chunk::Height 
                        && coordToCheck.z>=0 && coordToCheck.z<Chunk::Width) {
                        if (this->blocks[coordToCheck.x][coordToCheck.y][coordToCheck.z].getType() != BlockType::AIR) {
                            continue;
                        }
                    }

                    auto vertices = yc::graphic::BlockVertex::GetVerticesFromDirection(d);
                    
                    const uint32_t id = chunkVertices.size();

                    for (auto& vertex: vertices) {
                        vertex.moveCoordinate(x, y, z);
                        vertex.setBlockType(block.getType(), d);

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
}

void Chunk::render() {
    this->mesh.bind();
    this->mesh.draw();
    this->mesh.unbind();
}

}