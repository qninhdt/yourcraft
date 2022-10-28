#pragma once

#include <glm/glm.hpp>

namespace yc::util {

bool IsIntersect(const glm::vec3& aMax, const glm::vec3& aMin,
    const glm::vec3& bMax, const glm::vec3& bMin) {

    return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
           (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
           (aMin.z <= bMax.z && aMax.z >= bMin.z);
}

bool IsInRect(const glm::vec3& point, const glm::vec3& rectMax, const glm::vec3& rectMin) {
    return (point.x >= rectMin.x && point.x <= rectMax.x &&
            point.y >= rectMin.y && point.y <= rectMax.y);
}

}