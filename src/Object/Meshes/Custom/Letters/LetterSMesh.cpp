#include "Object/Meshes/Custom/Letters/LetterSMesh.h"

#include <glm/ext/matrix_transform.hpp>

#include "MarchingCubes/Polygonizer.h"

void LetterSMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    const glm::vec3 minCorner(-1.f, -1.f, -0.3f);
    const glm::vec3 maxCorner( 1.f,  1.f,  0.3f);
    constexpr int resolution = 196;  // aumenta para mais detalhe
    
    // Chama o marching cubes
    Polygonizer::PolygonizeSurface(
        [this](const glm::vec3& p) { return LetterSWithSDF(p); },
        minCorner, maxCorner,
        resolution,
        vertices, indices,
        false
    );

    RecalculateUVs(minCorner, maxCorner, vertices);
}

float LetterSMesh::LetterSWithSDF(const glm::vec3& p)
{
    // Parâmetros principais
    const float R      = 0.5f;   // raio externo
    const float r      = 0.25f;   // raio interno (espessura = R–r)
    const float D      = 0.3f;   // profundidade em Z
    const float shift = 0.8f;    // deslocamento vertical entre arcos
    
    // 1) Cria o “anel” (cilindro extrudado – cilindro interno)
    // glm::vec3 p1 = p + glm::vec3(0.0f, 0.25f, 0.0f);
    // float ringOuter = cappedCylinderSDF(p, R, D);
    // float ringInner = cappedCylinderSDF(p, r, D);
    // float capPlane1 = boxSDF(p1, glm::vec3(R, r, D));
    // float ring      = opSubtract(opSubtract(ringOuter, capPlane1), ringInner);

    return 1.0f;//ring;
}
