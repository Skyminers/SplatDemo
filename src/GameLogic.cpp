#include "GameLogic.h"
#include "Floor.h"

random_device e;
uniform_real_distribution<float> u(-0.5, 0.5);
uniform_real_distribution<float> uu(0, 1);
const float eps = 1e-5;

vector<Bullet*> Bullet::bulletQueue;
vector<Bullet*> Bullet::deadBulletQueue;
vector<Player*> Player::playerQueue;
unsigned int Player::ID = 0;

Bullet::Bullet(glm::vec3 pos, glm::vec3 dir, float time, unsigned int id, glm::vec3 color, float speed):
    bornPosition(pos), bornDirection(dir), bornTime(time) {
    this->position = pos + glm::vec3(0.0f, radius, 0.0f);
    this->color = color;
    this->speed = speed + u(e) * 2;
    this->g = 25.0f;
    this->teamid = id;
}

bool Bullet::checkPos(float x, float z) {
    return (fabs(x) <= FLOOR_MAX_POSITION && fabs(z) <= FLOOR_MAX_POSITION);
}

void Bullet::update(float time) {
    float deltaTime = time - bornTime;
    float y = bornPosition.y + bornDirection.y * deltaTime * speed - g * deltaTime * deltaTime / 2;
    float x = bornPosition.x + bornDirection.x * deltaTime * speed;
    float z = bornPosition.z + bornDirection.z * deltaTime * speed;
    if (y <= 0.0f) {
        y = 0.0f;
        alive = false;
        if (deadBulletQueue.size() < 100 && checkPos(position.x, position.z)) {
            deadBulletQueue.push_back(this);
        }
    }
    position = glm::vec3(x, y, z) + glm::vec3(0.0f, radius, 0.0f);
}

Player::Player(glm::vec3 pos, glm::vec3 color, unsigned int teamid, glm::vec3 dir) {
    this->position = pos;
    this->position_new = pos;
    this->color = color;
    this->g = 15.0f;
    this->speed = 5.0f;
    this->id = ID++;
    this->teamid = teamid;
}

bool Player::checkPos(float x, float z) {
    return (fabs(x) <= FLOOR_MAX_POSITION - 5.0f && fabs(z) <= FLOOR_MAX_POSITION - 5.0f);
}

void Player::startJump(float time, float v) {
    if (jumpPhase == 1) return;
    jumpPhase++;
    jumpTime = time;
    velocityY += v;
}

void Player::jumpUpdate(float time) {
    float deltaTime = time - jumpTime;
    float height = position.y + velocityY * deltaTime - g * deltaTime * deltaTime / 2;
    velocityY -= g * deltaTime;
    if (height < 0.0f) {
        height = 0.0f;
        velocityY = 0.0f;
        jumpPhase = 0;
    }
    jumpTime = time;
    position_new.y = height;
}

void Player::shoot(glm::vec3 front, float time) {
    glm::vec3 pos = position + front * 1.0f + glm::vec3(u(e), uu(e), u(e));
    Bullet::bulletQueue.push_back(new Bullet(pos, front, time, teamid, color));
}

int Player::HP() {
    return hp;
}

void Player::updateVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    gunDirection = glm::normalize(front);
    Right = Right = glm::normalize(glm::cross(Front, glm::vec3(0, 1, 0)));
}

const float playerA = 20.0f;

void Player::randomAct(float time, float deltaTime) {
    if (uu(e) < 0.005) startJump(time);
    if (uu(e) < 0.05) {
        yaw_new += u(e) * 30;
        GameLogic::checkPlayer(time, this);
    }
    if (uu(e) < 0.1) pitch_new += u(e) * 5;
    updateVectors();
    if (uu(e) < 0.4) {
        float addV = deltaTime * playerA;
        float p = uu(e);
        if (p < 0.5) V += Front * addV;
        else if (p < 0.6) V -= Front * addV;
        else if (p < 0.8) V += Right * addV;
        else V -= Right * addV;
    }
    if (uu(e) < 0.1) shoot(gunDirection, time);
}

const float MAXV = 6.0f;
const float floorA = 2.0f;

void Player::update(float time) {
    float deltaTime = time - moveTime;
    //cout << "player: " << deltaTime << " " << time << " " << pressTime << endl;
    //cout << frontA << " " << frontV << endl;
    //if (id == 0) cout << frontA << " " << frontV << endl;
    if (!inControl) randomAct(time, deltaTime);
    if (jumpPhase) jumpUpdate(time);
    float len =  glm::length(V);
    if (len > MAXV) {
        float ratio = MAXV / len;
        frontV *= ratio;
        rightV *= ratio;
        V *= ratio;
        len = MAXV;
    }
    //cout << len << endl;
    if (len > eps) {
        glm::vec3 dir = glm::normalize(V);
        glm::vec3 A = - floorA * dir;
        deltaTime = min(fabs(len) / fabs(floorA * deltaTime), deltaTime);
        position_new += V * deltaTime + A * deltaTime * deltaTime / 2.0f;
        V += A * deltaTime;
    }
    position_old = position;
    position = position_new;
    yaw_old = yaw;
    yaw = yaw_new;
    pitch_old = pitch;
    pitch = pitch_new;
    moveTime = time;
}

void Player::collisionSolve() {
    position.x = position_old.x;
    position.z = position_old.z;
    V = -V;
    yaw = yaw_old;
    pitch = pitch_old;
}

void Player::updateNew() {
    position_new = position;
    yaw_new = yaw;
    pitch_new = pitch;
}

void Player::edgeSolve() {
    V = -V;
    if (!inControl) {
        yaw += 180.0f;
    }
}

int GameLogic::playerNum;
int GameLogic::teamNum;

void GameLogic::init(int tNum, int pNum) {
    GameLogic::playerNum = pNum;
    GameLogic::teamNum = tNum;
    for (int i = 0; i < teamNum; ++i) {
        glm::vec3 color(uu(e), uu(e), uu(e));
        for (int j = 0; j < playerNum; ++j) {
            float px = u(e) * 50, py = u(e) * 50;
            Player::playerQueue.push_back(new Player(glm::vec3(px, 0.0f, py), color, i));
        }
    }
}

void GameLogic::checkPlayer(float time, Player* player) {
    if (player == nullptr) {
        for (auto & p1 : Player::playerQueue) {
            p1->update(time);
            if (!checkPos(p1->position.x, p1->position.z))
                p1->edgeSolve();
        }
        for (auto & p1 : Player::playerQueue) {
            for (auto &p2 : Player::playerQueue) {
                if (p2->id == p1->id) continue;
                if (PhysicalEngine::intersect(*p1, *p2)) {
                    p1->collisionSolve();
                    break;
                }
            }
            p1->updateNew();
        }
    } else {
        player->update(time);
        if (!checkPos(player->position.x, player->position.z)) {
            player->edgeSolve();
        }
        for (auto &p2 : Player::playerQueue) {
            if (p2->id == player->id) continue;
            if (PhysicalEngine::intersect(*player, *p2)) {
                player->collisionSolve();
                break;
            }
        }
        player->updateNew();
    }
}

void GameLogic::checkBullet(Bullet* bullet, float time) {
    bullet->update(time);
    for (auto & player : Player::playerQueue) {
        if (PhysicalEngine::intersect(*player, *bullet)) {
            if (player->teamid == bullet->teamid) {
                bullet->alive = false;
                continue;
            }
            player->hp--;
            if (player->hp <= 0) player->alive = false;
            player->halfLength = 0.6f + 0.4f * (float)player->hp / 50.0f;
            bullet->alive = false;
        }
    }
}

void GameLogic::plantBullet(){
    for(auto & bullet : Bullet::deadBulletQueue){
        plant(bullet->getPos()[0], bullet->getPos()[2], bullet->color);
    }
    Bullet::deadBulletQueue.clear();
}