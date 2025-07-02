#include "Object/Meshes/Custom/Letters/LetterCMesh.h"

#include "MarchingCubes/Polygonizer.h"

void LetterCMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    const glm::vec3 minCorner(-0.7f, -0.7f, -0.3f);
    const glm::vec3 maxCorner( 0.7f,  0.7f,  0.3f);
    constexpr int resolution =  196;  // aumenta para mais detalhe
    
    // Chama o marching cubes
    Polygonizer::PolygonizeSurface(
        [this](const glm::vec3& p) { return LetterCWithSDF(p); },
        minCorner, maxCorner,
        resolution,
        vertices, indices,
        false
    );

    RecalculateUVs(minCorner, maxCorner, vertices);
}

float LetterCMesh::LetterCWithSDF(const glm::vec3& p)
{
    // 1) Cilindro maior e interno
    float outer = cylinderSDF(p, {0,0,-0.3f}, {0,0, 0.3f}, 0.65f);
    float inner = cylinderSDF(p, {0,0,-0.3f}, {0,0, 0.3f}, 0.45f);
    float ring  = opSubtract(outer, inner);

    // 2) Plano de corte vertical (X = cutX) — mantém p.x ≤ cutX
    constexpr float cutX =  0.3f;  
    const float planeX   = planeSDF(p, {1,0,0}, cutX);

    // 3) Plano de corte horizontal (Y = cutY) — mantém p.y ≥ cutY
    //    se preferir cortar em p.y ≤ cutY, basta inverter o offset:
    constexpr float cutZ = 0.2f;
    const float planeZ   = planeSDF(p, {0,0,1}, cutZ);
    const float planeNZ = planeSDF(p, {0,0,-1}, cutZ);

    // 4) Interseções em cadeia: ring ∩ X‐meio‐espaço ∩ Y‐meio‐espaço
    const float cutAlongX = opIntersection(ring, planeX);
    const float cutAlongZ = opIntersection(cutAlongX, planeZ);
    return opIntersection(cutAlongZ, planeNZ);
}
