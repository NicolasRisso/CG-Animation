#include "cube.h"
#include <vector>

void Cube::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    // cada vértice: pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, tex.u, tex.v
    vertices = {
        // back face
        -0.5f, -0.5f, -0.5f,  0,  0, -1,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0,  0, -1,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0,  0, -1,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0,  0, -1,  0.0f, 1.0f,
         // front face
        -0.5f, -0.5f,  0.5f,  0,  0,  1,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0,  0,  1,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0,  0,  1,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0,  0,  1,  0.0f, 1.0f,
         // left face
        -0.5f,  0.5f,  0.5f, -1,  0,  0,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1,  0,  0,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1,  0,  0,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1,  0,  0,  0.0f, 0.0f,
         // right face
         0.5f,  0.5f,  0.5f,  1,  0,  0,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1,  0,  0,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1,  0,  0,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1,  0,  0,  0.0f, 0.0f,
         // bottom face
        -0.5f, -0.5f, -0.5f,  0, -1,  0,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0, -1,  0,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0, -1,  0,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0, -1,  0,  0.0f, 0.0f,
         // top face
        -0.5f,  0.5f, -0.5f,  0,  1,  0,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0,  1,  0,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0,  1,  0,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0,  1,  0,  0.0f, 0.0f
     };

    indices = {
        0, 3, 2, 2, 1, 0,         // back
        4, 5, 6, 6, 7, 4,         // front (corrected)
        8, 9,10,10,11, 8,         // left
       12,13,14,14,15,12,         // right (corrected)
       16,17,18,18,19,16,         // bottom
       20,22,21,22,20,23          // top (corrected)
    };
}
