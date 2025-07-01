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

    static float rotatedRightTriangleSDF(
    const glm::vec3& p, const glm::vec2& baseXZ, const float heightY, const float angleDeg = 0.0f)
    {
        // —————— 1) rotaciona o triângulo no plano XZ ——————
        float a = glm::radians(angleDeg);
        float ca = std::cos(a), sa = std::sin(a);
        // gira (x,z) → ( x·cos – z·sin, x·sin + z·cos )
        glm::vec3 q = p;
        q.x = p.x * ca - p.z * sa;
        q.z = p.x * sa + p.z * ca;
        // q.y = p.y fica igual

        // —————— 2) limita em Y (depth) ——————
        float halfY = heightY * 0.5f;
        float dY    = std::fabs(q.y) - halfY;           // dentro se |y| ≤ halfY

        // —————— 3) half‐spaces do triângulo no plano XZ ——————
        // (a) x ≥ 0  → d1 = –x
        float d1 = -q.x;
        // (b) z ≥ 0  → d2 = –z
        float d2 = -q.z;
        // (c) hipotenusa ligando (baseX,0) a (0,baseZ):
        //     z ≤ –(baseZ/baseX)*x + baseZ
        //     → d3 = z + (baseZ/baseX)*x – baseZ
        float slope = baseXZ.y / baseXZ.x;
        float d3    = q.z + slope * q.x - baseXZ.y;

        // SDF 2D do triângulo = intersecção dos três half‐spaces
        float d2D = std::max(std::max(d1, d2), d3);

        // —————— 4) interseção triângulo ∩ extrusão Y ——————
        return std::max(d2D, dY);
    }

    static float rightTrianglePrismSDF(const glm::vec3& p, const glm::vec2& baseXZ, const float heightY )
    {
        float halfY = heightY * 0.5f;
        float dY    = fabs(p.y) - halfY;

        // 2) Três half‐spaces do triângulo no plano XZ:

        // (a) x >= 0  → dentro se -p.x <= 0
        float d1 = -p.x;

        // (b) z >= 0  → dentro se -p.z <= 0
        float d2 = -p.z;

        // (c) hipotenusa ligando (baseX,0) a (0,baseZ):
        //     equação: z = −(baseZ/baseX) * x + baseZ
        //     → dentro se p.z + (baseZ/baseX)*p.x − baseZ ≤ 0
        float ratio = baseXZ.y / baseXZ.x;            // baseZ/baseX
        float d3    = p.z + ratio * p.x - baseXZ.y;

        // 3) SDF 2D do triângulo: interseção dos três half‐spaces
        float d2D = std::max(std::max(d1, d2), d3);

        // 4) Combina 2D + extrusão em Y (CSG intersection = max)
        return std::max(d2D, dY);
    }

    static float boxExtrudedSDF(const glm::vec3& p,
                           float hx, float hz, float heightY)
    {
        // move p para o “primeiro octante” (espelha X e Z)
        glm::vec3 q(fabs(p.x), p.y, fabs(p.z));
        glm::vec3 b(hx, heightY, hz);

        glm::vec3 d = q - b;
        float outside = glm::length(glm::max(d, glm::vec3(0.0f)));
        float inside  = std::min(std::max(d.x, std::max(d.y, d.z)), 0.0f);

        return outside + inside;
    }

    // 2) SDF do plano de corte oblíquo, extrudado em Z
    //    – o plano em XY tem normal n = normalize( ( heightY, -hx, 0 ) )
    //    – offset = dot(n, A), onde A = (hx, heightY, 0) é o ápice
    static float slopePlaneSDF(const glm::vec3& p, float hx, float heightY)
    {
        glm::vec3 n = glm::normalize(glm::vec3(heightY, -hx, 0.0f));
        float c     = glm::dot(n, glm::vec3(hx, heightY, 0.0f));
        // dentro do wedge quando n·p <= c
        return glm::dot(n, p) - c;
    }

    // 3) o wedge é a interseção (CSG ∩) desses dois SDFs:
    static float wedgeSDF(const glm::vec3& p, float hx, float hz, float heightY)
    {
        float dBox   = boxExtrudedSDF(p, hx, hz, heightY);
        float dPlane = slopePlaneSDF(p, hx, heightY);
        // intersection = max(box, plane)
        return std::max(dBox, dPlane);
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
