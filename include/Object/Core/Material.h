#pragma once
#include <string>
#include <glm/vec3.hpp>

#include "shader.h"
#include "texture.h"
#include "glad/glad.h"

struct Material
{
    Shader shader;
    Texture diffuseMap;
    glm::vec3 diffuseColor{ 1.f, 1.f, 1.f };

    explicit Material(const std::string& vsPath = "Shaders/default.vs", const std::string& fsPath = "Shaders/default.fs",
        const std::string& texturePath = "textures/metal_texture.jpg")
        : shader(vsPath.c_str(), fsPath.c_str()), diffuseMap()
    {
        diffuseMap.load(texturePath);
    }

    void bind()
    {
        shader.use();
        shader.setInt("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE0);
        diffuseMap.bind();
        shader.setVec3("material.color", diffuseColor);
    }
};
