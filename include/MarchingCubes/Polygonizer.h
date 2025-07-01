#pragma once

#include <vector>
#include <functional>
#include <glm/vec3.hpp>

namespace Polygonizer
{
    void PolygonizeSurface(const std::function<float(const glm::vec3&)>& sdf,
        const glm::vec3& minCorner,
        const glm::vec3& maxCorner,
        int resolution,
        std::vector<float>& outVerts,
        std::vector<unsigned int>& outIdx
    );    
}
