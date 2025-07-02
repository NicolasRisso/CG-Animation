#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Object/Meshes/Custom/InvisibleMesh.h"

Camera::Camera(glm::vec3 up, float yaw, float pitch)
    : m_worldUp(up), m_yaw(yaw), m_pitch(pitch), SceneObject(Transform(), [&]()
    {
        InvisibleMesh* mesh = new InvisibleMesh();
        return mesh;
    }(), Material())
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(GetObjectPosition(), GetObjectPosition() + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio, float fov, float nearPlane, float farPlane) const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::getFront() const
{
    return m_front;
}

glm::vec3 Camera::getUp() const
{
    return m_up;
}

glm::vec3 Camera::getRight() const
{
    return m_right;
}

void Camera::SetCameraRotation(glm::vec3 rotation)
{
    m_yaw = rotation.x;
    m_pitch = rotation.y;
    SetObjectRotation(rotation);
    updateCameraVectors();
}

void Camera::rotate(float yawOffset, float pitchOffset, bool constrainPitch)
{
    m_yaw += yawOffset;
    m_pitch += pitchOffset;
    
    // Restringir pitch se necessário
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }
    
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // Calcular novo vetor front
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    
    // Recalcular vetores right e up
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
