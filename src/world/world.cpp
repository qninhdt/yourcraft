#include <iostream>
#include <atomic>
#include "glm/gtc/matrix_transform.hpp"
#include "world/world.h"
#include "util/math.h"
#include "resource.h"
#include <sstream>

namespace yc::world {

World::World(Persistence* persistence):
    generator(240404),
    persistence(persistence) {}

void World::init() {
}

void World::update(yc::Camera* camera) {
    glm::ivec2 cameraChunkCoord = {
        camera->getPosition().x/Chunk::Length,
        camera->getPosition().z/Chunk::Width,
    };

    const int32_t viewDistance = 24;
    std::vector<std::shared_ptr<Chunk>> shouldBeUnloadedChunks;
    for (const auto& [chunkCoord, chunk]: this->chunks) {
        if (Chunk::DistanceTo(chunkCoord, cameraChunkCoord) > viewDistance) {
            shouldBeUnloadedChunks.push_back(chunk);
        }
    }

    for(const auto& chunk: shouldBeUnloadedChunks) {
        unloadChunk(chunk->getCoord());
    }
    persistence->syncRegionFiles();

    std::atomic_int chunkCount = 0;
    const int32_t maxChunksLoadPerFrame = 2;

    int32_t x=0, z=0, dx=0, dz=-1;
    int32_t size = viewDistance*2+1;
    int32_t numChunksToCheck = size*size;

    while (numChunksToCheck-- && chunkCount < maxChunksLoadPerFrame) {
        glm::ivec2 chunkCoordToCheck = glm::ivec2(x, z) + cameraChunkCoord;

        if (Chunk::DistanceTo(chunkCoordToCheck, cameraChunkCoord) <= viewDistance) {
                auto iter = chunks.find(chunkCoordToCheck);
                if (iter == chunks.end()) {
                    generateOrLoadChunkAt(chunkCoordToCheck);
                    ++chunkCount;
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

    for (const auto& [chunkCoord, chunk]: this->chunks) {
        chunk->buildMeshIfNeeded();
    }
}

void World::generateOrLoadChunkAt(const glm::ivec2& chunkCoord) {
    auto chunk = persistence->getChunk(chunkCoord, this);

    if (chunk == nullptr) {
        chunk = this->generator.generateChunk(this, chunkCoord);
    }
    
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

    this->chunks[chunkCoord] = chunk;
}

void World::unloadChunk(const glm::ivec2& chunkCoord) {
    persistence->saveChunk(this->chunks[chunkCoord]);
    this->chunks.erase(chunkCoord);
}

bool World::isChunkLoaded(const glm::ivec2& chunkCoord) {
    auto iter = this->chunks.find(chunkCoord);
    return iter != this->chunks.end();
}

void World::render(Camera* camera) {
    yc::Resource::GameTexure.bind();
    
    static glm::mat4 modelMats[5000];
    uint32_t idx;

    // pre-calc model matrix
    idx = 0;
    for (const auto& [coord, chunk]: this->chunks) {
        modelMats[idx++] =  glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * Chunk::Length, 0, coord.y * Chunk::Width));
    }

    yc::Resource::OpaqueShader.use();
    yc::Resource::OpaqueShader.setMat4("projection_view", camera->getProjectionViewMatrix());

    idx = 0;
    for (const auto& [coord, chunk]: this->chunks) {
        yc::Resource::OpaqueShader.setMat4("model", modelMats[idx++]);
        chunk->renderOpaque();
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    yc::Resource::TransparentShader.use();
    yc::Resource::TransparentShader.setMat4("projection_view", camera->getProjectionViewMatrix());

    idx = 0;
    for (const auto& [coord, chunk]: this->chunks) {
        yc::Resource::TransparentShader.setMat4("model", modelMats[idx++]);
        chunk->renderTransparent();
    }

    glDisable(GL_BLEND);
}

void World::saveChunks() {
    std::cout << "Saving world . . .\n";
    for (auto& [coord, chunk]: this->chunks) {
        persistence->saveChunk(chunk);
    }
    persistence->syncRegionFiles();
}

BlockData World::getBlockDataIfLoadedAt(const glm::ivec3& coord) {
    if (coord.y >= 0 && coord.y < 256) {
        glm::ivec2 chunkCoord = GetChunkCoordOf(coord);
        auto iter = this->chunks.find(chunkCoord);

        if (iter != this->chunks.end()) {
            auto chunk = iter->second;
            return chunk->getBlockDataAt({
                yc::util::PositiveMod(coord.x, Chunk::Length),
                coord.y,
                yc::util::PositiveMod(coord.z, Chunk::Width)
            });
        }
    }

    return { BlockType::NONE, BlockFaceDirection::NONE };
}

// no thread-safe
void World::reloadChunks() {
    std::cout << this->chunks.size() << '\n';
    double begin = glfwGetTime();
    for (auto& [chunkCoord, chunk]: this->chunks) {
        chunk->buildMesh();
    }
    double end = glfwGetTime();

    std::cout << "Reloaded chunks in " << end - begin << " s\n";
}

// no thread-safe
std::shared_ptr<Chunk> World::getChunkIfLoadedAt(const glm::ivec2& coord) {
    auto iter = this->chunks.find(coord);

    if (iter != this->chunks.end()) {
        return iter->second;
    } else {
        return nullptr;
    }
}

glm::ivec2 World::GetChunkCoordOf(const glm::ivec3& coord) {
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

    return chunkCoord;
}

bool World::destroyBlockIfLoaded(const glm::ivec3& coord) {
    return setBlockDataIfLoadedAt(coord, { yc::world::BlockType::AIR });
}

bool World::setBlockDataIfLoadedAt(const glm::ivec3& coord, const BlockData& blockData) {
    if (coord.y<0 || coord.y>=256) return false;

    glm::ivec2 chunkCoord = GetChunkCoordOf(coord);

    auto iter = this->chunks.find(chunkCoord);

    if (iter != this->chunks.end()) {
        auto chunk = iter->second;

        chunk->setBlockData({
            util::PositiveMod(coord.x, Chunk::Length),
            coord.y,
            util::PositiveMod(coord.z, Chunk::Width)
        }, blockData);
        chunk->prepareToBuildMesh();

        glm::ivec3 localCoord = { coord.x & 15, coord.y, coord.z & 15 };

        if (localCoord.x == 0) {
            auto westChunk = getChunkIfLoadedAt({ chunkCoord.x-1, chunkCoord.y });
            if (westChunk) westChunk->prepareToBuildMesh();
        } else if (localCoord.x == 15) {
            auto eastChunk = getChunkIfLoadedAt({ chunkCoord.x+1, chunkCoord.y });
            if (eastChunk) eastChunk->prepareToBuildMesh();
        }

        if (localCoord.z == 0) {
            auto northChunk = getChunkIfLoadedAt({ chunkCoord.x, chunkCoord.y-1 });
            if (northChunk) northChunk->prepareToBuildMesh();
        } else if (localCoord.z == 15) {
            auto southChunk = getChunkIfLoadedAt({ chunkCoord.x, chunkCoord.y+1 });
            if (southChunk) southChunk->prepareToBuildMesh();
        }

        return true;
    } else {    
        return false;
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