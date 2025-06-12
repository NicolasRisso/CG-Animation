#include "shader.h"

#include <fstream>
#include <iostream>
#include <glad/glad.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
    : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath)
{
    // 1. Recuperar o código fonte do vertex/fragment shader a partir do caminho do arquivo
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    
    // Garantir que os objetos ifstream possam lançar exceções
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try 
    {
        // Abrir arquivos
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;
        
        // Ler conteúdo dos arquivos para os streams
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        
        // Fechar manipuladores de arquivo
        vertexShaderFile.close();
        fragmentShaderFile.close();
        
        // Converter stream para string
        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERRO::SHADER::FALHA_AO_LER_ARQUIVO" << std::endl;
    }
    
    const char* vertexShaderCode = vertexCode.c_str();
    const char* fragmentShaderCode = fragmentCode.c_str();
    
    // 2. Compilar shaders
    unsigned int vertex, fragment;
    
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // Programa de Shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    
    // Excluir os shaders, pois já estão vinculados ao programa e não são mais necessários
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

std::string Shader::getVertexPath() const
{
    return m_vertexPath;
}

std::string Shader::getFragmentPath() const
{
    return m_fragmentPath;
}

void Shader::use()
{ 
    glUseProgram(ID); 
}

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setSampler(const std::string &name, unsigned int position) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), position);
}

void Shader::setVec4(const std::string &name, double values[]) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), 
               values[0], values[1], values[2], values[3]);
}

void Shader::setVec2(const std::string &name, double values[]) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), values[0], values[1]);
}

void Shader::setVec2(const std::string &name, glm::vec2 vector) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), vector[0], vector[1]);
}

void Shader::setVec3(const std::string &name, glm::vec3 vector) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vector[0], vector[1], vector[2]);
}

void Shader::setMat4(const std::string &name, glm::mat4 matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERRO::SHADER::COMPILACAO_FALHOU do tipo: " << type << "\n" 
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERRO::PROGRAMA::VINCULACAO_FALHOU do tipo: " << type << "\n" 
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
