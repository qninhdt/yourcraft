#pragma once

#include "gl/common.h"
#include "world/block.h"

namespace yc::graphic {

const float FloraVertices[8][3] = {
    // x1
    { 0, 0, 0 },
    { 1, 0, 1 },
    { 1, 1, 1 },
    { 0, 1, 0 },
    // x2
    { 1, 0, 0 },
    { 0, 0, 1 },
    { 0 ,1, 1 },
    { 1, 1, 0 },
};

const float FloraTexcoords[8][2] = {
    // x1
    { 0, 0 },
    { 1, 0 },
    { 1, 1 },
    { 0 ,1 },
    // x2
    { 0, 0 },
    { 1, 0 },
    { 1, 1 },
    { 0 ,1 },
};

int32_t GetFloraTexureCoord(yc::world::BlockType blockType, int32_t id);

}