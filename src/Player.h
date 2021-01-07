//
// Created by ZzZZCHS on 2021/1/6.
//

#ifndef SPLATDEMO_PLAYER_H
#define SPLATDEMO_PLAYER_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shaders.h"
#include <random>

using namespace std;

class Bullet {
public:
    static vector<Bullet*> bulletQueue;
    bool die = false;
    Bullet(glm::vec3 pos, glm::vec3 dir, float time, float speed = 15.0f): position(pos), bornPosition(pos), bornDirection(dir), bornTime(time), speed(speed) { }
    static void readOBJ(const char *filename);
    static void renderInit();
    void render(glm::mat4 projection, glm::mat4 view, float time);
    void update(float time);

private:
    glm::vec3 position, bornDirection, bornPosition;
    float bornTime, speed, g = 10.0f;
    static unsigned int VAO, VBO, EBO;
    static Shaders* shader;
    static vector<float> bulletVertices;
    static vector<unsigned int> bulletElement;
};

class Player {
public:
    static vector<Player*> playerQueue;
    glm::vec3 position, gunDirection, moveDirection;
    float yaw = -90.0f, pitch = 0.0f, speed = 5.0f, zoom = 45.0f;
    bool die = false;

    explicit Player(glm::vec3 pos, glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f)):
        position(pos), gunDirection(dir), moveDirection(dir) { }

    static void readOBJ(const char *filename);
    static void renderInit();
    void render(glm::mat4 projection, glm::mat4 view, float time);
    void startJump(float time, float v = 7.0f, float g = 15.0f);
    void jumpUpdate(float time);
    void shoot(glm::vec3 front, float time);

private:
    int hp = 100;
    float g = 15.0f, velocityY = 7.0f, jumpTime = 0.0f;
    bool jumping = false;
    static unsigned int VAO, VBO, EBO;
    static Shaders* shader;
    static vector<float> playerVertices;
    static vector<unsigned int> playerElement;
};

#endif //SPLATDEMO_PLAYER_H
