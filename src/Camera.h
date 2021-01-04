//
// Created by 刘一辰 on 2020/10/25.
//

#ifndef GLTEST_CAMERA_H
#define GLTEST_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum cameraMovement{
    FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
};

class Camera {
public:
    glm::vec3 Position, Front, Up, Right, WorldUp;
    float yaw, pitch, speed, sensitivity, zoom;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 getViewMat();
    void processKey(cameraMovement move, float deltaTime);
    void processMouse(float xOffset, float yOffset);
    void processScroll(float yOffset);
    glm::vec3 getPosition();

private:
    constexpr static float YAW = -90.0f;
    constexpr static float PITCH = 0.0f;
    constexpr static float SPEED = 2.5f;
    constexpr static float SENSITIVITY = 0.1f;
    constexpr static float ZOOM = 45.0f;

    void updateCameraVectors();

};


#endif //GLTEST_CAMERA_H
