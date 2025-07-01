#pragma once

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <algorithm>
#include <vector>

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

    static float planeSDF(const glm::vec3& p, const glm::vec3& n, const float offset)
    {
        return glm::dot(p, n) - offset;
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

    static void RecalculateUVs(const glm::vec3 minCorner, const glm::vec3 maxCorner, std::vector<float>& vertices)
    {
        for(size_t i = 0; i < vertices.size(); i += 8)
        {
            const glm::vec3 p{ vertices[i+0], vertices[i+1], vertices[i+2] };
            const glm::vec3 n{ vertices[i+3], vertices[i+4], vertices[i+5] };
            const float ax = fabs(n.x);
            const float ay = fabs(n.y);
            const float az = fabs(n.z);
            float u,v;

            if (az >= ax && az >= ay) {
                // face “topo” ou “base”: projeta em XY
                u = (p.x - minCorner.x)/(maxCorner.x - minCorner.x);
                v = (p.y - minCorner.y)/(maxCorner.y - minCorner.y);
            }
            else if (ax >= ay && ax >= az) {
                // face de “lado” em X: projeta em ZY
                u = (p.z - minCorner.z)/(maxCorner.z - minCorner.z);
                v = (p.y - minCorner.y)/(maxCorner.y - minCorner.y);
            }
            else {
                // face de “lado” em Y: projeta em XZ
                u = (p.x - minCorner.x)/(maxCorner.x - minCorner.x);
                v = (p.z - minCorner.z)/(maxCorner.z - minCorner.z);
            }

            vertices[i+6] = u;
            vertices[i+7] = v;
        }
    }
};
