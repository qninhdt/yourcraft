#pragma once

#include <glm/glm.hpp>

namespace yc {

const glm::vec3 VectorUp = { 0, 1, 0 };
const glm::vec3 VectorRight = { 1, 0, 0 };

class Camera {

public:
    Camera();
    void init(uint32_t screenWidth, uint32_t screenHeight);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getProjectionViewMatrix();
    float getYaw();
    float getPitch();
    void setOrientation(float pitch, float yaw);
    float getZoom();
    glm::vec3 getRight();
    glm::vec3 getFront();
    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);
    
private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    float m_zoom;
    float m_yaw;
    float m_pitch;
    glm::vec3 m_right;
    glm::vec3 m_front;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_projectionViewMatrix;

    void updateDirection();
    void updateOrientation();
    void updateMatrix();
};

}