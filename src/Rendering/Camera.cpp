#include <Rendering/Camera.h>

Camera::Camera(const glm::vec3 &cameraPos)
    : cameraPos(cameraPos), pitch(0.0f), yaw(-90.0f), finalPitch(0.0f), finalYaw(90.0f), pitchStep(0.0f), yawStep(0.0f), zoomValue(45.0f), speed(10.0f), sensitivity(0.14f)
{
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}

void Camera::moveCamera(const glm::vec3 cameraMovement, float deltaTime)
{
    if (glm::length(cameraMovement) > 0)
    {
        cameraPos += glm::normalize(cameraMovement) * speed * deltaTime;
    }
}

void Camera::rotateCamera(float xOffset, float yOffset)
{
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;

    pitch = (pitch > 89.0f) ? 89.0f : pitch;
    pitch = (pitch < -89.0f) ? -89.0f : pitch;

    yaw = normalizeAngle(yaw);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}

void Camera::zoom(int wheelOffset)
{
    (wheelOffset < 0) ? zoomValue += 5 : zoomValue;
    (wheelOffset > 0) ? zoomValue -= 5 : zoomValue;
    zoomValue = (zoomValue <= 20) ? 25 : zoomValue;
    zoomValue = (zoomValue >= 160) ? 155 : zoomValue;
}

void Camera::setStepValues(glm::vec3 dir)
{
    finalPitch = glm::degrees(asin(dir.y));
    finalYaw = normalizeAngle(glm::degrees(atan2(dir.z, dir.x)));

    pitchStep = (finalPitch - pitch) / 10.0f;
    yawStep = shortestAngleDiff(yaw, finalYaw) / 10.0f;
}

void Camera::setCameraDirection(glm::vec3 dir)
{
    float p = glm::degrees(asin(dir.y));
    float y = glm::degrees(atan2(dir.z, dir.x));

    if (finalPitch != p || finalYaw != y)
        setStepValues(dir);

    pitch += pitchStep;
    yaw += yawStep;

    yaw = normalizeAngle(yaw);

    if (abs(finalPitch - pitch) < 0.1)
        pitchStep = 0;

    if (abs(finalYaw - yaw) < 0.1)
        yawStep = 0;

    reorienting = (pitchStep != 0 || yawStep != 0) ? true : false;

    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(dir);
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}

float Camera::normalizeAngle(float a)
{
    a = fmod(a + 180.0f, 360.0f);
    if (a < 0)
        a += 360.0f;
    return a - 180.0f;
}

float Camera::shortestAngleDiff(float from, float to)
{
    float diff = fmod(to - from + 180.0f, 360.0f);
    if (diff < 0)
        diff += 360.0f;
    return diff - 180.0f;
}

glm::vec3 Camera::getCameraPos() const
{
    return cameraPos;
}

glm::vec3 Camera::getCameraFront() const
{
    return cameraFront;
}

glm::vec3 Camera::getCameraUp() const
{
    return cameraUp;
}

glm::vec3 Camera::getCameraRight() const
{
    return cameraRight;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

float Camera::getZoom() const
{
    return zoomValue;
}

bool Camera::isRotating() const
{
    return rotating;
}

bool Camera::isReorienting() const
{
    return reorienting;
}

void Camera::setRotating(bool isRotating)
{
    rotating = isRotating;
}
