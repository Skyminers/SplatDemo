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

using namespace std;

class Bullet {
public:
    static vector<Bullet*> bulletQueue;
    Bullet(glm::vec3 pos, glm::vec3 dir, float time, float speed): position(pos), bornDirection(dir), bornTime(time), bornSpeed(speed) { }
    static void renderInit();
    void render(glm::mat4 projection, glm::mat4 view);

private:
    glm::vec3 position, bornDirection;
    float bornTime, bornSpeed;
};

class Player {
public:
    static vector<Player*> playerQueue;

    explicit Player(glm::vec3 pos, glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f), float speed = 1.0f, int id = 0, int hp = 100):
        position(pos), gunDirection(dir), moveDirection(dir), speed(speed), id(id), hp(hp) { }

    static void readOBJ(const char *filename);
    static void renderInit();
    void render(glm::mat4 projection, glm::mat4 view);

private:
    glm::vec3 position, gunDirection, moveDirection;
    float speed;
    int id, hp;
    static unsigned int VAO, VBO, EBO;
    static Shaders* shader;
    static vector<float> playerVertices;
    static vector<unsigned int> playerElement;
};

#endif //SPLATDEMO_PLAYER_H
