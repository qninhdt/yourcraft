#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include "world/chunk.h"

namespace yc {

struct Region {
    glm::ivec2 coord;
    int16_t offsets[1024];
    int16_t numGeneratedChunks;
    std::shared_ptr<std::ifstream> input;
    std::shared_ptr<std::ofstream> output;
};

class Persistence {

const static std::string WorldFolder;

public:

    Persistence();

    ~Persistence();

    void loadRegion(const glm::ivec2& regionCoord);

    void saveChunk(std::shared_ptr<yc::world::Chunk> chunk);

    void syncRegionFiles();

    std::string getRegionFileName(const glm::ivec2& regionCoord) const;

    std::shared_ptr<yc::world::Chunk> getChunk(const glm::ivec2& chunkCoord, yc::world::World* world);

private:

    struct HashRegionCoord {
        size_t operator() (const glm::ivec2& coord) const noexcept;
    };

    std::unordered_map<glm::ivec2, std::shared_ptr<Region>, HashRegionCoord> regions;

};

}