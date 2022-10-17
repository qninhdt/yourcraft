#include "world/world_generator.h"
#include <iostream>
namespace yc::world {

WorldGenerator::WorldGenerator(int32_t seed): seed(seed), noise1(seed), noise2(seed), noise3(seed) {
    this->noise1.SetFractalOctaves(5);
    this->noise1.SetFrequency(0.008f);
    this->noise1.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    this->noise1.SetFractalType(FastNoiseLite::FractalType_FBm);
    this->noise2.SetFractalOctaves(5);
    this->noise2.SetFrequency(0.004f);
    this->noise2.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    this->noise2.SetFractalType(FastNoiseLite::FractalType_FBm);
    this->noise3.SetFractalOctaves(5);
    this->noise3.SetFrequency(0.002f);
    this->noise3.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    this->noise3.SetFractalType(FastNoiseLite::FractalType_FBm);
}

std::shared_ptr<Chunk> WorldGenerator::generateChunk(std::shared_ptr<World> world,
    const glm::ivec2& chunkCoord) {
        
    auto chunk = std::make_shared<Chunk>();
    chunk->setCoordinate(world, chunkCoord);

    // pre init
    static BlockData dirtBlock { BlockType::DIRT };
    static BlockData grassBlock { BlockType::GRASS };
    static BlockData stoneBlock { BlockType::STONE };

    glm::vec3 worldCoord = chunk->getWorldCoord();
    
    for (int32_t x=0; x<Chunk::Length; ++x) 
    for (int32_t z=0; z<Chunk::Width; ++z) {
        float noiseX = worldCoord.x + static_cast<float>(x);
        float noiseZ = worldCoord.z + static_cast<float>(z);
        float noiseValue1 = this->noise1.GetNoise(noiseX, noiseZ);
        float noiseValue2 = this->noise2.GetNoise(noiseX, noiseZ);
        float noiseValue3 = this->noise3.GetNoise(noiseX, noiseZ);
        float noiseValue = ((noiseValue1 + noiseValue2 + noiseValue3)/3+1)/2;

        int32_t height = noiseValue * 128;
        glm::ivec3 coord { x, 0, z };
        
        for (int32_t y=0; y<height*3/4; ++y) {
            chunk->setBlockData(coord, stoneBlock);
            coord.y++;
        }
        for (int32_t y=height*3/4; y<height-1; ++y) {
            chunk->setBlockData(coord, dirtBlock);
            coord.y++;
        }
        chunk->setBlockData(coord, grassBlock);
    }

    chunk->prepareToBuildMesh();

    return chunk;
}

}