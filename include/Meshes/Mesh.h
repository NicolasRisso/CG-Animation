#ifndef MESH_H
#define MESH_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>    
#endif

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "texture.h"

class Mesh {
public:
    Mesh();
    virtual ~Mesh();

    /** @brief Inicializa a Malha com textura
     * @param texturePath Caminho para o arquivo de textura
     * @return true se a inicialização foi bem-sucedida, false caso contrário */
    bool initialize(const std::string& texturePath);

    /** @brief Renderiza a Malha
     * @param shader Shader a ser utilizado na renderização
     * @param viewMatrix Matriz de visualização
     * @param projectionMatrix Matriz de projeção */
    void render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

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

    unsigned int m_vao;          ///< Vertex Array Object
    unsigned int m_vbo;          ///< Vertex Buffer Object
    unsigned int m_ebo;          ///< Element Buffer Object
    Texture m_texture;           ///< Textura do cubo
    
    glm::vec3 m_position;        ///< Posição da Malha
    glm::vec3 m_rotation;        ///< Rotação da Malha em graus
    glm::vec3 m_scale;           ///< Escala da Malha

    unsigned int m_indexCount = 0;
};

#endif