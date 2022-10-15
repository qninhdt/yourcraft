#include "world/world_generator.h"

namespace yc::world {

WorldGenerator::WorldGenerator(int32_t seed): seed(seed), noiseHandle(seed) {
    this->noiseHandle.SetFractalOctaves(5);
    this->noiseHandle.SetFractalLacunarity(1.75);
    this->noiseHandle.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    this->noiseHandle.SetFractalType(FastNoiseLite::FractalType_FBm);
}

std::shared_ptr<Chunk> WorldGenerator::generateChunk(std::shared_ptr<World> world,
    const glm::ivec2& chunkCoord) {
    
    auto chunk = std::make_shared<Chunk>();
    chunk->setCoordinate(world, chunkCoord);

    glm::vec3 worldCoord = chunk->getWorldCoord();
    
    for (int32_t x=0; x<Chunk::Length; ++x) {
        for (int32_t z=0; z<Chunk::Width; ++z) {
            float noiseX = worldCoord.x + static_cast<float>(x);
            float noiseZ = worldCoord.z + static_cast<float>(z);
            float noiseValue = this->noiseHandle.GetNoise(noiseX, noiseZ);

            int32_t height = 45 + static_cast<int32_t>(noiseValue * 45);

            for (int32_t y=0; y<height; ++y) {
                glm::ivec3 blockCoord = { x, y, z };
                BlockData blockData;

                blockData.setType(BlockType::DIRT);

                chunk->setBlockData(blockCoord, blockData);
            }
        }
    }

    return chunk;
}

}