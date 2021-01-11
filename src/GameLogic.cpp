#include "GameLogic.h"
#include "Floor.h"

random_device e;
uniform_real_distribution<float> u(-0.5, 0.5);
uniform_real_distribution<float> uu(0, 1);
const float eps = 1e-9;

vector<Bullet*> Bullet::bulletQueue;
vector<Bullet*> Bullet::deadBulletQueue;
vector<Player*> Player::playerQueue;
unsigned int Player::ID = 0;

int GameLogic::playerNum;
int GameLogic::teamNum;
vector<glm::vec3> GameLogic::colors;

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
        if (deadBulletQueue.size() < 1000 && checkPos(position.x, position.z)) {
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
    this->yaw = this->yaw_new = this->yaw_old = u(e) * 360.0f;
}

bool Player::checkPos(float x, float z) {
    return (fabs(x) <= FLOOR_MAX_POSITION - 5.0f && fabs(z) <= FLOOR_MAX_POSITION - 5.0f);
}

void Player::startJump(float time, float v) {
    if (jumpPhase == 1 || diving) return;
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
    if (diving) return;
    glm::vec3 pos = position + front * 1.0f + glm::vec3(u(e), uu(e), u(e));
    Bullet::bulletQueue.push_back(new Bullet(pos, front, time, teamid, color));
}

int Player::HP() const {
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
    if (uu(e) < 0.03) {
        yaw_new += u(e) * 30;
        GameLogic::checkPlayer(time, this);
    }
    if (uu(e) < 0.1) {
        pitch_new += u(e) * 5;
        if (pitch_new < -20.0f) pitch_new = -20.0f;
        if (pitch_new > 30.0f) pitch_new = 30.0f;
    }
    updateVectors();
    if (uu(e) < 0.6) {
        float addV = deltaTime * playerA;
        float p = uu(e);
        if (p < 0.5) V += Front * addV;
        else if (p < 0.6) V -= Front * addV;
        else if (p < 0.8) V += Right * addV;
        else V -= Right * addV;
    }
    if (uu(e) < 0.5) shoot(gunDirection, time);
    if (uu(e) < 0.05) dive(time);
}
const float ourMAXV = 10.0f;
const float MAXV = 6.0f;
const float otherMAXV = 4.0f;
const float ourFloorF = 2.0f;
const float floorF = 5.0f;
const float otherFloorF = 10.0f;
const float airF = 1.0f;
const float diveV = 5.0f;
const float MINY = 0.05f;

void Player::update(float time) {
    float deltaTime = time - moveTime;
    if (!inControl) randomAct(time, deltaTime);
    if (jumpPhase) jumpUpdate(time);
    if (time - pressTime > 0.2f) diving = false;
    int colorID = getColorID(position.x, position.z);
    if (colorID != teamid) diving = false;
    if (diving) {
        fateY -= deltaTime * diveV;
        if (fateY < MINY) fateY = MINY;
    } else if (fateY < 1.0f){
        fateY += deltaTime * diveV;
        if (fateY > 1.0f) fateY = 1.0f;
    }
    float len =  glm::length(V);
    float maxV = MAXV;
    if (!jumpPhase) {
        if (colorID == -1) maxV = MAXV;
        else if (colorID == teamid) {
            if (diving) maxV = ourMAXV;
            else maxV = MAXV;
        } else maxV = otherMAXV;
    }
    if (len > maxV - eps) {
        float ratio = maxV / len;
        frontV *= ratio;
        rightV *= ratio;
        V *= ratio;
        len = maxV;
    }
    float floorA = floorF * halfLength * halfLength;
    float airA = airF * halfLength * halfLength;
    float ourFloorA = ourFloorF * halfLength * halfLength;
    float otherFloorA = otherFloorF * halfLength * halfLength;
    if (len > eps) {
        glm::vec3 dir = glm::normalize(V);
        glm::vec3 A = - airA * dir;
        if (!jumpPhase) {
            if (colorID == -1) A += - floorA * dir;
            else if (colorID == teamid) A += - ourFloorA * dir;
            else A += - otherFloorA * dir;
        }
        deltaTime = min(fabs(len) / fabs(floorA * deltaTime), deltaTime);
        if (len > 0.1f) position_new += V * deltaTime + A * deltaTime * deltaTime / 2.0f;
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
    collisionSolve();
    yaw = yaw_new;
    if (!inControl) {
        yaw += 180.0f;
    }
}

void Player::dive(float time) {
    if (jumpPhase) {
        diving = false;
        return;
    }
    diving = true;
    pressTime = time;
}

bool& Player::Attacked() {
    return attacked;
}

void Player::deadBomb(float time) {
    for (int i = 0; i < 20; ++i) {
        pitch = 30.0f + 20.0f * uu(e);
        yaw = 360.f * u(e);
        updateVectors();
        shoot(gunDirection, time);
    }
}

const float colorEps = 1e-3;

int Player::getColorID(float x, float z) {
    glm::vec3 c1 = getColorAt(x, z);
    for (int i = 0; i < GameLogic::colors.size(); ++i) {
        glm::vec3 c2 = GameLogic::colors[i];
        if (fabs(c1.x - c2.x) > colorEps) continue;
        if (fabs(c1.y - c2.y) > colorEps) continue;
        if (fabs(c1.z - c2.z) > colorEps) continue;
        return i;
    }
    return -1;
}

void GameLogic::init(int tNum, int pNum) {
    GameLogic::playerNum = pNum;
    GameLogic::teamNum = tNum;
    for (int i = 0; i < teamNum; ++i) {
        glm::vec3 color(uu(e), uu(e), uu(e));
        colors.push_back(color);
        for (int j = 0; j < playerNum; ++j) {
            float px = u(e) * 45, py = u(e) * 45;
            Player::playerQueue.push_back(new Player(glm::vec3(px, 0.0f, py), color, i));
        }
    }
    for (int i = 1; i < Player::playerQueue.size(); ++i) {
        Player* p1 = Player::playerQueue[i];
        int flag = 1;
        while (flag) {
            flag = 0;
            for (int j = 0; j < i; ++j) {
                Player* p2 = Player::playerQueue[j];
                if (PhysicalEngine::intersect(*p1, *p2)) { flag = 1; break; }
            }
            if (flag) p1->position = glm::vec3(u(e) * 45, 0.0f, u(e) * 45);
        }
    }
}

void GameLogic::checkPlayer(float time, Player* player) {
    if (player == nullptr) {
        for (auto & p1 : Player::playerQueue) {
            p1->update(time);
            if (!Player::checkPos(p1->position.x, p1->position.z))
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
        if (!Player::checkPos(player->position.x, player->position.z)) {
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
            player->attacked = true;
            if (player->hp <= 0) {
                player->alive = false;
                player->deadBomb(time);
            }
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