#ifndef MESH_H
#define MESH_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
#endif

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory> // Added for std::unique_ptr
#include "shader.h"
#include "texture.h"

class Mesh {
public:
    Mesh();
    virtual ~Mesh();

    /** @brief Inicializa a Malha com textura
     * @param texturePath Caminho para o arquivo de textura
     * @param shaderVert Caminho para o .vs do Shader
     * @param shaderFrag Caminho para o .fs do Shader
     * @return true se a inicialização foi bem-sucedida, false caso contrário */
    bool initialize(const std::string& texturePath, const std::string& shaderVert = "Shaders/default.vs", const std::string& shaderFrag = "Shaders/default.fs");

    /** @brief Renderiza a Malha
     * @param shader Shader a ser utilizado na renderização
     * @param viewMatrix Matriz de visualização
     * @param projectionMatrix Matriz de projeção */
    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<class Light>& lights, const glm::vec3& cameraPosition);

    /** @brief Atualiza a posição da Malha
     * @param position Nova posição */
    void setPosition(const glm::vec3& position) { m_position = position; }
    
    /** @brief Atualiza a rotação da Malha
     * @param rotation Nova rotação em graus (x, y, z) */
    void setRotation(const glm::vec3& rotation) { m_rotation = rotation; }
    
    /** @brief Atualiza a escala da Malha
     * @param scale Nova escala (x, y, z) */
    void setScale(const glm::vec3& scale) { m_scale = scale; }
    
    /** @brief Obtém a posição atual da Malha
     * @return Posição atual */
    glm::vec3 getPosition() const { return m_position; }
    
    /** @brief Obtém a rotação atual da Malha
     * @return Rotação atual em graus */
    glm::vec3 getRotation() const { return m_rotation; }
    
    /** @brief Obtém a escala atual da Malha
     * @return Escala atual */
    glm::vec3 getScale() const { return m_scale; }
    
    /** @brief Obtém a matriz de modelo da Malha
     * @return Matriz de modelo */
    glm::mat4 getModelMatrix() const;

protected:
    /** Cada subclasse da Malha deve preencher seus próprios vértices e índices usando essa função. */
    virtual void setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices) = 0;

    /** @brief Configura os buffers de vértices e índices. (Gera VAO, VBO e EBO) */
    void setupBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

private:
    void cacheUniformLocations();

    // Vertex Array Object, Vertex Buffer Object, Element Buffer Object
    unsigned int m_vao = 0, m_vbo = 0, m_ebo = 0, m_indexCount = 0;
    glm::vec3 m_position{0.0f}, m_rotation{0.0f}, m_scale{0.0f};
    Texture m_texture;
    std::unique_ptr<Shader> m_shader;

    // Cached uniform locations
    GLint locModel, locView, locProjection, locViewPosition;
    GLint locMaterialShininess;
    std::vector<GLint> locLightPosition, locLightColor, locLightConst, locLightLinear, locLightQuad;

    // REMOVE REMOVE:
    const int MAX_LIGHTS = 4;
};

#endif