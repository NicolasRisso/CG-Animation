#include "Object/Meshes/Custom/Letters/LetterNMesh.h"

#include <glm/ext/matrix_transform.hpp>

#include "MarchingCubes/Polygonizer.h"

void LetterNMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // Define o volume de amostragem
    const glm::vec3 minCorner(-1.f, -1.f, -0.3f);
    const glm::vec3 maxCorner( 1.f,  1.f,  0.3f);
    constexpr int resolution = 128;  // aumenta para mais detalhe
    
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
    glm::mat4 CW_Rot = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(0,0,1));
    glm::mat4 CCW_Rot = glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(0,0,1));
    glm::vec3 CW_p = glm::vec3(CW_Rot * glm::vec4(p - glm::vec3(0.05f, 0.2f, 0.0f),1.0f));
    glm::vec3 CCW_p = glm::vec3(CCW_Rot * glm::vec4(p + glm::vec3(0.05f, 0.2f, 0.0f),1.0f));
    
    constexpr float height = 1.2f;
    const float body = boxSDF(p, glm::vec3(0.5f, height*0.5f, 0.1f));

    const float cut1 = wedgeSDF(CCW_p, 0.45f, 0.3f, 0.2f);
    const float cut2 = wedgeSDF(CW_p, 0.45f, 0.3f, 0.2f);
    
    return opSubtract(opSubtract(body, cut1), cut2);
}
