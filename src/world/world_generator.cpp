#include "world/world_generator.h"
#include <iostream>
#include "world/world.h"

namespace yc::world {

static BlockData dirtBlock { BlockType::DIRT };
static BlockData grassBlock { BlockType::GRASS };
static BlockData stoneBlock { BlockType::STONE };
static BlockData sandBlock { BlockType::SAND };
static BlockData snowBlock { BlockType::SNOW };
static BlockData woodBlock { BlockType::WOOD };
static BlockData leafBlock { BlockType::LEAF };

WorldGenerator::WorldGenerator(int32_t seed):
    seed(seed),
    mountainNoise(seed) {
    this->mountainNoise.SetFractalOctaves(5);
    this->mountainNoise.SetFrequency(0.002f);
    this->mountainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    this->mountainNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    // this->mountainNoise.SetFractalWeightedStrength(1);
}

BlockData WorldGenerator::getBlockData(int32_t height, int32_t maxHeight, float noise) {
    if (height == 0) return stoneBlock;

    if (height <=46) return sandBlock;
    
    if (height >= 125 + noise*10) return snowBlock;

    if (height >= 100 + noise*10) return stoneBlock;

    if (height == maxHeight) return grassBlock;

    return dirtBlock;
}

std::shared_ptr<Chunk> WorldGenerator::generateChunk(World* world,
    const glm::ivec2& chunkCoord) {
        
    auto chunk = std::make_shared<Chunk>();
    chunk->setCoordinate(world, chunkCoord);

    glm::vec3 worldCoord = chunk->getWorldCoord();
    
    for (int32_t x=0; x<Chunk::Length; ++x) 
    for (int32_t z=0; z<Chunk::Width; ++z) {
        float noiseX = worldCoord.x + static_cast<float>(x);
        float noiseZ = worldCoord.z + static_cast<float>(z);
        float mountainNoiseValue = 
            0.50 * this->mountainNoise.GetNoise(noiseX, noiseZ) +
            0.35 * this->mountainNoise.GetNoise(noiseX*2, noiseZ*2) +
            0.15 * this->mountainNoise.GetNoise(noiseX*4, noiseZ*4);

        float noise = mountainNoise.GetNoise(noiseX*16, noiseZ*16);
        noise = (noise+1)/2;

        float noiseValue = ((mountainNoiseValue)+1)/2;
        noiseValue = pow(noiseValue, 3);

        float treeNoise = mountainNoise.GetNoise(noiseX*1024, noiseZ*1024);
        treeNoise = (treeNoise+1)/2;
        treeNoise = pow(treeNoise, 4);

        float forestNoise = mountainNoise.GetNoise(noiseX*4, noiseZ*4);
        forestNoise = (forestNoise+1)/2;

        int32_t height = noiseValue * 220 + 30;
        
        glm::ivec3 coord { x, 0, z }; 
        for (;coord.y<=height; ++coord.y) {
            chunk->setBlockData(coord, getBlockData(coord.y, height, noise));
        }

        if (treeNoise > 0.45f && forestNoise > 0.5f) {
            if (0<x && x<15 && 0<z && z<15 && height>50 && height<100) {
                generateTreeAt(chunk, coord, 1.0f);
            }
        } 

        for (;coord.y<=44;++coord.y) {
            chunk->setBlockData(coord, { BlockType::WATER });
        }
    }

    chunk->prepareToBuildMesh();

    return chunk;
}

void WorldGenerator::generateTreeAt(std::shared_ptr<Chunk> chunk, const glm::ivec3& coord, float noise) {
    int32_t height = 4;
    int32_t leafHeight = 3;

    // build leafs
    for (int y=height-(leafHeight+1)/2; y<=height+leafHeight/2;++y) {
        for (int x=-1;x<=1;++x) {
            for (int z=-1;z<=1;++z) {
                chunk->setBlockData({ coord.x+x, coord.y+y, coord.z+z }, leafBlock);
            }
        }
    }

    // build woods
    for (int i=0; i<height; ++i) {
        chunk->setBlockData({ coord.x, coord.y+i, coord.z }, woodBlock);
    }
    
    chunk->prepareToBuildMesh();
}

void WorldGenerator::generateTreeAt(World* world, const glm::ivec3& coord, float noise) {
    generateTreeAt(world->getChunkIfLoadedAt(World::GetChunkCoordOf(coord)), {
        coord.x & 15,
        coord.y,
        coord.z & 15
    }, noise);
}

}