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

    explicit Material()
        : shader(std::string("Shaders/default.vs").c_str(), std::string("Shaders/default.fs").c_str()), diffuseMap()
    {
        diffuseMap.load("textures/metal_texture.jpg");
    }
    explicit Material(const std::string& texturePath)
        : shader(std::string("Shaders/default.vs").c_str(), std::string("Shaders/default.fs").c_str()), diffuseMap()
    {
        diffuseMap.load(texturePath);
    }
    Material(const std::string& vsPath, const std::string& fsPath, const std::string& texturePath)
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

    std::string getTexturePath() const { return diffuseMap.getFilePath(); }
    std::string getVertexShaderPath() const { return shader.getVertexPath(); }
    std::string getFragmentShaderPath() const { return shader.getFragmentPath(); }
};
