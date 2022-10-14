#include "glm/gtc/matrix_transform.hpp"
#include "world/world.h"
#include "util/math.h"
#include "resource.h"
#include <iostream>

namespace yc::world {

World::World() {
    
}

void World::update(const yc::Camera& camera) {
    glm::ivec2 cameraChunkCoord = {
        floor(camera.getPosition().x/Chunk::Length),
        floor(camera.getPosition().z/Chunk::Width),
    };

    const int32_t viewDistance = 8;

    for (auto& [chunkCoord, chunk]: this->chunks) {
        if (Chunk::DistanceTo(chunkCoord, cameraChunkCoord) > viewDistance) {
            this->unloadChunk(chunkCoord);
        }
    }

    for (int32_t x=-viewDistance; x<=viewDistance; ++x) {
        for (int32_t z=-viewDistance; z<=viewDistance; ++z) {
            glm::ivec2 chunkCoordToCheck = glm::ivec2(x, z) + cameraChunkCoord;
            
            if (Chunk::DistanceTo(chunkCoordToCheck, cameraChunkCoord) <= viewDistance) {
                generateOrLoadChunkAt(chunkCoordToCheck);
            }
        }
    }

    for (auto& chunk: this->unbuiltChunks) {
        chunk->buildMesh();
    }

    this->unbuiltChunks.clear();
}

void World::generateOrLoadChunkAt(const glm::ivec2& chunkCoord) {
    // exit function if chunk is already loaded
    if (isChunkLoaded(chunkCoord)) {
        return;
    }

    // remove later
    auto chunk = std::make_shared<Chunk>();

    chunk->setCoordinate(shared_from_this(), chunkCoord);
    chunk->loadBlock();

    this->chunks[chunkCoord] = chunk;
    this->unbuiltChunks.push_back(chunk);
}

void World::unloadChunk(const glm::ivec2& chunkCoord) {
    this->chunks.erase(chunkCoord);
}

bool World::isChunkLoaded(const glm::ivec2& chunkCoord) {
    return this->chunks.find(chunkCoord) != this->chunks.end();
}

void World::loadChunks() {

}

void World::render() {
    for (const auto& [coord, chunk]: this->chunks) {
        glm::mat4 model =  glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * Chunk::Length, 0, coord.y * Chunk::Width));
        Resource::ChunkShader.setMat4("model", model);
        chunk->render();
    }
}

BlockData World::getBlockDataIfLoadedAt(const glm::ivec3& coord) {
    glm::ivec2 chunkCoord = { 0, 0 };

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

size_t World::HashChunkCoord::operator() (const glm::ivec2& coord) const noexcept {
    auto hashX = std::hash<int32_t>{}(coord.x);
    auto hashY = std::hash<int32_t>{}(coord.y);

    if (hashX != hashY) {
        return hashX ^ hashY;
    }

    return hashX;
}

}