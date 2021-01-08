#ifndef SPLATDEMO_GAMELOGIC_H
#define SPLATDEMO_GAMELOGIC_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shaders.h>
#include <random>

using namespace std;

class Bullet {
public:
    static vector<Bullet*> bulletQueue;
    Bullet(glm::vec3 pos, glm::vec3 dir, float time, float speed = 15.0f):
        position(pos), bornPosition(pos), bornDirection(dir), bornTime(time), speed(speed) { }

    void update(float time);
    glm::vec3 getPos();
    bool isAlive();

private:
    glm::vec3 position, bornDirection, bornPosition;
    float bornTime, speed, g = 10.0f;
    bool alive = true;

};

class Player {
public:
    friend class Camera;
    static vector<Player*> playerQueue;
    explicit Player(glm::vec3 pos, glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f)):
        position(pos), gunDirection(dir), moveDirection(dir) { }

    void startJump(float time, float v = 7.0f, float g = 15.0f);
    void jumpUpdate(float time);
    void shoot(glm::vec3 front, float time);
    bool isAlive();
    bool isJumping();
    int HP();
    glm::vec3 getPos();
    float rotateAngle();

private:
    glm::vec3 position, gunDirection, moveDirection;
    float yaw = -90.0f, pitch = 0.0f, speed = 5.0f, zoom = 45.0f;
    float g = 15.0f, velocityY = 7.0f, jumpTime = 0.0f;
    bool jumping = false;
    int hp = 100;

};

class GameLogic {
public:
    friend class Player;
    friend class Bullet;
    friend class GameWindow;
    void init(int num);
    void renderObjects(float time);

private:

    int playerNum;
};

#endif //SPLATDEMO_GAMELOGIC_H
