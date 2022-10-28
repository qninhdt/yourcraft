#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <FastNoiseLite.h>
#include "world/chunk.h"

namespace yc::world {

class WorldGenerator {

public:

    std::shared_ptr<Chunk> generateChunk(World* world,
        const glm::ivec2& chunkCoord);

    BlockData getBlockData(int32_t height, int32_t maxHeight, float noise);

    void generateTreeAt(World* world, const glm::ivec3& coord, float noise);

    void generateTreeAt(std::shared_ptr<Chunk> chunk, const glm::ivec3& coord, float noise);

    WorldGenerator(int32_t seed);

private:

    int32_t seed;
    FastNoiseLite mountainNoise;
    FastNoiseLite noise2;
    FastNoiseLite noise3;

};

}