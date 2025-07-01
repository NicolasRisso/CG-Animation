#include "Object/Meshes/Custom/Letters/LetterNMesh.h"

#include "MarchingCubes/Polygonizer.h"

void LetterNMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    const glm::vec3 minCorner(-1.f, -1.f, -0.3f);
    const glm::vec3 maxCorner( 1.f,  1.f,  0.3f);
    constexpr int resolution = 96;  // aumenta para mais detalhe
    
    // Chama o marching cubes
    Polygonizer::PolygonizeSurface(
        [this](const glm::vec3& p) { return LetterNWithSDF(p); },
        minCorner, maxCorner,
        resolution,
        vertices, indices,
        false
    );

    RecalculateUVs(minCorner, maxCorner, vertices);
}

float LetterNMesh::LetterNWithSDF(const glm::vec3& p)
{
    constexpr float height = 1.2f;
    const float body = boxSDF(p, glm::vec3(0.5f, height*0.5f, 0.1f));

    const float cut1 = wedgeSDF(p, 0.3f, 0.3f, 0.4f);

    const float lowerCut = truncatedPrismSDF(p + glm::vec3(0.0f, height * 0.5f, 0.0f), {0.25f, 0.3f}, {0.15f, 0.3f}, 0.35f);
    const float upperCut = truncatedPrismSDF(p + glm::vec3(0.0f, height * 0.5f - 0.525f, 0.0f), {0.125f, 0.3f}, {0.05f, 0.3f}, 0.45f);
    
    return cut1;
}
