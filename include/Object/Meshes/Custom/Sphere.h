#ifndef SPHERE_H
#define SPHERE_H

#include "Object/Meshes/Mesh.h"

class Sphere final : public Mesh
{
public:
    explicit Sphere(unsigned int sectorCount = 36, unsigned int stackCount = 10, float radius = 1.0f);
    ~Sphere() override = default;

    void setRadius(const float radius) { m_Radius = radius; }
    [[nodiscard]] float getRadius() const { return m_Radius; }
    
protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;

private:
    unsigned int m_SectorCount = 0;
    unsigned int m_StackCount = 0;

    float m_Radius = 1.0f;
};
#endif