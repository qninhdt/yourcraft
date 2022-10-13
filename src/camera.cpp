#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace yc {

Camera::Camera() {}

void Camera::init(uint32_t screenWidth, uint32_t screenHeight) {
    m_pitch = 0;
    m_yaw = 0;
    m_position = glm::vec3(0);

    float screenRatio = 1.0f * screenWidth / screenHeight;
    m_projectionMatrix = glm::perspective(glm::radians(45.0f), screenRatio, 0.1f, 500.0f);

    updateDirection();
    updateMatrix();
}

glm::mat4 Camera::getViewMatrix() {
    return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() {
    return m_projectionMatrix;
}

glm::mat4 Camera::getProjectionViewMatrix() {
    return m_projectionViewMatrix;
}

float Camera::getPitch() {
    return m_pitch;
}

float Camera::getYaw() {
    return m_yaw;
}

glm::vec3 Camera::getRight() {
    return m_right;
}

glm::vec3 Camera::getFront() {
    return m_front;
}

glm::vec3 Camera::getPosition() {
    return m_position;
}

void Camera::setPosition(glm::vec3 postion) {
    m_position = postion;
    updateMatrix();
}

void Camera::setOrientation(float pitch, float yaw) {
    pitch = std::max(pitch, -89.0f);
    pitch = std::min(pitch, +89.0f);

    m_pitch = pitch;
    m_yaw = yaw;

    updateDirection();
    updateMatrix();
}

void Camera::updateDirection() {
    float rpitch = glm::radians(m_pitch);
    float ryaw = glm::radians(m_yaw);

    m_direction.x = cos(ryaw) * cos(rpitch);
    m_direction.y = sin(rpitch);
    m_direction.z = sin(ryaw) * cos(rpitch);

    m_direction = glm::normalize(m_direction);

    m_right = glm::normalize(glm::cross(m_direction, VectorUp));
    m_front = -glm::normalize(glm::cross(m_right, VectorUp));
}

void Camera::updateOrientation() {
    
}

void Camera::updateMatrix() {
    m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, VectorUp);
    m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
}

}