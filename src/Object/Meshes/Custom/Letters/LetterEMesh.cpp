#include "Object/Meshes/Custom/Letters/LetterEMesh.h"

#include "MarchingCubes/Polygonizer.h"

void LetterEMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    const glm::vec3 minCorner(-0.5f, -0.625f, -0.3f);
    const glm::vec3 maxCorner( 0.5f,  0.625f,  0.3f);
    constexpr int resolution =  64;  // aumenta para mais detalhe
    
    // Chama o marching cubes
    Polygonizer::PolygonizeSurface(
        [this](const glm::vec3& p) { return LetterEWithSDF(p); },
        minCorner, maxCorner,
        resolution,
        vertices, indices,
        false
    );

    RecalculateUVs(minCorner, maxCorner, vertices);
}

float LetterEMesh::LetterEWithSDF(const glm::vec3& p)
{
    const float base = boxSDF(p, glm::vec3(0.5f, 0.625f, 0.1f));

    const float cut1 = boxSDF(p - glm::vec3(0.2f, 0.265f, 0.0f), glm::vec3(0.4f, 0.15f, 0.12f));
    const float cut2 = boxSDF(p - glm::vec3(0.2f, -0.265f, 0.0f), glm::vec3(0.4f, 0.15f, 0.12f));

    const float e = opSubtract(opSubtract(base, cut1), cut2);
    return e;
}
