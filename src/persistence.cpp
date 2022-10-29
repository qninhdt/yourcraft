#include "persistence.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <assert.h>

namespace yc {

const std::string Persistence::WorldFolder = "save/"; 

const size_t ChunkSize = yc::world::Chunk::Volume * sizeof(yc::world::BlockData);
const size_t RegionHeaderSize = sizeof(int16_t) + sizeof(int16_t) * 32 * 32;

Persistence::Persistence() {}

Persistence::~Persistence() {
    for (auto& [regionCoord, region]: this->regions) {
        region->input->close();
    }
}

void Persistence::loadRegion(const glm::ivec2& regionCoord) {
    std::shared_ptr<Region> region = std::make_shared<Region>();
    region->coord = regionCoord;

    std::string fileName = getRegionFileName(regionCoord);
    std::string filePath = WorldFolder + fileName;

    // create & setup region file if not exist
    std::ifstream checker(filePath);
    bool firstTime = checker.fail();
    if (firstTime) {
        std::ofstream newFile(filePath);

        // fill offets by value 1024 (1024 -> not generated)
        for (int i=0;i<32*32;++i) {
            region->offsets[i] = -1;
        }

        region->numGeneratedChunks = 0;
        newFile.write((char*) &region->numGeneratedChunks, sizeof(int16_t));
        newFile.write((char*) region->offsets, sizeof(region->offsets));

        newFile.close();

        std::cout << "Created new region file " << fileName << "\n";
    }
    checker.close();

    region->input = std::make_shared<std::ifstream>(filePath, std::ios::binary|std::ios::in);
    region->output = nullptr;

    // load offset array if region file is already exist
    if (!firstTime) {
        region->input->seekg(0, std::ios::beg);
        region->input->read((char*) &region->numGeneratedChunks, sizeof(int16_t));
        region->input->seekg(sizeof(int16_t), std::ios::beg);
        region->input->read((char*) &region->offsets[0], sizeof(region->offsets));
    }

    region->coord = regionCoord;
    this->regions[regionCoord] = region;
}

std::shared_ptr<yc::world::Chunk> Persistence::getChunk(const glm::ivec2& chunkCoord, yc::world::World* world) {
    const glm::ivec2 localChunkCoord = { chunkCoord.x & 31, chunkCoord.y & 31 };
    const glm::ivec2 regionCoord = { chunkCoord.x >> 5, chunkCoord.y >> 5 };
    const int16_t chunkId = localChunkCoord.x + localChunkCoord.y * 32;
    
    if (this->regions.find(regionCoord) == this->regions.end()) {
        loadRegion(regionCoord);
    }

    // begin reading region file
    const auto& region = this->regions[regionCoord];

    if (region->offsets[chunkId] == -1) {
        return nullptr;
    }

    region->input->clear();

    auto chunk = std::make_shared<yc::world::Chunk>();
    chunk->setCoordinate(world, chunkCoord);

    // read chunk data
    region->input->seekg(RegionHeaderSize + ChunkSize * region->offsets[chunkId], std::ios::beg);
    region->input->read((char*) chunk->getChunkData(), ChunkSize);

    return chunk;
}

void Persistence::saveChunk(std::shared_ptr<yc::world::Chunk> chunk) {
    // return;
    const glm::ivec2 chunkCoord = chunk->getCoord();
    const glm::ivec2 localChunkCoord = { chunkCoord.x & 31, chunkCoord.y & 31 };
    const glm::ivec2 regionCoord = { chunkCoord.x >> 5, chunkCoord.y >> 5 };
    const int16_t chunkId = localChunkCoord.x + localChunkCoord.y * 32;

    // begin writing region file
    const auto& region = this->regions[regionCoord];

    if (region->output == nullptr) {
        auto filePath = WorldFolder + getRegionFileName(regionCoord);
        region->output = std::make_shared<std::ofstream>(filePath, std::ios::binary|std::ios::out|std::ios::in);
    }

    // chunk is not stored in file
    if (region->offsets[chunkId] == -1) {
        // set chunk offset
        region->offsets[chunkId] = region->numGeneratedChunks;

        region->output->seekp(sizeof(int16_t) + chunkId * sizeof(int16_t), std::ios::beg);
        region->output->write((char*) &region->offsets[chunkId], sizeof(int16_t));

        // update numGeneratedChunks
        region->numGeneratedChunks += 1;
        region->output->seekp(0, std::ios::beg);
        region->output->write((char*) &region->numGeneratedChunks, sizeof(int16_t));
    }

    // insert chunk data
    region->output->seekp(RegionHeaderSize + ChunkSize * region->offsets[chunkId], std::ios::beg);
    region->output->write((char*) chunk->getChunkData(), ChunkSize);
}

void Persistence::syncRegionFiles() {
    for (auto& [regionCoord, region]: this->regions) {
        if (region->output != nullptr) {
            region->output->close();
            region->output = nullptr;
        }
    }
}

std::string Persistence::getRegionFileName(const glm::ivec2& regionCoord) const {
    std::stringstream fileName;
    fileName << "r." << regionCoord.x << "." << regionCoord.y << ".cjbt";
    return fileName.str();
}

size_t Persistence::HashRegionCoord::operator() (const glm::ivec2& coord) const noexcept {
    auto hashX = std::hash<int32_t>{}(coord.x);
    auto hashY = std::hash<int32_t>{}(coord.y);

    if (hashX != hashY) {
        return hashX ^ hashY;
    }

    return hashX;
}

}