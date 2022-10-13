#include "glm/gtc/matrix_transform.hpp"
#include "world/world.h"
#include "util/math.h"
#include "resource.h"
#include <iostream>

namespace yc::world {

World::World() {
    
}

void World::loadChunks() {
    int mx = 8;
    for (int x=-mx;x<mx;++x) {
        for (int z=-mx;z<mx;++z) {
            glm::ivec2 coord = { x, z };
            auto chunk = std::make_shared<Chunk>();
            chunk->setCoordinate(shared_from_this(), coord);
            chunk->loadBlock();
            this->chunks[coord] = chunk;
        }
    }
    for (auto& [coord, chunk]: this->chunks) {
        chunk->buildMesh();
    }
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