#ifndef SPLATDEMO_GAMELOGIC_H
#define SPLATDEMO_GAMELOGIC_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shaders.h>
#include <random>
#include <PhysicalEngine.h>

using namespace std;

class Bullet : public object {
public:
    static vector<Bullet*> bulletQueue;
    Bullet(glm::vec3 pos, glm::vec3 dir, float time, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f), float speed = 15.0f);

    void update(float time);

private:
    glm::vec3 bornDirection, bornPosition;
    float bornTime;

};

class Player : public object {
public:
    friend class Camera;
    static vector<Player*> playerQueue;
    explicit Player(glm::vec3 pos, glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f));

    void startJump(float time, float v = 7.0f, float g = 15.0f);
    void jumpUpdate(float time);
    void shoot(glm::vec3 front, float time);
    bool isJumping();
    int HP();
    float rotateAngle();

private:
    glm::vec3 gunDirection, moveDirection;
    float yaw = -90.0f, pitch = 0.0f, zoom = 45.0f;
    float velocityY = 7.0f, jumpTime = 0.0f;
    bool jumping = false;
    int hp = 100;

};

class GameLogic {
public:
    friend class Player;
    friend class Bullet;
    friend class GameWindow;
    void init(int num);

private:
    int playerNum;

};

#endif //SPLATDEMO_GAMELOGIC_H
