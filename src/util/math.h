#pragma once

#include "stdint.h"

namespace yc::util {

    static int32_t PositiveMod(int32_t a, int32_t b) {
        int32_t m = a % b;

        if (m < 0) {
            return m + b;
        }

        return m;
    }
}