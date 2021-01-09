#include "GameLogic.h"
#include "Floor.h"

random_device e;
uniform_real_distribution<float> u(-0.5, 0.5);
uniform_real_distribution<float> uu(0, 1);
const float eps = 1e-3;

vector<Bullet*> Bullet::bulletQueue;
vector<Bullet*> Bullet::deadBulletQueue;
vector<Player*> Player::playerQueue;
unsigned int Player::ID = 0;

Bullet::Bullet(glm::vec3 pos, glm::vec3 dir, float time, unsigned int id, glm::vec3 color, float speed):
    bornPosition(pos), bornDirection(dir), bornTime(time) {
    this->position = pos + glm::vec3(0.0f, radius, 0.0f);
    this->color = color;
    this->speed = speed + u(e) * 2;
    this->g = 10.0f;
    this->teamid = id;
}

void Bullet::update(float time) {
    float deltaTime = time - bornTime;
    float y = bornPosition.y + bornDirection.y * deltaTime * speed - g * deltaTime * deltaTime / 2;
    float x = bornPosition.x + bornDirection.x * deltaTime * speed;
    float z = bornPosition.z + bornDirection.z * deltaTime * speed;
    if (y <= 0.0f) {
        y = 0.0f;
        alive = false;
        if (deadBulletQueue.size() < 100) {
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

void Player::startJump(float time, float v, float g) {
    if (jumping) return;
    jumping = true;
    jumpTime = time;
    velocityY = v;
    this->g = g;
}

void Player::jumpUpdate(float time) {
    float deltaTime = time - jumpTime;
    float height = velocityY * deltaTime - g * deltaTime * deltaTime / 2;
    if (height < 0.0f) {
        height = 0.0f;
        jumping = false;
    }
    position_new.y = height;
}

void Player::shoot(glm::vec3 front, float time) {
    glm::vec3 pos = position + front * 1.0f + glm::vec3(u(e), u(e), u(e));
    Bullet::bulletQueue.push_back(new Bullet(pos, front, time, teamid, color));
}

bool Player::isJumping() {
    return jumping;
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

void Player::randomAct(float time, float deltaTime) {
    if (uu(e) < 0.005) startJump(time);
    if (uu(e) < 0.05) {
        yaw_new += u(e) * 30;
        GameLogic::checkPlayer(time, this);
    }
    if (uu(e) < 0.1) pitch_new += u(e) * 5;
    updateVectors();
    if (uu(e) < 0.4) {
        float moveLen = deltaTime * speed;
        float p = uu(e);
        if (p < 0.7) position_new += Front * moveLen;
        else if (p < 0.8) position_new -= Front * moveLen;
        else if (p < 0.9) position_new += Right * moveLen;
        else position_new -= Right * moveLen;
    }
    if (uu(e) < 0.1) shoot(gunDirection, time);
}

void Player::update(float time) {
    float deltaTime = time - moveTime;
    //cout << "player: " << deltaTime << " " << time << " " << pressTime << endl;
    //cout << frontA << " " << frontV << endl;
    //if (id == 0) cout << frontA << " " << frontV << endl;
    if (!inControl) randomAct(time, deltaTime);
    if (jumping) jumpUpdate(time);
    position_old = position;
    position = position_new;
//    cout << position.x << " " << position.y << " " << position.z << endl;
//    if (frontV > speed + eps) {
//        frontV = speed;
//    }
//    if (frontV < -speed - eps) {
//        frontV = -speed;
//    }
//    if (rightV > speed + eps) {
//        rightV = speed;
//    }
//    if (rightV < -speed - eps) {
//        rightV = -speed;
//    }
//    position += (frontV * deltaTime + frontA * deltaTime * deltaTime / 2) * Front;
//    position += (rightV * deltaTime + rightA * deltaTime * deltaTime / 2) * Right;
    yaw_old = yaw;
    yaw = yaw_new;
    pitch_old = pitch;
    pitch = pitch_new;
//    if (time - pressTime > 0.1f) {
//        if (frontV > 0) frontA -= 5.0f * deltaTime;
//        else frontA += 5.0f * deltaTime;
//        if (rightV > 0) rightA -= 5.0f * deltaTime;
//        else rightA += 5.0f * deltaTime;
//        //if (id == 0) cout << "!" << frontA << " " << frontV << endl;
//        float fV = frontV;
//        float rV = rightV;
//        frontV += frontA * deltaTime;
//        //if (id == 0) cout << "!" << frontA << " " << frontV << endl;
//        rightV += rightA * deltaTime;
//        if ((fV > 0) != (frontV > 0) || fabs(frontV) >= fabs(fV) + fabs(frontA * deltaTime) + eps) frontV = frontA = 0;
//        if ((rV > 0) != (rightV > 0) || fabs(rightV) >= fabs(rV) + fabs(rightA * deltaTime) + eps) rightV = rightV = 0;
//        //if (id == 0) cout << "!" << frontA << " " << frontV << endl;
//    } else {
//        frontV += frontA * deltaTime;
//        rightV += rightA * deltaTime;
//    }
    moveTime = time;
}

void Player::collisionSolve() {
    position.x = position_old.x;
    position.z = position_old.z;
    yaw = yaw_old;
    pitch = pitch_old;
}

void Player::updateNew() {
    position_new = position;
    yaw_new = yaw;
    pitch_new = pitch;
}

int GameLogic::playerNum;
int GameLogic::teamNum;

void GameLogic::init(int teamNum, int playerNum) {
    GameLogic::playerNum = playerNum;
    GameLogic::teamNum = teamNum;
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
        for (auto & p1 : Player::playerQueue) p1->update(time);
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
        if (player->teamid == bullet->teamid) continue;
        if (PhysicalEngine::intersect(*player, *bullet)) {
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