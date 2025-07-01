#include "Object/Meshes/Custom/Letters/LetterCMesh.h"

#include "MarchingCubes/Polygonizer.h"

void LetterCMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    glm::vec3 minCorner(-1.1f, -1.1f, -0.35f);
    glm::vec3 maxCorner( 1.1f,  1.1f,  0.35f);
    int resolution =  128;  // aumenta para mais detalhe

    auto sphereSDF = [](const glm::vec3& p) {
        return glm::length(p) - 1.0f;  // esfera de raio 1 centrada em 0
    };
    
    // Chama o marching cubes
    Polygonizer::PolygonizeSurface(
        [this](const glm::vec3& p) { return LetterCWithSDF(p); },
        minCorner, maxCorner,
        resolution,
        vertices, indices
    );
}

float LetterCMesh::LetterCWithSDF(const glm::vec3& p)
{
    // 1) Cilindro maior (anel externo)
    float outer = cylinderSDF(p,
                              glm::vec3(0,0,-0.3f),
                              glm::vec3(0,0, 0.3f),
                              1.0f);
    // 2) Cilindro menor (anel interno)
    float inner = cylinderSDF(p,
                              glm::vec3(0,0,-0.3f),
                              glm::vec3(0,0, 0.3f),
                              0.7f);
    // cria um anel
    float ring = opSubtract(outer, inner);

    // 3) Plano para cortar o lado direito e formar o "C"
    float d_plane = 0.3f - p.x;  // keep points with x > -0.3
    // Interseção entre o anel e o semiespaço do plano
    return opIntersection(ring, d_plane);
}
