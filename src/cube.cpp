#include "cube.h"
#include <vector>

Cube::Cube()
    : m_vao(0), m_vbo(0), m_ebo(0),
      m_position(0.0f, 0.0f, 0.0f),
      m_rotation(0.0f, 0.0f, 0.0f),
      m_scale(1.0f, 1.0f, 1.0f)
{
}

Cube::~Cube()
{
    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }
}

bool Cube::initialize(const std::string& texturePath)
{
    // Carregar textura
    if (!m_texture.load(texturePath))
    {
        return false;
    }
    
    // Configurar buffers
    setupBuffers();
    
    return true;
}

void Cube::render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    // Ativar shader
    shader.use();
    
    // Configurar matrizes
    glm::mat4 modelMatrix = getModelMatrix();
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
    
    // Ativar textura
    m_texture.bind(0);
    shader.setInt("material.diffuse", 0);
    
    // Renderizar cubo
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    // Desativar textura
    m_texture.unbind();
}

void Cube::setPosition(const glm::vec3& position)
{
    m_position = position;
}

void Cube::setRotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
}

void Cube::setScale(const glm::vec3& scale)
{
    m_scale = scale;
}

glm::vec3 Cube::getPosition() const
{
    return m_position;
}

glm::vec3 Cube::getRotation() const
{
    return m_rotation;
}

glm::vec3 Cube::getScale() const
{
    return m_scale;
}

glm::mat4 Cube::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    
    // Aplicar transformações na ordem: escala -> rotação -> translação
    model = glm::translate(model, m_position);
    
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    model = glm::scale(model, m_scale);
    
    return model;
}

void Cube::setupBuffers()
{
    // Vértices do cubo com posições, normais e coordenadas de textura
    float vertices[] = {
        // Posições            // Normais             // Coords de Textura
        // Face frontal
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
        
        // Face traseira
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        
        // Face esquerda
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        
        // Face direita
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        
        // Face inferior
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        
        // Face superior
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f
    };
    
    // Índices para desenhar o cubo usando triângulos
    unsigned int indices[] = {
        // Face frontal
        0, 1, 2,
        2, 3, 0,
        
        // Face traseira
        4, 5, 6,
        6, 7, 4,
        
        // Face esquerda
        8, 9, 10,
        10, 11, 8,
        
        // Face direita
        12, 13, 14,
        14, 15, 12,
        
        // Face inferior
        16, 17, 18,
        18, 19, 16,
        
        // Face superior
        20, 21, 22,
        22, 23, 20
    };
    
    // Criar VAO, VBO e EBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    
    // Vincular VAO
    glBindVertexArray(m_vao);
    
    // Configurar VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Configurar EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Configurar atributos de vértice
    // Posições
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normais
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Coordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Desvincular VAO
    glBindVertexArray(0);
}
