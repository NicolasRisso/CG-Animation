#pragma once
#include "Object/Meshes/Mesh.h"

class LetterAMesh : public Mesh
{
public:
    explicit LetterAMesh(const float width = 1.0f, const float height = 1.0f, const float barThickness = 0.1f, const float barHeightRatio = 0.5f)
        : m_Width(width), m_Height(height), m_BarThickness(barThickness), m_BarHeightRatio(barHeightRatio) {}
    ~LetterAMesh() override = default;

protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;
    
private:
    float m_Width, m_Height, m_BarThickness, m_BarHeightRatio;
};