#include "Object/Meshes/Custom/Letters/LetterHMesh.h"

#include "MarchingCubes/Polygonizer.h"

void LetterHMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    const glm::vec3 minCorner(-0.5f, -0.625f, -0.3f);
    const glm::vec3 maxCorner( 0.5f,  0.625f,  0.3f);
    constexpr int resolution =  64;  // aumenta para mais detalhe
    
    // Chama o marching cubes
    Polygonizer::PolygonizeSurface(
        [this](const glm::vec3& p) { return LetterHWithSDF(p); },
        minCorner, maxCorner,
        resolution,
        vertices, indices,
        false
    );

    RecalculateUVs(minCorner, maxCorner, vertices);
}

float LetterHMesh::LetterHWithSDF(const glm::vec3& p)
{
    const float base = boxSDF(p, glm::vec3(0.5f, 0.625f, 0.1f));

    const float cut1 = boxSDF(p - glm::vec3(0.0f, +0.3625f, 0.0f), glm::vec3(0.3f,  0.2625f,  0.1f));
    const float cut2 = boxSDF(p - glm::vec3(0.0f, -0.3625f, 0.0f), glm::vec3(0.3f,  0.2625f,  0.1f));  

    const float h = opSubtract(opSubtract(base, cut1), cut2);
    return h;
}
