#include "Meshes/Mesh.h"

Mesh::Mesh() = default;

Mesh::~Mesh()
{
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

bool Mesh::initialize(const std::string& texturePath)
{
    if (!m_texture.load(texturePath)) return false;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    setupMesh(vertices, indices);
    setupBuffers(vertices, indices);
    
    return true;
}

void Mesh::render(Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    shader.use();
    shader.setMat4("model", getModelMatrix());
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);

    m_texture.bind(0);
    shader.setInt("material.diffuse", 0);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    m_texture.unbind();
}

glm::mat4 Mesh::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, m_scale);
    return model;
}

void Mesh::setupBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
    m_indexCount = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Layout: Position (3 Floats), Normal (3 Floats), Texture Coord (2 Floats).
    constexpr GLsizei stride = 8 * sizeof(float);

    // Posição - Layout 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // Normal - Layout 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coord - Layout 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}