#pragma once

#include "stdint.h"
#include <cmath>

namespace yc::util {

    static int32_t PositiveMod(int32_t a, int32_t b) {
        int32_t m = a % b;

        if (m < 0) {
            return m + b;
        }

        return m;
    }

    static int32_t SignNum(float num) {
        return num>0 ? 1 : num<0 ? -1 : 0;
    }

     static float IntBound(float s, float ds) {
        return (ds>0 ? ceil(s)-s : s-floor(s))/abs(ds);
     }
}