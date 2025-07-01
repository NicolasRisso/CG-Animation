#pragma once

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <algorithm>

/** Constructive Solid Geometry Implementable */
class CSGImplementable
{
protected:
    static float opUnion(float a, float b) {
        return std::min(a, b);
    }

    static float opIntersection(float a, float b) {
        return std::max(a, b);
    }

    static float opSubtract(float a, float b) {
        return std::max(a, -b);
    }

    static float cylinderSDF(const glm::vec3& p,
                      const glm::vec3& a,
                      const glm::vec3& b,
                      const float r)
    {
        const glm::vec3 pa = p - a;
        const glm::vec3 ba = b - a;
        const float h = glm::clamp(glm::dot(pa, ba) / glm::dot(ba, ba), 0.0f, 1.0f);
        return glm::length(pa - ba * h) - r;
    }
};
