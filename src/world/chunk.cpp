#include <array>
#include "world/chunk.h"
#include "world/world.h"
#include "graphic/block_vertex.h"
#include <iostream>
#include <random>

namespace yc::world {

uint32_t chunkVertices[300000];
uint32_t chunkIndices[500000];

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
    this->needToBuildMesh = true;
    this->firstMeshBuild = true;
}

BlockData* Chunk::getChunkData() {
    return &this->blocks[0][0][0];
}

void Chunk::prepareToBuildMesh() {
    this->needToBuildMesh = true;
}

void Chunk::setCoordinate(const std::shared_ptr<World>& world, const glm::ivec2& coord) {
    this->world = world;
    this->coord = coord;
}

glm::ivec3 Chunk::getWorldCoord() const {
    return { this->coord.x<<4, 0, this->coord.y<<4 };
}

void Chunk::buildMeshIfNeeded() {
    if (this->needToBuildMesh) {
        buildMesh();
    }
}

// TODO: optimize this later
void Chunk::buildMesh() {
    size_t chunkVerticesSize = 0, chunkIndicesSize = 0;

    auto northChunk = this->world->getChunkIfLoadedAt({ this->coord.x, this->coord.y - 1 });
    auto southChunk = this->world->getChunkIfLoadedAt({ this->coord.x, this->coord.y + 1 });
    auto eastChunk = this->world->getChunkIfLoadedAt({ this->coord.x + 1, this->coord.y });
    auto westChunk = this->world->getChunkIfLoadedAt({ this->coord.x - 1, this->coord.y });

    for (int32_t x=0; x<Chunk::Length; ++x) 
    for (int32_t y=0; y<Chunk::Height; ++y) 
    for (int32_t z=0; z<Chunk::Width; ++z) {
        const BlockData& block = this->blocks[x][y][z];
        const BlockType blockType = block.getType();

        if (blockType == BlockType::AIR) continue;
        
        for (auto& direction: directionsToCheck) {
            const glm::ivec3 coordToCheck { x+direction[0], y+direction[1], z+direction[2] };
            BlockData blockToCheck;

            if (coordToCheck.y>=0 && coordToCheck.y<Chunk::Height) {
                if (coordToCheck.x >= Chunk::Length) {
                    // block to check in east chunk
                    if (eastChunk == nullptr) continue;
                    blockToCheck = eastChunk->blocks[0][y][z];
                } else if (coordToCheck.x < 0) {
                    // block to check in west chunk
                    if (westChunk == nullptr) continue;
                    blockToCheck = westChunk->blocks[Chunk::Length-1][y][z];
                } else if (coordToCheck.z >= Chunk::Width) {
                    // block to check in south chunk
                    if (southChunk == nullptr) continue;
                    blockToCheck = southChunk->blocks[x][y][0];
                } else if (coordToCheck.z < 0) {
                    // block to check in north chunk
                    if (northChunk == nullptr) continue;
                    blockToCheck = northChunk->blocks[x][y][Chunk::Width-1];
                } else {
                    // block to check in this chunk
                    blockToCheck = this->blocks[coordToCheck.x][coordToCheck.y][coordToCheck.z];
                }

                if (blockToCheck.getType() != BlockType::AIR) continue;
            }

            const uint32_t id = chunkVerticesSize;
            auto vertices = yc::graphic::BlockVertex::GetVerticesFromDirection(direction);                    

            for (auto& vertex: vertices) {
                vertex.moveCoordinate(x, y, z);
                vertex.setBlockType(blockType, direction);

                chunkVertices[chunkVerticesSize++] = vertex.getData();
            }

            chunkIndices[chunkIndicesSize++] = id + 0;
            chunkIndices[chunkIndicesSize++] = id + 1;
            chunkIndices[chunkIndicesSize++] = id + 2;
            chunkIndices[chunkIndicesSize++] = id + 0;
            chunkIndices[chunkIndicesSize++] = id + 2;
            chunkIndices[chunkIndicesSize++] = id + 3;
        }
    }

    if (this->firstMeshBuild) {
        this->mesh.init();
        this->mesh.bind();
        this->mesh.addIndices(&chunkIndices[0], chunkIndicesSize);
        this->mesh.addStaticBuffer(1, &chunkVertices[0], chunkVerticesSize);
        this->mesh.unbind();
        
        this->firstMeshBuild = false;
    }  else {
        this->mesh.bind();
        this->mesh.updateIndices(&chunkIndices[0], chunkIndicesSize);
        this->mesh.updateStaticBuffer(0, &chunkVertices[0], chunkVerticesSize);
        this->mesh.unbind();
    }

    this->needToBuildMesh = false;
}

glm::ivec2 Chunk::getCoord() const {
    return this->coord;
}

glm::ivec3 Chunk::getWorldCoordOfBlock(const glm::ivec3& blockCoord) {
    assert(blockCoord.x>=0 && blockCoord.x<Chunk::Length);
    assert(blockCoord.y>=0 && blockCoord.y<Chunk::Height);
    assert(blockCoord.z>=0 && blockCoord.z<Chunk::Width);

    return {
        blockCoord.x + (this->coord.x << 4),
        blockCoord.y,
        blockCoord.z + (this->coord.y << 4)
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
}

int32_t Chunk::DistanceTo(const glm::ivec2& chunkCoord, const glm::ivec2& coord) {
    
    return sqrt(
        (chunkCoord.x-coord.x)
        *(chunkCoord.x-coord.x)
        + (chunkCoord.y-coord.y)
        *(chunkCoord.y-coord.y));
}

}