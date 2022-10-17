#include <iostream>
#include <atomic>
#include "glm/gtc/matrix_transform.hpp"
#include "world/world.h"
#include "util/math.h"
#include "resource.h"
#include <sstream>

namespace yc::world {

World::World(): generator(240404) {}

void World::init() {
    this->chunkPool.start();
}

void World::update(const yc::Camera& camera) {
    glm::ivec2 cameraChunkCoord = {
        camera.getPosition().x/Chunk::Length,
        camera.getPosition().z/Chunk::Width,
    };

    const int32_t viewDistance = 32;

    {
        std::unique_lock<std::mutex> lock(this->chunksLock);
        for (auto& [chunkCoord, chunk]: this->chunks) {
            if (Chunk::DistanceTo(chunkCoord, cameraChunkCoord) > viewDistance) {
                this->unloadChunk(chunkCoord);
            }
        }
    }

    std::atomic_int chunkCount = 0;
    const int32_t maxChunksLoadPerFrame = 8;

    int32_t x=0, z=0, dx=0, dz=-1;
    int32_t size = viewDistance*2+1;
    int32_t numChunksToCheck = size*size;
    // double begin = glfwGetTime();

    while (numChunksToCheck-- && chunkCount < maxChunksLoadPerFrame) {
        glm::ivec2 chunkCoordToCheck = glm::ivec2(x, z) + cameraChunkCoord;

        if (Chunk::DistanceTo(chunkCoordToCheck, cameraChunkCoord) <= viewDistance) {
            {
                std::unique_lock<std::mutex> lock(this->chunksLock);
                auto iter = chunks.find(chunkCoordToCheck);
                if (iter == chunks.end()) {
                    this->chunks[chunkCoordToCheck] = nullptr; // nullptr -> waiting threadpool to build
                    this->chunkPool.queueJob([chunkCoordToCheck, this] {
                        generateOrLoadChunkAt(chunkCoordToCheck);
                    });
                }
            }
        }

        if (x == z || (x < 0 && x == -z) || (x > 0 && x == 1-z)) {
            int32_t t = dx;
            dx = -dz;
            dz = t;
        }

        x += dx;
        z += dz;
    }

    // if (chunkCount == 400) {
    //     std::cout << "Build 400 chunks in " << glfwGetTime() - begin << " s\n";
    // }

    {
        std::unique_lock<std::mutex> lock(this->chunksLock);
        for (auto& [chunkCoord, chunk]: this->chunks) {
            if (chunk != nullptr) {
                chunk->buildMeshIfNeeded();
            }
        }
    }

}

void World::generateOrLoadChunkAt(const glm::ivec2& chunkCoord) {
    {
        // std::unique_lock<std::mutex> lock(this->chunksLock);
        if (this->chunks.find(chunkCoord)==this->chunks.end()) return;
    }
    auto chunk = this->generator.generateChunk(
        shared_from_this(),
        chunkCoord
    );

    const std::array<glm::ivec2, 4> neighborChunks = {{
        { +1, 0 },
        { -1, 0 },
        { 0, +1 },
        { 0, -1 },
    }};

    std::unique_lock<std::mutex> lock(this->chunksLock);

    for (auto& neighbor: neighborChunks) {
        auto neighborChunkCoord = chunkCoord + neighbor;

        if (isChunkLoaded(neighborChunkCoord)) {
            this->chunks[neighborChunkCoord]->prepareToBuildMesh();
        }
    }

    auto iter = this->chunks.find(chunkCoord);

    // if chunk is not found in map -> chunk is unloaded before built
    if (iter != this->chunks.end()) {
        iter->second = chunk;
    }
}

void World::unloadChunk(const glm::ivec2& chunkCoord) {
    this->chunks.erase(chunkCoord);
}

bool World::isChunkLoaded(const glm::ivec2& chunkCoord) {
    auto iter = this->chunks.find(chunkCoord);
    return iter != this->chunks.end() && iter->second != nullptr;
}

void World::render() {
    // TODO: unoptimized

    {
        std::unique_lock<std::mutex> lock(this->chunksLock);
        for (const auto& [coord, chunk]: this->chunks) {
            if (chunk == nullptr) continue;

            glm::mat4 model =  glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * Chunk::Length, 0, coord.y * Chunk::Width));
            Resource::ChunkShader.setMat4("model", model);
            chunk->render();
        }

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
    if (iter != this->chunks.end() && iter->second != nullptr) {
        auto chunk = iter->second;
        return chunk->getBlockDataAt({
            yc::util::PositiveMod(coord.x, Chunk::Length),
            coord.y,
            yc::util::PositiveMod(coord.z, Chunk::Width)
        });
    }
    

    return { BlockType::NONE, BlockFaceDirection::NONE };
}

// no thread-safe
void World::reloadChunks() {
    std::cout << this->chunks.size() << '\n';
    double begin = glfwGetTime();
    for (auto& [chunkCoord, chunk]: this->chunks) {
        if (chunk != nullptr)
        chunk->buildMesh();
    }
    double end = glfwGetTime();

    std::cout << "Reload: " << end - begin << " s\n";
}

// no thread-safe
std::shared_ptr<Chunk> World::getChunkIfLoadedAt(const glm::ivec2& coord) {
    auto iter = this->chunks.find(coord);

    if (iter != this->chunks.end() && iter->second != nullptr) {
        return iter->second;
    } else {
        return nullptr;
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