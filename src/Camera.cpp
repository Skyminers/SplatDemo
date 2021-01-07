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
    if (!bindToPlayer) {
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    } else {
        front.x = cos(glm::radians(player->yaw)) * cos(glm::radians(player->pitch));
        front.y = sin(glm::radians(player->pitch));
        front.z = sin(glm::radians(player->yaw)) * cos(glm::radians(player->pitch));
        player->moveDirection = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(player->moveDirection, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }

}

void Camera::processKey(cameraMovement key, float deltaTime, float nowTime) {
    float moveLen = deltaTime * speed;
    switch (key) {
        case FORWARD:
            if (!bindToPlayer) Position += Front * moveLen;
            else player->position += player->moveDirection * moveLen;
            break;
        case BACKWARD:
            if (!bindToPlayer) Position -= Front * moveLen;
            else player->position -= player->moveDirection * moveLen;
            break;
        case LEFT:
            if (!bindToPlayer) Position -= Right * moveLen;
            else player->position -= Right * moveLen;
            break;
        case RIGHT:
            if (!bindToPlayer) Position += Right * moveLen;
            else player->position += Right * moveLen;
            break;
        case UP:
            if (!bindToPlayer) Position += Up * moveLen;
            else player->startJump(nowTime);
            break;
        case DOWN:
            if (!bindToPlayer) Position -= Up * moveLen;
            //else player->position -= Up * moveLen;
            break;
        case CAMERA_PLAYER:
            shooting = false;
            if (jitter < 0.3f) break;
            bindToPlayer = !bindToPlayer;
            player = Player::playerQueue[playerid];
            updateCameraVectors();
            jitter = 0.0f;
            break;
        case LAST_PLAYER:
            shooting = false;
            if (jitter < 0.3f) break;
            if (bindToPlayer) {
                if (--playerid < 0) playerid = Player::playerQueue.size() - 1;
                player = Player::playerQueue[playerid];
                updateCameraVectors();
            }
            jitter = 0.0f;
            break;
        case NEXT_PLAYER:
            shooting = false;
            if (jitter < 0.3f) break;
            if (bindToPlayer) {
                if (++playerid >= Player::playerQueue.size()) playerid = 0;
                player = Player::playerQueue[playerid];
                updateCameraVectors();
            }
            jitter = 0.0f;
            break;
        case SHOOT:
            if (shootJitter < 0.08f) break;
            if (bindToPlayer) {
                shooting = true;
                player->shoot(Front, nowTime);
            }
            shootJitter = 0.0f;
            break;
    }
}

void Camera::processMouse(float xOffset, float yOffset) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    if (!bindToPlayer) {
        yaw += xOffset; pitch += yOffset;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    } else {
        player->yaw += xOffset, player->pitch += yOffset;
        if (player->pitch < -50.0f) player->pitch = -50.0f;
        if (player->pitch > 30.0f) player->pitch = 30.0f;
    }
    updateCameraVectors();
}

void Camera::processScroll(float yOffset) {
    if (!bindToPlayer) {
        zoom -= yOffset;
        if (zoom < 1.0f) zoom = 1.0f;
        if (zoom > ZOOM) zoom = ZOOM;
    } else {
        player->zoom -= yOffset;
        if (player->zoom < 1.0f) player->zoom = 1.0f;
        if (player->zoom > ZOOM) player->zoom = ZOOM;
    }
}

float Camera::getZoom() {
    if (bindToPlayer) return player->zoom;
    return zoom;
}

glm::vec3 Camera::getPosition() {
    return Position;
}

glm::mat4 Camera::getViewMat() {
    if (!bindToPlayer) {
        return glm::lookAt(Position, Position + Front, Up);
    } else {
        glm::vec3 pos = player->position - Front * 10.0f + glm::vec3(0.0f, 8.0f, 0.0f);
        if (pos.y < 0.1f) pos.y = 0.1f;
        return glm::lookAt(pos, player->position + Front * 10.0f, Up);
    }
}

