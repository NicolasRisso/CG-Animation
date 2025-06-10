#include "Meshes/Mesh.h"

#include <glm/gtc/type_ptr.hpp>

#include "Light.h"

Mesh::Mesh() = default;

Mesh::~Mesh()
{
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

bool Mesh::initialize(const std::string& texturePath, const std::string& shaderVert, const std::string& shaderFrag)
{
    m_shader = std::make_unique<Shader>(shaderVert.c_str(), shaderFrag.c_str());
    m_shader->use();

    cacheUniformLocations();

    if (!m_texture.load(texturePath)) return false;
    
    std::vector<float> vertexes;
    std::vector<unsigned int> indexes;
    
    setupMesh(vertexes, indexes);

    // Upload to GPU
    setupBuffers(vertexes, indexes);
    
    return true;
}

void Mesh::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<Light>& lights, const glm::vec3& cameraPosition)
{
    m_shader->use();

    // Send matrices
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(getModelMatrix()));
    glUniformMatrix4fv(locView,  1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(locProjection,  1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Camera Position
    glUniform3fv(locViewPosition, 1, glm::value_ptr(cameraPosition));

    // Bind diffuse texture to unit 0
    glActiveTexture(GL_TEXTURE0);
    m_texture.bind();
    m_shader->setInt("material.diffuse", 0);
    glUniform1f(locMaterialShininess, 32.0f);

    // Send Lights
    for (size_t i = 0; i < lights.size(); i++)
    {
        const Light& light = lights[i];
        glUniform3fv(locLightPosition[i],   1, glm::value_ptr(light.position));
        glUniform3fv(locLightColor[i], 1, glm::value_ptr(light.color));
        glUniform1f (locLightConst[i], light.constant);
        glUniform1f (locLightLinear[i],   light.linear);
        glUniform1f (locLightQuad[i],  light.quadratic);
    }

    // Draw
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

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

void Mesh::cacheUniformLocations()
{
    locModel   = glGetUniformLocation(m_shader->ID, "model");
    locView    = glGetUniformLocation(m_shader->ID, "view");
    locProjection    = glGetUniformLocation(m_shader->ID, "projection");
    locViewPosition = glGetUniformLocation(m_shader->ID, "viewPos");
    locMaterialShininess = glGetUniformLocation(m_shader->ID, "material.shininess");

    locLightPosition.resize(MAX_LIGHTS);
    locLightColor.resize(MAX_LIGHTS);
    locLightConst.resize(MAX_LIGHTS);
    locLightLinear.resize(MAX_LIGHTS);
    locLightQuad.resize(MAX_LIGHTS);

    char buf[64];
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        snprintf(buf, sizeof(buf), "lights[%d].position",  i);
        locLightPosition[i]   = glGetUniformLocation(m_shader->ID, buf);
        snprintf(buf, sizeof(buf), "lights[%d].color",     i);
        locLightColor[i] = glGetUniformLocation(m_shader->ID, buf);
        snprintf(buf, sizeof(buf), "lights[%d].constant",  i);
        locLightConst[i] = glGetUniformLocation(m_shader->ID, buf);
        snprintf(buf, sizeof(buf), "lights[%d].linear",    i);
        locLightLinear[i]   = glGetUniformLocation(m_shader->ID, buf);
        snprintf(buf, sizeof(buf), "lights[%d].quadratic", i);
        locLightQuad[i]  = glGetUniformLocation(m_shader->ID, buf);
    }
}
