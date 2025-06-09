#ifndef CAMERA_H
#define CAMERA_H

// Definições específicas para Windows para evitar conflitos de headers
#ifdef _WIN32
    #define NOMINMAX  // Evita conflitos com min/max do Windows
    #define WIN32_LEAN_AND_MEAN  // Reduz inclusões do Windows.h
    #include <windows.h>
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class Camera
 * @brief Classe para gerenciamento de câmera 3D
 */
class Camera {
public:
    /**
     * @brief Construtor da câmera
     * @param position Posição inicial da câmera
     * @param up Vetor up da câmera
     * @param yaw Ângulo de rotação horizontal (em graus)
     * @param pitch Ângulo de rotação vertical (em graus)
     */
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f
    );
    
    /**
     * @brief Obtém a matriz de visualização (view matrix)
     * @return Matriz de visualização
     */
    glm::mat4 getViewMatrix() const;
    
    /**
     * @brief Obtém a matriz de projeção
     * @param aspectRatio Razão de aspecto da janela
     * @param fov Campo de visão em graus
     * @param nearPlane Plano próximo de recorte
     * @param farPlane Plano distante de recorte
     * @return Matriz de projeção
     */
    glm::mat4 getProjectionMatrix(float aspectRatio, float fov = 45.0f, float nearPlane = 0.1f, float farPlane = 100.0f) const;
    
    /**
     * @brief Atualiza a posição da câmera
     * @param position Nova posição da câmera
     */
    void setPosition(const glm::vec3& position);
    
    /**
     * @brief Obtém a posição atual da câmera
     * @return Posição da câmera
     */
    glm::vec3 getPosition() const;
    
    /**
     * @brief Obtém o vetor de direção frontal da câmera
     * @return Vetor de direção frontal
     */
    glm::vec3 getFront() const;
    
    /**
     * @brief Obtém o vetor up da câmera
     * @return Vetor up
     */
    glm::vec3 getUp() const;
    
    /**
     * @brief Obtém o vetor right da câmera
     * @return Vetor right
     */
    glm::vec3 getRight() const;
    
    /**
     * @brief Atualiza os ângulos de rotação da câmera
     * @param yaw Ângulo de rotação horizontal (em graus)
     * @param pitch Ângulo de rotação vertical (em graus)
     */
    void setRotation(float yaw, float pitch);
    
    /**
     * @brief Rotaciona a câmera
     * @param yawOffset Deslocamento do ângulo yaw
     * @param pitchOffset Deslocamento do ângulo pitch
     * @param constrainPitch Se verdadeiro, restringe o pitch entre -89 e 89 graus
     */
    void rotate(float yawOffset, float pitchOffset, bool constrainPitch = true);

private:
    glm::vec3 m_position;    ///< Posição da câmera
    glm::vec3 m_front;       ///< Vetor de direção frontal
    glm::vec3 m_up;          ///< Vetor up
    glm::vec3 m_right;       ///< Vetor right
    glm::vec3 m_worldUp;     ///< Vetor up do mundo
    
    float m_yaw;             ///< Ângulo de rotação horizontal
    float m_pitch;           ///< Ângulo de rotação vertical
    
    /**
     * @brief Atualiza os vetores da câmera com base nos ângulos de Euler
     */
    void updateCameraVectors();
};

#endif // CAMERA_H
