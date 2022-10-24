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

Chunk::Chunk():
    needToBuildMesh(false),
    opaqueMesh(nullptr),
    transparentMesh(nullptr) {
}

BlockData* Chunk::getChunkData() {
    return &this->blocks[0][0][0];
}

void Chunk::prepareToBuildMesh() {
    this->needToBuildMesh = true;
}

void Chunk::setCoordinate(World* world, const glm::ivec2& coord) {
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
    size_t chunkOpaqueVerticesSize = 0;
    size_t chunkOpaqueIndicesSize = 0;
    size_t chunkTransparentVerticesSize = 0;
    size_t chunkTransparentIndicesSize = 0;

    static uint32_t chunkOpaqueVertices[300000];
    static uint32_t chunkOpaqueIndices[500000];
    static uint32_t chunkTransparentVertices[300000];
    static uint32_t chunkTransparentIndices[500000];

    auto northChunk = this->world->getChunkIfLoadedAt({ this->coord.x, this->coord.y - 1 });
    auto southChunk = this->world->getChunkIfLoadedAt({ this->coord.x, this->coord.y + 1 });
    auto eastChunk = this->world->getChunkIfLoadedAt({ this->coord.x + 1, this->coord.y });
    auto westChunk = this->world->getChunkIfLoadedAt({ this->coord.x - 1, this->coord.y });

    for (int32_t x=0; x<Chunk::Length; ++x) 
    for (int32_t y=0; y<Chunk::Height; ++y) 
    for (int32_t z=0; z<Chunk::Width; ++z) {
        BlockData& block = this->blocks[x][y][z];
        BlockType blockType = block.getType();

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

                if (blockType == blockToCheck.getType() && blockToCheck.isTransparent()) {
                    continue;
                }

                if (blockToCheck.isOpaque() && blockType!=BlockType::GLASS) continue;
            }

            const uint32_t opaqueId = chunkOpaqueVerticesSize;
            const uint32_t transparentId = chunkTransparentVerticesSize;
            auto vertices = yc::graphic::BlockVertex::GetVerticesFromDirection(direction);                    

            for (auto& vertex: vertices) {
                vertex.moveCoordinate(x, y, z);
                vertex.setBlockType(blockType, direction);

                if (block.isOpaque()) {
                    chunkOpaqueVertices[chunkOpaqueVerticesSize++] = vertex.getData();
                } else {
                    chunkTransparentVertices[chunkTransparentVerticesSize++] = vertex.getData();
                }
            }

            if (block.isOpaque()) {
                chunkOpaqueIndices[chunkOpaqueIndicesSize++] = opaqueId + 0;
                chunkOpaqueIndices[chunkOpaqueIndicesSize++] = opaqueId + 1;
                chunkOpaqueIndices[chunkOpaqueIndicesSize++] = opaqueId + 2;
                chunkOpaqueIndices[chunkOpaqueIndicesSize++] = opaqueId + 0;
                chunkOpaqueIndices[chunkOpaqueIndicesSize++] = opaqueId + 2;
                chunkOpaqueIndices[chunkOpaqueIndicesSize++] = opaqueId + 3;
            } else {
                chunkTransparentIndices[chunkTransparentIndicesSize++] = transparentId + 0;
                chunkTransparentIndices[chunkTransparentIndicesSize++] = transparentId + 1;
                chunkTransparentIndices[chunkTransparentIndicesSize++] = transparentId + 2;
                chunkTransparentIndices[chunkTransparentIndicesSize++] = transparentId + 0;
                chunkTransparentIndices[chunkTransparentIndicesSize++] = transparentId + 2;
                chunkTransparentIndices[chunkTransparentIndicesSize++] = transparentId + 3;
            }
        }
    }

    if (chunkOpaqueIndices != 0) {
        if (opaqueMesh == nullptr) {
            opaqueMesh = std::make_shared<yc::gl::Mesh>();
            opaqueMesh->init();
            opaqueMesh->bind();
            opaqueMesh->addIndices(&chunkOpaqueIndices[0], chunkOpaqueIndicesSize);
            opaqueMesh->addStaticBuffer(1, &chunkOpaqueVertices[0], chunkOpaqueVerticesSize);
        }  else {
            opaqueMesh->bind();
            opaqueMesh->updateIndices(&chunkOpaqueIndices[0], chunkOpaqueIndicesSize);
            opaqueMesh->updateStaticBuffer(0, &chunkOpaqueVertices[0], chunkOpaqueVerticesSize);
        }
    }

    if (chunkTransparentIndices != 0) {
        if (transparentMesh == nullptr) {
            transparentMesh = std::make_shared<yc::gl::Mesh>();
            transparentMesh->init();
            transparentMesh->bind();
            transparentMesh->addIndices(&chunkOpaqueIndices[0], chunkOpaqueIndicesSize);
            transparentMesh->addStaticBuffer(1, &chunkOpaqueVertices[0], chunkOpaqueVerticesSize);
        }  else {
            transparentMesh->bind();
            transparentMesh->updateIndices(&chunkTransparentIndices[0], chunkTransparentIndicesSize);
            transparentMesh->updateStaticBuffer(0, &chunkTransparentVertices[0], chunkTransparentVerticesSize);
        }
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

void Chunk::renderOpaque() {
    if (opaqueMesh == nullptr) return;
    opaqueMesh->bind();
    opaqueMesh->draw();
}

void Chunk::renderTransparent() {
    if (transparentMesh == nullptr) return;
    transparentMesh->bind();
    transparentMesh->draw();
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