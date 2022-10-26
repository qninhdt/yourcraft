#pragma once

#include <unordered_map>
#include <unordered_set>
#include "persistence.h"
#include "world/world_generator.h"
#include "world/chunk.h"
#include "player.h"

namespace yc::world {

class World {

public:

    struct HashChunkCoord {
        size_t operator() (const glm::ivec2& coord) const noexcept;
    };

    static glm::ivec2 GetChunkCoordOf(const glm::ivec3& coord);

    World(Persistence* persistence);

    void init();

    void update(yc::Camera* camera);

    void renderOpaque(yc::Camera* camera);

    void renderTransparent(yc::Camera* camera);

    BlockData getBlockDataIfLoadedAt(const glm::ivec3& coord);

    bool setBlockDataIfLoadedAt(const glm::ivec3& coord, const BlockData& blockData);
    
    bool destroyBlockIfLoaded(const glm::ivec3& coord);

    std::shared_ptr<Chunk> getChunkIfLoadedAt(const glm::ivec2& coord);

    bool isChunkLoaded(const glm::ivec2& chunkCoord);

    void generateOrLoadChunkAt(const glm::ivec2& chunkCoord);

    void unloadChunk(const glm::ivec2& chunkCoord);

    void reloadChunks();

    void saveChunks();

    void spawnTreeAt(const glm::ivec3& coord);

    int32_t getSeed() const;

private:

    std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, HashChunkCoord> chunks;

    WorldGenerator generator;
    Persistence* persistence;
};

}