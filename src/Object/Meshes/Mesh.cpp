#include "Object/Meshes/Mesh.h"

#include <glm/gtc/type_ptr.hpp>

#include "Light.h"
#include "Utility/Constants/EngineLimits.h"

Mesh::Mesh()
    : m_Material{Material()}, locModel(0), locView(0), locProjection(0), locViewPosition(0), locMaterialShininess(0) {}

Mesh::~Mesh()
{
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

bool Mesh::initialize()
{
    m_Material.shader.use();

    cacheUniformLocations();
    if (locMaterialDiffuse >= 0) glUniform1i(locMaterialDiffuse, 0);
    if (locMaterialShininess >= 0) glUniform1f(locMaterialShininess, 64.0f);
    
    glActiveTexture(GL_TEXTURE0);
    m_Material.diffuseMap.bind();

    std::vector<float> vertexes;
    std::vector<unsigned int> indexes;
    setupMesh(vertexes, indexes);

    // Upload to GPU
    setupBuffers(vertexes, indexes);
    
    return true;
}

void Mesh::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix, const std::vector<Light>& lights, const glm::vec3& cameraPosition)
{
    m_Material.bind();

    // Send matrices
    glUniformMatrix4fv(locModel, 1, GL_FALSE, value_ptr(modelMatrix));
    glUniformMatrix4fv(locView,  1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(locProjection,  1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Camera Position
    glUniform3fv(locViewPosition, 1, glm::value_ptr(cameraPosition));

    // Bind diffuse texture to unit 0
    //glActiveTexture(GL_TEXTURE0);
    //m_Material.diffuseMap.bind();

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
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    //m_Material.diffuseMap.unbind();
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
    locModel = glGetUniformLocation(m_Material.shader.ID, "model");
    locView = glGetUniformLocation(m_Material.shader.ID, "view");
    locProjection = glGetUniformLocation(m_Material.shader.ID, "projection");
    locViewPosition = glGetUniformLocation(m_Material.shader.ID, "viewPos");
    locMaterialShininess = glGetUniformLocation(m_Material.shader.ID, "material.shininess");
    locMaterialDiffuse = glGetUniformLocation(m_Material.shader.ID, "material.diffuse");

    locLightPosition.resize(EngineLimits::MAX_LIGHTS);
    locLightColor.resize(EngineLimits::MAX_LIGHTS);
    locLightConst.resize(EngineLimits::MAX_LIGHTS);
    locLightLinear.resize(EngineLimits::MAX_LIGHTS);
    locLightQuad.resize(EngineLimits::MAX_LIGHTS);

    char buf[64];
    for (int i = 0; i < EngineLimits::MAX_LIGHTS; i++)
    {
        snprintf(buf, sizeof(buf), "lights[%d].position",  i);
        locLightPosition[i]   = glGetUniformLocation(m_Material.shader.ID, buf);
        snprintf(buf, sizeof(buf), "lights[%d].color",     i);
        locLightColor[i] = glGetUniformLocation(m_Material.shader.ID, buf);
        snprintf(buf, sizeof(buf), "lights[%d].constant",  i);
        locLightConst[i] = glGetUniformLocation(m_Material.shader.ID, buf);
        snprintf(buf, sizeof(buf), "lights[%d].linear",    i);
        locLightLinear[i]   = glGetUniformLocation(m_Material.shader.ID, buf);
        snprintf(buf, sizeof(buf), "lights[%d].quadratic", i);
        locLightQuad[i]  = glGetUniformLocation(m_Material.shader.ID, buf);
    }
}
