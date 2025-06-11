#include "Object/Meshes/Custom/Sphere.h"

#include <vector>

#include "Utility/Constants/MathConsts.h"

Sphere::Sphere(const unsigned int sectorCount, const unsigned int stackCount, const float radius)
    : m_SectorCount(sectorCount), m_StackCount(stackCount), m_Radius(radius){}

void Sphere::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    for (unsigned int i = 0; i <= m_StackCount; ++i)
    {
        const float stackAngle = MathConstants::PI / 2.0f - i * MathConstants::PI / m_StackCount;
        const float xy = cosf(stackAngle);
        float z = sinf(stackAngle);

        for (unsigned int j = 0; j <= m_SectorCount; ++j)
        {
            const float sectorAngle = j * 2.0f * MathConstants::PI / m_SectorCount;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            // Position
            vertices.push_back(x * m_Radius);
            vertices.push_back(y * m_Radius);
            vertices.push_back(z * m_Radius);

            // Normals
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // UV
            vertices.push_back(static_cast<float>(j) / static_cast<float>(m_SectorCount));
            vertices.push_back(static_cast<float>(i) / static_cast<float>(m_StackCount));
        }
    }

    for (unsigned int i = 0; i < m_StackCount; ++i)
    {
        for (unsigned int j = 0; j < m_SectorCount; ++j)
        {
            unsigned int first = i * (m_SectorCount + 1) + j;
            unsigned int second = (i + 1) * (m_SectorCount + 1) + j;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

