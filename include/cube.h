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

/**
 * @class Cube
 * @brief Classe para representação e renderização de um cubo 3D
 */
class Cube {
public:
    /**
     * @brief Construtor padrão
     */
    Cube();
    
    /**
     * @brief Destrutor
     */
    ~Cube();
    
    /**
     * @brief Inicializa o cubo com textura
     * @param texturePath Caminho para o arquivo de textura
     * @return true se a inicialização foi bem-sucedida, false caso contrário
     */
    bool initialize(const std::string& texturePath);
    
    /**
     * @brief Renderiza o cubo
     * @param shader Shader a ser utilizado na renderização
     * @param viewMatrix Matriz de visualização
     * @param projectionMatrix Matriz de projeção
     */
    void render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    
    /**
     * @brief Atualiza a posição do cubo
     * @param position Nova posição
     */
    void setPosition(const glm::vec3& position);
    
    /**
     * @brief Atualiza a rotação do cubo
     * @param rotation Nova rotação em graus (x, y, z)
     */
    void setRotation(const glm::vec3& rotation);
    
    /**
     * @brief Atualiza a escala do cubo
     * @param scale Nova escala (x, y, z)
     */
    void setScale(const glm::vec3& scale);
    
    /**
     * @brief Obtém a posição atual do cubo
     * @return Posição atual
     */
    glm::vec3 getPosition() const;
    
    /**
     * @brief Obtém a rotação atual do cubo
     * @return Rotação atual em graus
     */
    glm::vec3 getRotation() const;
    
    /**
     * @brief Obtém a escala atual do cubo
     * @return Escala atual
     */
    glm::vec3 getScale() const;
    
    /**
     * @brief Obtém a matriz de modelo do cubo
     * @return Matriz de modelo
     */
    glm::mat4 getModelMatrix() const;

private:
    unsigned int m_vao;          ///< Vertex Array Object
    unsigned int m_vbo;          ///< Vertex Buffer Object
    unsigned int m_ebo;          ///< Element Buffer Object
    Texture m_texture;           ///< Textura do cubo
    
    glm::vec3 m_position;        ///< Posição do cubo
    glm::vec3 m_rotation;        ///< Rotação do cubo em graus
    glm::vec3 m_scale;           ///< Escala do cubo
    
    /**
     * @brief Configura os buffers de vértices e índices
     */
    void setupBuffers();
};

#endif // CUBE_H
