#ifndef CUBE_H
#define CUBE_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #define NOMINMAX  // Evita conflitos com min/max do Windows
    #define WIN32_LEAN_AND_MEAN  // Reduz inclusões do Windows.h
    #include <windows.h>
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "texture.h"
#include "Meshes/Mesh.h"

/**
 * @class Cube
 * @brief Classe para representação e renderização de um cubo 3D
 */
class Cube : public Mesh{
public:
    Cube() = default;
    virtual ~Cube() = default;
    
protected:
    void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) override;
    
};

#endif // CUBE_H
