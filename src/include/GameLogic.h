#ifndef SPLATDEMO_GAMELOGIC_H
#define SPLATDEMO_GAMELOGIC_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shaders.h>
#include <random>
#include <PhysicalEngine.h>
#include <defineList.h>
#include <Floor.h>

using namespace std;

class Bullet : public Sphere {
public:
    friend class GameLogic;
    static vector<Bullet*> bulletQueue;
    static vector<Bullet*> deadBulletQueue;
    Bullet(glm::vec3 pos, glm::vec3 dir, float time, unsigned int id, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f), float speed = 40.0f);
    void update(float time);
    static bool checkPos(float x, float z);

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

    void startJump(float time, float v = 8.0f);
    void jumpUpdate(float time);
    bool shoot(glm::vec3 front, float time, unsigned int tid = 0, bool flag = false, float speed = 0.0f);
    int HP() const;
    void update(float time);
    void collisionSolve();
    void updateNew();
    void updateVectors();
    void randomAct(float time, float deltaTime);
    void edgeSolve();
    void dive(float time);
    bool& Attacked();
    void deadBomb(float time, unsigned int tid);
    unsigned int getDefeater() const;
    static bool checkPos(float x, float z);
    static int getColorID(float x, float z);

private:
    glm::vec3 Front, Right, position_new, position_old, gunDirection;
    float frontV = 0.0f, rightV = 0.0f, moveTime = 0.0f;
    float yaw_new = 0.0f, yaw_old = 0.0f, pitch_new = 30.0f, pitch_old = 30.0f;
    int hp = 50, jumpPhase = 0;
    bool inControl = false, diving = false, attacked = false;
    float jumpTime = 0.0f, velocityY = 0.0f, pressTime = 0.0f;
    unsigned int defeater = 0;

};

class Particle : public Object {
public:
    static vector<Particle*> particleQueue;
    float life;
    glm::vec3 speedvec;
    Particle(glm::vec3 pos, glm::vec3 dir, float life, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f), float speed = 20.0f);
    void update(float time);
};

class GameLogic {
public:
    static vector<glm::vec3> colors;
    static int playerNum;
    static int teamNum;
    static void init(int teamNum, int playerNum);
    static void checkPlayer(float time, Player* player = nullptr);
    static void checkBullet(Bullet* bullet, float time);
    static void plantBullet();
};

#endif //SPLATDEMO_GAMELOGIC_H
