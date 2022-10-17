#pragma once

#include <unordered_map>
#include <map>
#include "thread_pool.h"
#include "world/world_generator.h"
#include "world/chunk.h"
#include "camera.h"

namespace yc::world {

class World: public std::enable_shared_from_this<World> {

public:

    struct HashChunkCoord {
        size_t operator() (const glm::ivec2& coord) const noexcept;
    };

    World();

    void init();

    void update(const yc::Camera& camera);

    void render();

    BlockData getBlockDataIfLoadedAt(const glm::ivec3& coord);

    std::shared_ptr<Chunk> getChunkIfLoadedAt(const glm::ivec2& coord);

    bool isChunkLoaded(const glm::ivec2& chunkCoord);

    void generateOrLoadChunkAt(const glm::ivec2& chunkCoord);

    void unloadChunk(const glm::ivec2& chunkCoord);

    void reloadChunks();

    int32_t getSeed() const;

    std::shared_ptr<FastNoiseLite> getNoiseHandle() const;

private:

    std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, HashChunkCoord> chunks;
    std::mutex chunksLock;

    WorldGenerator generator;

    ThreadPool chunkPool;
};

}