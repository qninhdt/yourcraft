#pragma once

#include <unordered_map>
#include "world/chunk.h"
#include "camera.h"

namespace yc::world {

class World: public std::enable_shared_from_this<World> {

public:

    struct HashChunkCoord {
        size_t operator() (const glm::ivec2& coord) const noexcept;
    };

    World();

    void loadChunks();

    void update(const yc::Camera& camera);

    void render();

    BlockData getBlockDataIfLoadedAt(const glm::ivec3& coord);

    bool isChunkLoaded(const glm::ivec2& chunkCoord);

    void generateOrLoadChunkAt(const glm::ivec2& chunkCoord);

    void unloadChunk(const glm::ivec2& chunkCoord);

private:

    std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, HashChunkCoord> chunks;

    std::vector<std::shared_ptr<Chunk>> unbuiltChunks;
};

}