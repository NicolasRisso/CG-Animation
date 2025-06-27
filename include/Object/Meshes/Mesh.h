#ifndef MESH_H
#define MESH_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
#endif

#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include "Object/Core/Material.h"

class Mesh {
public:
    Mesh();
    virtual ~Mesh();

    /** @brief Inicializa a Malha com textura
     * @param texturePath Caminho para o arquivo de textura
     * @param shaderVert Caminho para o .vs do Shader
     * @param shaderFrag Caminho para o .fs do Shader
     * @return true se a inicialização foi bem-sucedida, false caso contrário */
    bool initialize();

    /** @brief Renderiza a Malha
     * @param viewMatrix Matriz de visualização
     * @param projectionMatrix Matriz de projeção */
    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix, const std::vector<class Light>& lights, const glm::vec3& cameraPosition);

    void setMaterial(const Material& newMaterial) { m_Material = newMaterial; }
    [[nodiscard]] Material getMaterial() const { return m_Material; }

protected:
    /** Cada subclasse da Malha deve preencher seus próprios vértices e índices usando essa função. */
    virtual void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) = 0;

    /** @brief Configura os buffers de vértices e índices. (Gera VAO, VBO e EBO) */
    void setupBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

private:
    void cacheUniformLocations();

    Material m_Material;
    
    // Vertex Array Object, Vertex Buffer Object, Element Buffer Object
    unsigned int m_vao = 0, m_vbo = 0, m_ebo = 0, m_indexCount = 0;
    
    // Cached uniform locations
    GLint locModel, locView, locProjection, locViewPosition;
    GLint locMaterialShininess, locMaterialDiffuse;
    std::vector<GLint> locLightPosition, locLightColor, locLightConst, locLightLinear, locLightQuad;

};
#endif