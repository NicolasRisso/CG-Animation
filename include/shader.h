#ifndef SHADER_H
#define SHADER_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX  // Evita conflitos com min/max do Windows
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN  // Reduz inclusões do Windows.h
    #endif
#endif

#include <glm/glm.hpp>

#include <string>
#include <sstream>

/**
 * @class Shader
 * @brief Classe para gerenciamento de shaders OpenGL
 */
class Shader
{
public:
    // ID do programa de shader
    unsigned int ID;

    /**
     * @brief Construtor que lê e constrói o shader
     * @param vertexPath Caminho para o arquivo do vertex shader
     * @param fragmentPath Caminho para o arquivo do fragment shader
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Ativa o shader
     */
    void use();

    /**
     * @brief Define um valor booleano uniforme no shader
     * @param name Nome do uniforme
     * @param value Valor booleano
     */
    void setBool(const std::string &name, bool value) const;
    
    /**
     * @brief Define um valor inteiro uniforme no shader
     * @param name Nome do uniforme
     * @param value Valor inteiro
     */
    void setInt(const std::string &name, int value) const;
    
    /**
     * @brief Define um valor float uniforme no shader
     * @param name Nome do uniforme
     * @param value Valor float
     */
    void setFloat(const std::string &name, float value) const;
    
    /**
     * @brief Define um sampler uniforme no shader
     * @param name Nome do uniforme
     * @param position Posição do sampler
     */
    void setSampler(const std::string &name, unsigned int position) const;
    
    /**
     * @brief Define um vetor 4D uniforme no shader
     * @param name Nome do uniforme
     * @param values Array de 4 valores
     */
    void setVec4(const std::string &name, double values[]) const;
    
    /**
     * @brief Define um vetor 2D uniforme no shader
     * @param name Nome do uniforme
     * @param values Array de 2 valores
     */
    void setVec2(const std::string &name, double values[]) const;
    
    /**
     * @brief Define um vetor 2D uniforme no shader
     * @param name Nome do uniforme
     * @param vector Vetor glm::vec2
     */
    void setVec2(const std::string &name, glm::vec2 vector) const;
    
    /**
     * @brief Define um vetor 3D uniforme no shader
     * @param name Nome do uniforme
     * @param vector Vetor glm::vec3
     */
    void setVec3(const std::string &name, glm::vec3 vector) const;
    
    /**
     * @brief Define uma matriz 4x4 uniforme no shader
     * @param name Nome do uniforme
     * @param matrix Matriz glm::mat4
     */
    void setMat4(const std::string &name, glm::mat4 matrix) const;

private:
    /**
     * @brief Verifica erros de compilação/vinculação de shader
     * @param shader ID do shader ou programa
     * @param type Tipo de verificação ("VERTEX", "FRAGMENT" ou "PROGRAM")
     */
    void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
