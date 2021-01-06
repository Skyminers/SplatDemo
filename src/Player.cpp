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
//    for (int i = 0; i < playerVertices.size(); ++i)  printf("%.1f ", playerVertices[i]);puts("");
//    for (int i = 0; i < playerElement.size(); ++i) printf("%u ", playerElement[i]);puts("");
    fclose(fp);
}

void Player::renderInit() {
//    for (int i = 0; i < sizeof playerE; ++i) playerE[i]--;
//    printf("%d\n", sizeof playerE);
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

void Player::render(glm::mat4 projection, glm::mat4 view) {
    shader->useProgram();
    glm::mat4 model;
    model = glm::translate(model, position);
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, playerElement.size() * sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
}