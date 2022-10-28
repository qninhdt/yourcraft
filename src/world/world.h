#pragma once

#include <unordered_map>
#include <queue>
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

    struct RayCastResult {
        BlockData block;
        glm::ivec3 coord;
        glm::ivec3 face;
    };

    static glm::ivec2 GetChunkCoordOf(const glm::ivec3& coord);

    World(Persistence* persistence);

    void init();

    void update(yc::Camera* camera);

    void renderOpaque(yc::Camera* camera);

    void renderTransparent(yc::Camera* camera);

    void renderFlora(yc::Camera* camera);

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

    RayCastResult raycastCheck(const glm::vec3& position, const glm::vec3& direction, bool discardFlora, bool discardWater);

    int32_t getSeed() const;

private:

    std::unordered_map<glm::ivec2, std::shared_ptr<Chunk>, HashChunkCoord> chunks;
    std::queue<std::shared_ptr<Chunk>> shouldBeUnloadedChunks;

    WorldGenerator generator;
    Persistence* persistence;
};

}