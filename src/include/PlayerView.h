#ifndef SPLATDEMO_PLAYERVIEW_H
#define SPLATDEMO_PLAYERVIEW_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cstring>
#include <cstdio>
using namespace std;

void readOBJ(const char *filename, vector<float>& vertices, vector<unsigned int>& elements);
void bulletRenderInit();
void playerRenderInit();
void drawBullet();
void drawPlayer();
void drawParticle();

#endif //SPLATDEMO_PLAYERVIEW_H
