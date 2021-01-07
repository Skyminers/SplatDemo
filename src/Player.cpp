//
// Created by ZzZZCHS on 2021/1/6.
//

#include "Player.h"

vector<Bullet*> Bullet::bulletQueue;
vector<Player*> Player::playerQueue;
unsigned int Player::VAO, Player::VBO, Player::EBO;
Shaders* Player::shader;
vector<float> Player::playerVertices;
vector<unsigned int> Player::playerElement;
unsigned int Bullet::VAO, Bullet::VBO, Bullet::EBO;
Shaders* Bullet::shader;
vector<float> Bullet::bulletVertices;
vector<unsigned int> Bullet::bulletElement;

random_device e;
uniform_real_distribution<double> u(-0.5, 0.5);

void Bullet::readOBJ(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr) {
        puts("Error: failed to read player obj file!");
        exit(-1);
    }
    char cmd[1024];
    char seps[] = " ,\t\n";
    while (true) {
        if (fgets(cmd, 1024, fp) == nullptr) break;
        char *token = strtok(cmd, seps);
        if (token == nullptr) continue;

        if (strcmp(token, "v") == 0) {
            for (int i = 0; i < 3; ++i) {
                token = strtok(nullptr, seps);
                bulletVertices.push_back(atof(token));
            }
            bulletVertices.push_back(1.0f);
            bulletVertices.push_back(0.0f);
            bulletVertices.push_back(0.0f);
        }
        if (strcmp(token, "f") == 0) {
            for (int i = 0; i < 3; ++i) {
                token = strtok(nullptr, seps);
                bulletElement.push_back(atof(token) - 1);
            }
        }
    }
    fclose(fp);
}

void Bullet::renderInit() {
    readOBJ("../data/obj/bullet.obj");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bulletVertices.size() * sizeof(float), &bulletVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bulletElement.size() * sizeof(unsigned int), &bulletElement[0], GL_STATIC_DRAW);
    shader = new Shaders("../src/shaders/playerVS.glsl", "../src/shaders/playerFS.glsl");
}

void Bullet::render(glm::mat4 projection, glm::mat4 view, float time) {
    update(time);
    if (die) return;
    shader->useProgram();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0, 1.0f, 0));
    model = glm::translate(model, position);
    float ratio = 0.2f;
    model = glm::scale(model, glm::vec3(ratio, ratio, ratio));
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, bulletElement.size() * sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
}

void Bullet::update(float time) {
    float deltaTime = time - bornTime;
    float y = bornPosition.y + bornDirection.y * deltaTime * speed - g * deltaTime * deltaTime / 2;
    float x = bornPosition.x + bornDirection.x * deltaTime * speed;
    float z = bornPosition.z + bornDirection.z * deltaTime * speed;
    if (y <= 0.0f) {
        y = 0.0f;
        die = true;
    }
    position = glm::vec3(x, y, z);
}

void Player::readOBJ(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr) {
        puts("Error: failed to read player obj file!");
        exit(-1);
    }
    char cmd[1024];
    char seps[] = " ,\t\n";
    while (true) {
        if (fgets(cmd, 1024, fp) == nullptr) break;
        char *token = strtok(cmd, seps);
        if (token == nullptr) continue;

        if (strcmp(token, "v") == 0) {
            for (int i = 0; i < 3; ++i) {
                token = strtok(nullptr, seps);
                playerVertices.push_back(atof(token));
            }
            playerVertices.push_back(0.0f);
            playerVertices.push_back(0.0f);
            playerVertices.push_back(1.0f);
        }
        if (strcmp(token, "f") == 0) {
            for (int i = 0; i < 3; ++i) {
                token = strtok(nullptr, seps);
                playerElement.push_back(atof(token) - 1);
            }
        }
    }
    fclose(fp);
}

void Player::renderInit() {
    readOBJ("../data/obj/player.obj");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, playerVertices.size() * sizeof(float), &playerVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, playerElement.size() * sizeof(unsigned int), &playerElement[0], GL_STATIC_DRAW);
    shader = new Shaders("../src/shaders/playerVS.glsl", "../src/shaders/playerFS.glsl");
}

void Player::render(glm::mat4 projection, glm::mat4 view, float time) {
    if (jumping) jumpUpdate(time);
    shader->useProgram();
    glm::mat4 model;
    float ratio = hp / 100.0f;
    model = glm::translate(model, glm::vec3(0, ratio, 0));
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(ratio, ratio, ratio));
    model = glm::rotate(model, glm::radians(-yaw), glm::vec3(0, 1, 0));
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, playerElement.size() * sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
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