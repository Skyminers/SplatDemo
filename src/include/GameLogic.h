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

class Bullet : public Sphere {
public:
    friend class GameLogic;
    static vector<Bullet*> bulletQueue;
    static vector<Bullet*> deadBulletQueue;
    Bullet(glm::vec3 pos, glm::vec3 dir, float time, unsigned int id, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f), float speed = 15.0f);
    void update(float time);

private:
    glm::vec3 bornDirection, bornPosition;
    float bornTime;

};

class Player : public Cube {
public:
    friend class Camera;
    friend class GameLogic;
    static vector<Player*> playerQueue;
    static unsigned int ID;
    explicit Player(glm::vec3 pos, glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f), unsigned int teamid = 0, glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f));

    void startJump(float time, float v = 7.0f, float g = 15.0f);
    void jumpUpdate(float time);
    void shoot(glm::vec3 front, float time);
    bool isJumping();
    int HP();
    void update(float time);
    void collisionSolve();
    void updateNew();
    void updateVectors();
    void randomAct(float time, float deltaTime);

private:
    glm::vec3 Front, Right, position_new, position_old, gunDirection;
    float frontV = 0.0f, rightV = 0.0f, frontA = 0.0f, rightA = 0.0f, moveTime = 0.0f;
    float yaw_new = 0.0f, yaw_old = 0.0f, pitch_new = 30.0f, pitch_old = 30.0f;
    int hp = 50;
    bool jumping = false, inControl = false;
    float jumpTime = 0.0f, velocityY = 7.0f, pressTime = 0.0f;

};

class GameLogic {
public:
    static int playerNum;
    static int teamNum;
    static void init(int teamNum, int playerNum);
    static void checkPlayer(float time, Player* player = nullptr);
    static void checkBullet(Bullet* bullet, float time);
    static void plantBullet();
};

#endif //SPLATDEMO_GAMELOGIC_H
