#include <GameLogic.h>

random_device e;
uniform_real_distribution<double> u(-0.5, 0.5);

vector<Bullet*> Bullet::bulletQueue;
vector<Player*> Player::playerQueue;

void Bullet::update(float time) {
    float deltaTime = time - bornTime;
    float y = bornPosition.y + bornDirection.y * deltaTime * speed - g * deltaTime * deltaTime / 2;
    float x = bornPosition.x + bornDirection.x * deltaTime * speed;
    float z = bornPosition.z + bornDirection.z * deltaTime * speed;
    if (y <= 0.0f) {
        y = 0.0f;
        alive = false;
    }
    position = glm::vec3(x, y, z);
}

glm::vec3 Bullet::getPos() {
    return position;
}

bool Bullet::isAlive() {
    return alive;
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
    position.y = height;
}

void Player::shoot(glm::vec3 front, float time) {
    glm::vec3 pos = position + front * 1.0f + glm::vec3(u(e), u(e), u(e));
    Bullet::bulletQueue.push_back(new Bullet(pos, front, time));
}

bool Player::isAlive() {
    return hp > 0;
}

bool Player::isJumping() {
    return jumping;
}

int Player::HP() {
    return hp;
}

glm::vec3 Player::getPos() {
    return position;
}

float Player::rotateAngle() {
    return -yaw;
}

void GameLogic::init(int num) {
    playerNum = num;
    for (int i = 0; i < playerNum; ++i) {
        Player::playerQueue.push_back(new Player(glm::vec3(i * 10, 0, 0)));
    }
}
