#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "world/world.h"
#include "util/math.h"
#include "resource.h"

namespace yc::world {

World::World(): generator(240404) {}

void World::init() {

}

void World::update(const yc::Camera& camera) {
    glm::ivec2 cameraChunkCoord = {
        camera.getPosition().x/Chunk::Length,
        camera.getPosition().z/Chunk::Width,
    };

    const int32_t viewDistance = 16;

    for (auto& [chunkCoord, chunk]: this->chunks) {
        if (Chunk::DistanceTo(chunkCoord, cameraChunkCoord) > viewDistance) {
            this->unloadChunk(chunkCoord);
        }
    }

    int cnt = 0;
    const int32_t maxChunkLoadPerFrame = 3;
    for (int32_t x=-viewDistance; x<=viewDistance && cnt<maxChunkLoadPerFrame; ++x) {
        for (int32_t z=-viewDistance; z<=viewDistance && cnt<maxChunkLoadPerFrame; ++z) {
            glm::ivec2 chunkCoordToCheck = glm::ivec2(x, z) + cameraChunkCoord;

            if (Chunk::DistanceTo(chunkCoordToCheck, cameraChunkCoord) <= viewDistance) {
                if (!isChunkLoaded(chunkCoordToCheck)) {
                    generateOrLoadChunkAt(chunkCoordToCheck);
                    ++cnt;
                }
            }
        }
    }
    for (auto& [chunkCoord, chunk]: this->chunks) {
        chunk->buildMeshIfNeeded();
    }
}

void World::generateOrLoadChunkAt(const glm::ivec2& chunkCoord) {
    this->chunks[chunkCoord] = this->generator.generateChunk(
        shared_from_this(),
        chunkCoord
    );

    const std::array<glm::ivec2, 4> neighborChunks = {{
        { +1, 0 },
        { -1, 0 },
        { 0, +1 },
        { 0, -1 },
    }};

    for (auto& neighbor: neighborChunks) {
        auto neighborChunkCoord = chunkCoord + neighbor;

        if (isChunkLoaded(neighborChunkCoord)) {
            this->chunks[neighborChunkCoord]->prepareToBuildMesh();
        }
    }
}

void World::unloadChunk(const glm::ivec2& chunkCoord) {
    this->chunks.erase(chunkCoord);
}

bool World::isChunkLoaded(const glm::ivec2& chunkCoord) {
    return this->chunks.find(chunkCoord) != this->chunks.end();
}

void World::render() {
    for (const auto& [coord, chunk]: this->chunks) {
        glm::mat4 model =  glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * Chunk::Length, 0, coord.y * Chunk::Width));
        Resource::ChunkShader.setMat4("model", model);
        chunk->render();
    }
}

BlockData World::getBlockDataIfLoadedAt(const glm::ivec3& coord) {
    glm::ivec2 chunkCoord;

    if (coord.x >= 0) {
        chunkCoord.x = coord.x / Chunk::Length;
    } else {
        chunkCoord.x = (coord.x+1) / Chunk::Length - 1;
    }

    if (coord.z >= 0) {
        chunkCoord.y = coord.z / Chunk::Width;
    } else {
        chunkCoord.y = (coord.z+1) / Chunk::Width - 1;
    }

    auto iter = this->chunks.find(chunkCoord);

    if (iter != this->chunks.end()) {
        auto chunk = iter->second;
        return chunk->getBlockDataAt({
            yc::util::PositiveMod(coord.x, Chunk::Length),
            coord.y,
            yc::util::PositiveMod(coord.z, Chunk::Width)
        });
    }

    return { BlockType::NONE, BlockFaceDirection::NONE };
}

void World::reloadChunks() {
    for (auto& [chunkCoord, chunk]: this->chunks) {
        chunk->buildMesh();
    }
}

size_t World::HashChunkCoord::operator() (const glm::ivec2& coord) const noexcept {
    auto hashX = std::hash<int32_t>{}(coord.x);
    auto hashY = std::hash<int32_t>{}(coord.y);

    if (hashX != hashY) {
        return hashX ^ hashY;
    }

    return hashX;
}

}