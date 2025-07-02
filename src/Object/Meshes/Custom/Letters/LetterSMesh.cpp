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
    const float H        = 1.2f;
    const float halfH    = 0.2f;
    // raio do seu cilindro (escolha o que precisar)
    const float R        = 0.45f;

    // pivôs em Z para bottom e top
    glm::vec3 A_bot{ 0, 0, -halfH};  // base inferior em z = –0.6
    glm::vec3 B_bot{ 0, 0, halfH };  // topo inferior em z =  0.0

    glm::vec3 A_top{ 0, 0, -halfH };  // base superior em z =  0.0
    glm::vec3 B_top{ 0, 0, halfH};  // topo   superior em z = +0.6

    glm::vec3 offset{0, 0.345f, 0.0f};
    glm::vec3 offsetBox{0.225f, 0.0f, 0.0f};
    
    // cilindro inferior (deitado, circular no plano XY, z ∈ [–0.6,0])
    float cylBottom = cappedCylinderSDF(p + offset, A_bot, B_bot, R);
    float minorCylBottom = cappedCylinderSDF(p + offset, A_bot, B_bot, R*0.5f);
    float boxBottom = boxSDF(p + offsetBox, glm::vec3(0.225f, 0.225f, 0.3f));
    float sBottom = opSubtract( opSubtract(cylBottom, minorCylBottom), boxBottom);

    // cilindro superior (z ∈ [0,+0.6])
    float cylTop    = cappedCylinderSDF(p - offset, A_top, B_top, R);
    float minorCylTop = cappedCylinderSDF(p - offset, A_bot, B_bot, R*0.5f);
    float boxTop = boxSDF(p - offsetBox, glm::vec3(0.225f, 0.225f, 0.3f));
    float sTop = opSubtract( opSubtract(cylTop, minorCylTop), boxTop);

    glm::vec3 connHalf{ 0.05f, R * 0.25f, halfH};
    float connector = boxSDF(p, connHalf);

    return opUnion(opUnion(sBottom, sTop), connector);
}
