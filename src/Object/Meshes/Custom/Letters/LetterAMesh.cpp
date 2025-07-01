#include "Object/Meshes/Custom/Letters/LetterAMesh.h"

#include "MarchingCubes/Polygonizer.h"

void LetterAMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    const glm::vec3 minCorner(-1.f, -1.25f, -0.3f);
    const glm::vec3 maxCorner( 1.f,  1.25f,  0.3f);
    constexpr int resolution =  120;
    
    // Chama o marching cubes
    Polygonizer::PolygonizeSurface(
        [this](const glm::vec3& p) { return LetterAWithSDF(p); },
        minCorner, maxCorner,
        resolution,
        vertices, indices,
        false
    );

    RecalculateUVs(minCorner, maxCorner, vertices);
}

float LetterAMesh::LetterAWithSDF(const glm::vec3& p)
{
    constexpr float height = 1.2f;
    const float body = truncatedPrismSDF(p + glm::vec3(0.0f, height * 0.5f, 0.0f), {0.5f, 0.3f}, {0.2f, 0.3f}, height);

    const float lowerCut = truncatedPrismSDF(p + glm::vec3(0.0f, height * 0.5f, 0.0f), {0.25f, 0.3f}, {0.15f, 0.3f}, 0.35f);
    const float upperCut = truncatedPrismSDF(p + glm::vec3(0.0f, height * 0.5f - 0.525f, 0.0f), {0.125f, 0.3f}, {0.05f, 0.3f}, 0.45f);
    
    return opSubtract(opSubtract(body, lowerCut), upperCut);
}
