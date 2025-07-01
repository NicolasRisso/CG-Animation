#include "texture.h"
#include <stb_image/stb_image.h>
#include <iostream>
#include <glad/glad.h>

Texture::Texture()
    : m_id(0), m_width(0), m_height(0), m_channels(0)
{
}

Texture::Texture(const std::string& filePath)
    : m_id(0), m_width(0), m_height(0), m_channels(0)
{
    if (load(filePath)) {
        m_filePath = filePath;
    }
}

Texture::~Texture()
{
    if (m_id != 0)
    {
        glDeleteTextures(1, &m_id);
    }
}

bool Texture::load(const std::string& filePath)
{
    m_filePath = filePath;
    // Gerar textura
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    
    // Configurar parâmetros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Carregar imagem
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 0);
    
    if (data)
    {
        GLenum format;
        if (m_channels == 1)
            format = GL_RED;
        else if (m_channels == 3)
            format = GL_RGB;
        else if (m_channels == 4)
            format = GL_RGBA;
        else
        {
            std::cerr << "Formato de textura não suportado: " << m_channels << " canais" << std::endl;
            stbi_image_free(data);
            return false;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(data);
        return true;
    }
    else
    {
        std::cerr << "Falha ao carregar textura: " << filePath << std::endl;
        std::cerr << "Erro STB: " << stbi_failure_reason() << std::endl;
        return false;
    }
}

void Texture::bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::getId() const
{
    return m_id;
}

int Texture::getWidth() const
{
    return m_width;
}

int Texture::getHeight() const
{
    return m_height;
}

int Texture::getChannels() const
{
    return m_channels;
}

std::string Texture::getFilePath() const
{
    return m_filePath;
}
