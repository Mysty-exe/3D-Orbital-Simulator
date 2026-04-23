#pragma once
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
private:
    glm::vec3 cameraPos, cameraFront, cameraUp, cameraRight;
    float pitch, yaw, finalPitch, finalYaw, pitchStep, yawStep, zoomValue, speed, sensitivity;
    bool rotating, reorienting;

public:
    Camera(const glm::vec3 &cameraPos);
    void moveCamera(const glm::vec3 cameraMovement, float deltaTime);
    void rotateCamera(float xOffset, float yOffset);
    void zoom(int wheelOffset);
    void setStepValues(glm::vec3 dir);
    void setCameraDirection(glm::vec3 dir);
    float normalizeAngle(float a);
    float shortestAngleDiff(float from, float to);
    glm::vec3 getCameraPos() const;
    glm::vec3 getCameraFront() const;
    glm::vec3 getCameraUp() const;
    glm::vec3 getCameraRight() const;
    glm::mat4 getViewMatrix() const;
    float getZoom() const;
    bool isRotating() const;
    bool isReorienting() const;
    void setRotating(bool isRotating);
    void setPosition(glm::vec3 pos) { this->cameraPos = pos; };
    float getSpeed() { return speed; };
    void setSpeed(float speed) { this->speed = speed; };
};
