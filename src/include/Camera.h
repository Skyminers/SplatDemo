#ifndef GLTEST_CAMERA_H
#define GLTEST_CAMERA_H

#include <GameLogic.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum cameraMovement{
    FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN,
    CAMERA_PLAYER, LAST_PLAYER, NEXT_PLAYER, SHOOT
};

class Camera {
public:
    glm::vec3 Position, Front, Up, Right, WorldUp;
    float yaw, pitch, speed, sensitivity, zoom = 45.0f, jitter = 0.0f, shootJitter = 0.0f;
    bool shooting = false;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 getViewMat();
    void processKey(cameraMovement move, float deltaTime, float nowTime = 0.0f);
    void processMouse(float xOffset, float yOffset, float time);
    void processScroll(float yOffset);
    glm::vec3 getPosition() const;
    float getZoom();
    glm::vec3 getPlayerPos();
    bool bind();

private:
    constexpr static float YAW = -90.0f;
    constexpr static float PITCH = 0.0f;
    constexpr static float SPEED = 10.0f;
    constexpr static float SENSITIVITY = 0.1f;
    constexpr static float ZOOM = 70.0f;

    bool bindToPlayer = false;
    Player* player = nullptr;
    int playerid = 0;
    glm::vec3 playerPos;

    void updateCameraVectors();

};


#endif //GLTEST_CAMERA_H
