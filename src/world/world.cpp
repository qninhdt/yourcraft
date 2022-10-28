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
    const int32_t maxUnloadChunkPerFrame = 4;
    const int32_t maxChunksLoadPerFrame = 2;

    int32_t unloadedChunkCount = 0;
    for (const auto& [chunkCoord, chunk]: this->chunks) {
        if (Chunk::DistanceTo(chunkCoord, cameraChunkCoord) > viewDistance && maxUnloadChunkPerFrame>unloadedChunkCount) {
            shouldBeUnloadedChunks.push(chunk);
            unloadedChunkCount += 1;
        }
    }

    while (!shouldBeUnloadedChunks.empty() ) {
        auto chunk = shouldBeUnloadedChunks.front();
        shouldBeUnloadedChunks.pop();

        unloadChunk(chunk->getCoord());
    }
    persistence->syncRegionFiles();

    std::atomic_int chunkCount = 0;

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

void World::renderOpaque(Camera* camera) {
    yc::Resource::GameTexure.bind();
    
    yc::Resource::OpaqueShader.use();
    yc::Resource::OpaqueShader.setMat4("projection_view", camera->getProjectionViewMatrix());

    for (const auto& [coord, chunk]: this->chunks) {
        auto model = glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * Chunk::Length, 0, coord.y * Chunk::Width));
        yc::Resource::OpaqueShader.setMat4("model", model);
        chunk->renderOpaque();
    }
}

void World::renderTransparent(Camera* camera) {
    yc::Resource::GameTexure.bind();

    yc::Resource::TransparentShader.use();
    yc::Resource::TransparentShader.setMat4("projection_view", camera->getProjectionViewMatrix());

    for (const auto& [coord, chunk]: this->chunks) {
        auto model = glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * Chunk::Length, 0, coord.y * Chunk::Width));
        yc::Resource::TransparentShader.setMat4("model", model);
        chunk->renderTransparent();
    }
}

void World::renderFlora(yc::Camera* camera) {
    yc::Resource::GameTexure.bind();

    yc::Resource::FloraShader.use();
    yc::Resource::FloraShader.setMat4("projection_view", camera->getProjectionViewMatrix());

    for (const auto& [coord, chunk]: this->chunks) {
        auto model = glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * Chunk::Length, 0, coord.y * Chunk::Width));
        yc::Resource::FloraShader.setMat4("model", model);
        chunk->renderFlora();
    }
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

void World::spawnTreeAt(const glm::ivec3& coord) {
    generator.generateTreeAt(this, coord, 0.5f);
}

size_t World::HashChunkCoord::operator() (const glm::ivec2& coord) const noexcept {
    auto hashX = std::hash<int32_t>{}(coord.x);
    auto hashY = std::hash<int32_t>{}(coord.y);

    if (hashX != hashY) {
        return hashX ^ hashY;
    }

    return hashX;
}

World::RayCastResult World::raycastCheck(const glm::vec3& position, const glm::vec3& direction, bool discardFlora, bool discardWater) {
    RayCastResult result = {
        .face = { 0, 0, 0 }
    };
    
    float x = floor(position.x);
    float y = floor(position.y);
    float z = floor(position.z);
    float dx = direction.x;
    float dy = direction.y;
    float dz = direction.z;
    int32_t stepX = util::SignNum(dx);
    int32_t stepY = util::SignNum(dy);
    int32_t stepZ = util::SignNum(dz);

    float tMaxX = util::IntBound(position.x, dx);
    float tMaxY = util::IntBound(position.y, dy);
    float tMaxZ = util::IntBound(position.z, dz);

    float tDeltaX = stepX/dx;
    float tDeltaY = stepY/dy;
    float tDeltaZ = stepZ/dz;

    float radius = 500.0/sqrt(dx*dx+dy*dy+dz*dz);

    while (true) {
        yc::world::BlockData block = getBlockDataIfLoadedAt({ x, y, z });
        yc::world::BlockType blockType = block.getType();

        if (blockType == yc::world::BlockType::NONE) {
            result.block.setType(BlockType::NONE);
            break;
        } else if (blockType != yc::world::BlockType::AIR && 
            !(discardWater && blockType == yc::world::BlockType::WATER) &&
            !(discardFlora && block.isFlora())) {
            result.block = block;
            result.coord = { x, y, z };
            break;
        }

        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                if (tMaxX > radius) { result.block.setType(BlockType::NONE); break; }
                // Update which cube we are now in.
                x += stepX;
                // Adjust tMaxX to the next X-oriented boundary crossing.
                tMaxX += tDeltaX;
                // Record the normal vector of the cube face we entered.
                result.face[0] = -stepX;
                result.face[1] = 0;
                result.face[2] = 0;
            } else {
                if (tMaxZ > radius) { result.block.setType(BlockType::NONE); break; }
                z += stepZ;
                tMaxZ += tDeltaZ;
                result.face[0] = 0;
                result.face[1] = 0;
                result.face[2] = -stepZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                if (tMaxY > radius) { result.block.setType(BlockType::NONE); break; }
                y += stepY;
                tMaxY += tDeltaY;
                result.face[0] = 0;
                result.face[1] = -stepY;
                result.face[2] = 0;
            } else {
                if (tMaxZ > radius) { result.block.setType(BlockType::NONE); break; }
                z += stepZ;
                tMaxZ += tDeltaZ;
                result.face[0] = 0;
                result.face[1] = 0;
                result.face[2] = -stepZ;
            }
        }
    }

    return result;
}

}