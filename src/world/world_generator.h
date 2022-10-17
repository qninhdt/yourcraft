#pragma once

#include <memory.h>
#include <glm/glm.hpp>
#include <FastNoiseLite.h>
#include "world/chunk.h"

namespace yc::world {

class WorldGenerator {

public:

    std::shared_ptr<Chunk> generateChunk(std::shared_ptr<World> world,
        const glm::ivec2& chunkCoord);

    WorldGenerator(int32_t seed);

private:

    int32_t seed;
    FastNoiseLite noise1;
    FastNoiseLite noise2;
    FastNoiseLite noise3;

};

}