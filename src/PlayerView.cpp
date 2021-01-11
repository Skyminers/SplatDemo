#include <PlayerView.h>

unsigned int bulletVAO, bulletVBO, bulletEBO;
vector<float> bulletVertices;
vector<unsigned int> bulletElement;

unsigned int playerVAO, playerVBO, playerEBO;
vector<float> playerVertices;
vector<unsigned int> playerElement;

void readOBJ(const char *filename, vector<float>& vertices, vector<unsigned int>& elements) {
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
                vertices.push_back(atof(token));
            }
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
        }
        if (strcmp(token, "f") == 0) {
            for (int i = 0; i < 3; ++i) {
                token = strtok(nullptr, seps);
                elements.push_back(atof(token) - 1);
            }
        }
    }
    fclose(fp);
}

void bulletRenderInit() {
    readOBJ("../resource/obj/bullet.obj", bulletVertices, bulletElement);
    glGenVertexArrays(1, &bulletVAO);
    glGenBuffers(1, &bulletVBO);
    glGenBuffers(1, &bulletEBO);
    glBindVertexArray(bulletVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bulletVBO);
    glBufferData(GL_ARRAY_BUFFER, bulletVertices.size() * sizeof(float), &bulletVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bulletElement.size() * sizeof(unsigned int), &bulletElement[0], GL_STATIC_DRAW);
}

void playerRenderInit() {
    readOBJ("../resource/obj/player.obj", playerVertices, playerElement);
    glGenVertexArrays(1, &playerVAO);
    glGenBuffers(1, &playerVBO);
    glGenBuffers(1, &playerEBO);
    glBindVertexArray(playerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
    glBufferData(GL_ARRAY_BUFFER, playerVertices.size() * sizeof(float), &playerVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, playerElement.size() * sizeof(unsigned int), &playerElement[0], GL_STATIC_DRAW);
}

void drawBullet() {
    glBindVertexArray(bulletVAO);
    glDrawElements(GL_TRIANGLES, bulletElement.size() * sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
}

void drawPlayer() {
    glBindVertexArray(playerVAO);
    glDrawElements(GL_TRIANGLES, playerElement.size() * sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
}
void drawParticle() {
    glBindVertexArray(bulletVAO);
    glDrawElements(GL_TRIANGLES, bulletElement.size() * sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
}