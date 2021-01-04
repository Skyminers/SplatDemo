//
// Created by 刘一辰 on 2020/10/25.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
: Front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM){
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

void Camera::updateCameraVectors(){
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::processKey(cameraMovement move, float deltaTime) {
    float moveLen = deltaTime * speed;
    switch (move) {
        case FORWARD:
            Position += Front * moveLen;
            break;
        case BACKWARD:
            Position -= Front * moveLen;
            break;
        case LEFT:
            Position -= Right * moveLen;
            break;
        case RIGHT:
            Position += Right * moveLen;
            break;
        case UP:
            Position += Up * moveLen;
            break;
        case DOWN:
            Position -= Up * moveLen;
            break;
    }
}

void Camera::processMouse(float xOffset, float yOffset) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset; pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    updateCameraVectors();
}

void Camera::processScroll(float yOffset) {
    zoom -= yOffset;
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > ZOOM) zoom = ZOOM;
}

glm::vec3 Camera::getPosition() {
    return Position;
}

glm::mat4 Camera::getViewMat() {
    return glm::lookAt(Position, Position + Front, Up);
}

