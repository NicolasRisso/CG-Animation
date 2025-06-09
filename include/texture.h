#ifndef TEXTURE_H
#define TEXTURE_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #define NOMINMAX  // Evita conflitos com min/max do Windows
    #define WIN32_LEAN_AND_MEAN  // Reduz inclusões do Windows.h
    #include <windows.h>
#endif

#include <glad/glad.h>
#include <string>

/**
 * @class Texture
 * @brief Classe para gerenciamento de texturas OpenGL
 */
class Texture {
public:
    /**
     * @brief Construtor padrão
     */
    Texture();
    
    /**
     * @brief Construtor que carrega uma textura a partir de um arquivo
     * @param filePath Caminho para o arquivo de textura
     */
    Texture(const std::string& filePath);
    
    /**
     * @brief Destrutor
     */
    ~Texture();
    
    /**
     * @brief Carrega uma textura a partir de um arquivo
     * @param filePath Caminho para o arquivo de textura
     * @return true se o carregamento foi bem-sucedido, false caso contrário
     */
    bool load(const std::string& filePath);
    
    /**
     * @brief Ativa a textura em uma unidade específica
     * @param unit Unidade de textura (padrão: 0)
     */
    void bind(unsigned int unit = 0) const;
    
    /**
     * @brief Desativa a textura
     */
    void unbind() const;
    
    /**
     * @brief Obtém o ID da textura OpenGL
     * @return ID da textura
     */
    unsigned int getId() const;
    
    /**
     * @brief Obtém a largura da textura
     * @return Largura da textura em pixels
     */
    int getWidth() const;
    
    /**
     * @brief Obtém a altura da textura
     * @return Altura da textura em pixels
     */
    int getHeight() const;
    
    /**
     * @brief Obtém o número de canais da textura
     * @return Número de canais (1=R, 3=RGB, 4=RGBA)
     */
    int getChannels() const;

private:
    unsigned int m_id;       ///< ID da textura OpenGL
    int m_width;             ///< Largura da textura em pixels
    int m_height;            ///< Altura da textura em pixels
    int m_channels;          ///< Número de canais da textura
};

#endif // TEXTURE_H
