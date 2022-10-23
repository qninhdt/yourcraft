#include "world/world_generator.h"
#include <iostream>

namespace yc::world {

WorldGenerator::WorldGenerator(int32_t seed):
    seed(seed),
    mountainNoise(seed) {
    this->mountainNoise.SetFractalOctaves(5);
    this->mountainNoise.SetFrequency(0.002f);
    this->mountainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    this->mountainNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    // this->mountainNoise.SetFractalWeightedStrength(1);
}

std::shared_ptr<Chunk> WorldGenerator::generateChunk(World* world,
    const glm::ivec2& chunkCoord) {
        
    auto chunk = std::make_shared<Chunk>();
    chunk->setCoordinate(world, chunkCoord);

    // pre init
    static BlockData dirtBlock { BlockType::DIRT };
    static BlockData grassBlock { BlockType::GRASS };
    static BlockData stoneBlock { BlockType::STONE };
    static BlockData waterBlock { BlockType::WATER };

    glm::vec3 worldCoord = chunk->getWorldCoord();
    
    for (int32_t x=0; x<Chunk::Length; ++x) 
    for (int32_t z=0; z<Chunk::Width; ++z) {
        float noiseX = worldCoord.x + static_cast<float>(x);
        float noiseZ = worldCoord.z + static_cast<float>(z);
        float mountainNoiseValue = 
            0.50 * this->mountainNoise.GetNoise(noiseX, noiseZ) +
            0.35 * this->mountainNoise.GetNoise(noiseX*2, noiseZ*2) +
            0.15 * this->mountainNoise.GetNoise(noiseX*4, noiseZ*4);

        float noiseValue = ((mountainNoiseValue)+1)/2;
        noiseValue = pow(noiseValue, 3);

        int32_t height = noiseValue * 226 + 30;

        glm::ivec3 coord { x, 0, z };
        
        chunk->setBlockData(coord, stoneBlock);
        coord.y++;

        for (;coord.y<=std::min(100, height-1); ++coord.y) {
            chunk->setBlockData(coord, dirtBlock);
        }

        if (height<100) {
            chunk->setBlockData(coord, grassBlock);
            coord.y++;
        }

        for (;coord.y<=std::min(150, height-1); ++coord.y) {
            chunk->setBlockData(coord, stoneBlock);
        }

        

        for (int32_t y=height; y<=40; ++y) {
            chunk->setBlockData(coord, waterBlock);
            coord.y++;
        }
    }

    chunk->prepareToBuildMesh();

    return chunk;
}

}