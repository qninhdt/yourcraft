#pragma once

#include <glm/glm.hpp>

namespace yc {

const glm::vec3 VectorUp = { 0, 1, 0 };
const glm::vec3 VectorRight = { 1, 0, 0 };

class Camera {

public:
    Camera();
    void init();
    void update();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getProjectionViewMatrix() const;
    float getYaw() const;
    float getPitch() const;
    void setOrientation(float pitch, float yaw);
    float getZoom() const;
    glm::vec3 getRight() const;
    glm::vec3 getFront() const;
    glm::vec3 getPosition() const;
    void setPosition(glm::vec3 position);
    glm::vec3 getDirection() const;
    
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