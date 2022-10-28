#include "graphic/flora_vertex.h"

namespace yc::graphic {

int32_t GetFloraTexureCoord(yc::world::BlockType blockType, int32_t id) {
    switch (blockType) {
        case yc::world::BlockType::GRASS:
            return (2<<4) + 0;
        case yc::world::BlockType::RED_FLOWER:
            return (2<<4) + 1;
        case yc::world::BlockType::YELLOW_FLOWER:
            return (2<<4) + 2;
        case yc::world::BlockType::BLUE_FLOWER:
            return (2<<4) + 3;
    }
    return 0;
}

}