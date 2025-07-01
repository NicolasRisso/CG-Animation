#include "Object/Meshes/Custom/Letters/LetterOMesh.h"

#include "MarchingCubes/Polygonizer.h"

void LetterOMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    const glm::vec3 minCorner(-1.f, -1.f, -0.3f);
    const glm::vec3 maxCorner( 1.f,  1.f,  0.3f);
    constexpr int resolution =  150;  // aumenta para mais detalhe
    
    // Chama o marching cubes
    Polygonizer::PolygonizeSurface(
        [this](const glm::vec3& p) { return LetterOWithSDF(p); },
        minCorner, maxCorner,
        resolution,
        vertices, indices,
        false
    );

    RecalculateUVs(minCorner, maxCorner, vertices);
}

float LetterOMesh::LetterOWithSDF(const glm::vec3& p)
{
    // 1) Cilindro maior e interno
    float outer = cappedEllipticalCylinderSDF(p, 0.5f, 0.625f, 0.3f);
    float inner = cappedEllipticalCylinderSDF(p, 0.3f, 0.375, 0.3f);
    float ring  = opSubtract(outer, inner);

    // 2) Plano de corte horizontal (Y = cutY) — mantém p.y ≥ cutY
    //    se preferir cortar em p.y ≤ cutY, basta inverter o offset:
    constexpr float cutZ = 0.2f;
    const float planeZ   = planeSDF(p, {0,0,1}, cutZ);
    const float planeNZ = planeSDF(p, {0,0,-1}, cutZ);

    // 3) Interseções em cadeia: ring ∩ X‐meio‐espaço ∩ Y‐meio‐espaço
    const float cutAlongZ = opIntersection(ring, planeZ);
    return opIntersection(cutAlongZ, planeNZ);
}
