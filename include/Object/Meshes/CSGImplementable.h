#pragma once

#include <algorithm>
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

    static float boxSDF(const glm::vec3& p, const glm::vec3& b)
    {
        const glm::vec3 d = glm::abs(p) - b;
        return glm::length(glm::max(d, 0.0f)) + std::min(std::max({d.x, d.y, d.z}), 0.0f);
    }

    static float rightTrianglePrismSDF(const glm::vec3& p, const float legX, const float legY, const float depth)
    {
        const float dz = fabs(p.z) - depth*0.5f;
        
        const float d1 = -p.x;
        const float d2 = -p.y;
        const float d3 = p.y + (legY/legX)*p.x - legY;

        const float d2D = std::max(std::max(d1, d2), d3);
        
        return std::max(d2D, dz);
    }
    
    static float cylinderSDF(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const float r)
    {
        const glm::vec3 pa = p - a;
        const glm::vec3 ba = b - a;
        const float h = glm::clamp(glm::dot(pa, ba) / glm::dot(ba, ba), 0.0f, 1.0f);
        return glm::length(pa - ba * h) - r;
    }

    static float ellipticalCylinderSDF(const glm::vec3& p, const float rx, const float ry)
    {
        const float d = sqrt((p.x* p.x)/(rx*rx) + (p.y*p.y)/(ry*ry)) - 1.0f;
        return d;
    }

    static float cappedEllipticalCylinderSDF(const glm::vec3& p, const float rx, const float ry, const float depth)
    {
        const float d1 = sqrt((p.x*p.x)/(rx*rx) + (p.y*p.y)/(ry*ry)) - 1.0f;
        const float d2 = fabs(p.z) - depth*0.5f;
        return std::max(d1, d2);
    }

    static float truncatedPrismSDF(const glm::vec3& p, const glm::vec2& base, const glm::vec2& top, const float height){
        // 1) clampa e t ∈ [0,1]
        const float yClamped = glm::clamp(p.y, 0.0f, height);
        const float t        = yClamped / height;

        // 2) interpola semi-extensões em XZ
        const glm::vec2 half = glm::mix(base, top, t);

        // 3) SDF 2D no plano XZ (caixa retangular)
        const glm::vec2 d = glm::abs(glm::vec2(p.x, p.z)) - half;
        const float outsideDist = glm::length(glm::max(d, glm::vec2(0.0f)));
        const float insideDist  = std::min(std::max(d.x, d.y), 0.0f);
        const float dist2D      = outsideDist + insideDist;

        // 4) SDF em Y (caps planas em y=0 e y=height)
        const float dy = fabs(p.y - height * 0.5f) - (height * 0.5f);

        // 5) interseção 2D∩Y
        return std::max(dist2D, dy);
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
